import React, { useRef } from 'react';
import Editor, { Monaco } from '@monaco-editor/react';
import { useFileSystemStore } from '../stores/fileSystemStore';
import { AppEventBus } from '../App';
import { languages } from 'monaco-editor';

// Define Argon language configuration
const argonLanguageConfig: languages.LanguageConfiguration = {
  comments: {
    lineComment: '//',
    blockComment: ['/*', '*/'] as [string, string],
  },
  brackets: [
    ['{', '}'],
    ['[', ']'],
    ['(', ')'],
  ],
  autoClosingPairs: [
    { open: '{', close: '}' },
    { open: '[', close: ']' },
    { open: '(', close: ')' },
    { open: '"', close: '"' },
    { open: "'", close: "'" },
  ],
  surroundingPairs: [
    { open: '{', close: '}' },
    { open: '[', close: ']' },
    { open: '(', close: ')' },
    { open: '"', close: '"' },
    { open: "'", close: "'" },
  ],
};

// Define Argon syntax highlighting rules
const argonMonarchTokens: languages.IMonarchLanguage = {
  defaultToken: 'invalid',
  tokenPostfix: '.argon',
  keywords: [
    'fn', 'let', 'mut', 'return', 'if', 'else', 'while', 'for', 'in',
    'break', 'continue', 'struct', 'enum', 'impl', 'trait', 'pub',
    'unsafe', 'where', 'match', 'type', 'const', 'static', 'use',
    'mod', 'as', 'true', 'false', 'self', 'super', 'i8', 'i16',
    'i32', 'i64', 'u8', 'u16', 'u32', 'u64', 'f32', 'f64',
    'isize', 'usize', 'bool', 'char', 'str', 'String'
  ],

  typeKeywords: [
    'i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64',
    'f32', 'f64', 'isize', 'usize', 'bool', 'char', 'str'
  ],

  operators: [
    '=', '>', '<', '!', '~', '?', ':',
    '==', '<=', '>=', '!=', '&&', '||', '++', '--',
    '+', '-', '*', '/', '&', '|', '^', '%', '<<',
    '>>', '>>>', '+=', '-=', '*=', '/=', '&=', '|=',
    '^=', '%=', '<<=', '>>=', '>>>='
  ],

  symbols: /[=><!~?:&|+\-*\/\^%]+/,

  escapes: /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,4}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8})/,

  tokenizer: {
    root: [
      // Identifiers and keywords
      [/[a-z_$][\w$]*/, { cases: {
        '@typeKeywords': 'type',
        '@keywords': 'keyword',
        '@default': 'identifier'
      }}],

      // Whitespace
      { include: '@whitespace' },

      // Delimiters and operators
      [/[{}()\[\]]/, '@brackets'],
      [/[<>](?!@symbols)/, '@brackets'],
      [/@symbols/, { cases: {
        '@operators': 'operator',
        '@default': ''
      }}],

      // Numbers
      [/\d*\.\d+([eE][\-+]?\d+)?/, 'number.float'],
      [/0[xX][0-9a-fA-F]+/, 'number.hex'],
      [/\d+/, 'number'],

      // Strings
      [/"([^"\\]|\\.)*$/, 'string.invalid'],
      [/"/, { token: 'string.quote', bracket: '@open', next: '@string' }],

      // Characters
      [/'[^\\']'/, 'string'],
      [/(')(@escapes)(')/, ['string', 'string.escape', 'string']],
      [/'/, 'string.invalid']
    ],

    comment: [
      [/[^\/*]+/, 'comment'],
      [/\/\*/, 'comment', '@push'],
      ["\\*/", 'comment', '@pop'],
      [/[\/*]/, 'comment']
    ],

    string: [
      [/[^\\"]+/, 'string'],
      [/@escapes/, 'string.escape'],
      [/\\./, 'string.escape.invalid'],
      [/"/, { token: 'string.quote', bracket: '@close', next: '@pop' }]
    ],

    whitespace: [
      [/[ \t\r\n]+/, 'white'],
      [/\/\*/, 'comment', '@comment'],
      [/\/\/.*$/, 'comment'],
    ],
  }
};

export const CodeEditor: React.FC = () => {
  const { currentFileId, nodes, updateFileContent } = useFileSystemStore();
  const editorRef = useRef<any>(null);

  const handleEditorDidMount = (editor: any, monaco: Monaco) => {
    editorRef.current = editor;

    // Register Argon language
    monaco.languages.register({ id: 'argon' });

    // Configure Argon language
    monaco.languages.setMonarchTokensProvider('argon', argonMonarchTokens);
    monaco.languages.setLanguageConfiguration('argon', argonLanguageConfig);

    // Set editor theme
    monaco.editor.defineTheme('argonDark', {
      base: 'vs-dark',
      inherit: true,
      rules: [
        { token: 'keyword', foreground: '569CD6', fontStyle: 'bold' },
        { token: 'type', foreground: '4EC9B0' },
        { token: 'string', foreground: 'CE9178' },
        { token: 'number', foreground: 'B5CEA8' },
        { token: 'comment', foreground: '6A9955' },
        { token: 'operator', foreground: 'D4D4D4' },
        { token: 'identifier', foreground: '9CDCFE' },
      ],
      colors: {
        'editor.background': '#1E1E1E',
        'editor.foreground': '#D4D4D4',
        'editor.lineHighlightBackground': '#2F2F2F',
        'editor.selectionBackground': '#264F78',
        'editor.inactiveSelectionBackground': '#3A3D41',
      },
    });

    monaco.editor.setTheme('argonDark');
  };

  const handleEditorChange = (value: string | undefined) => {
    if (currentFileId && value !== undefined) {
      updateFileContent(currentFileId, value);
    }
  };

  const handleRunCode = () => {
    AppEventBus.runCode.dispatchEvent(new Event('run'));
  };

  const handleSaveProject = () => {
    AppEventBus.saveProject.dispatchEvent(new Event('save'));
  };

  const currentFile = currentFileId ? nodes[currentFileId] : null;

  return (
    <div className="h-full flex flex-col">
      {/* Toolbar */}
      <div className="p-2 border-b border-gray-200 dark:border-gray-700 flex gap-2">
        <button
          className="px-3 py-1 bg-blue-500 text-white rounded hover:bg-blue-600"
          onClick={handleRunCode}
          disabled={!currentFile}
        >
          Run
        </button>
        <button
          className="px-3 py-1 bg-green-500 text-white rounded hover:bg-green-600"
          onClick={handleSaveProject}
        >
          Save
        </button>
      </div>

      {/* Editor */}
      <div className="flex-1">
        {currentFile ? (
          <Editor
            height="100%"
            defaultLanguage="argon"
            language="argon"
            value={currentFile.content}
            onChange={handleEditorChange}
            onMount={handleEditorDidMount}
            //theme="argonDark"
            theme='vs-dark'
            options={{
              minimap: { enabled: true },
              fontSize: 16,
              lineNumbers: 'on',
              roundedSelection: false,
              scrollBeyondLastLine: false,
              readOnly: false,
              automaticLayout: true,
              suggestOnTriggerCharacters: true,
              tabSize: 2,
              wordBasedSuggestions: 'currentDocument',
              folding: true,
              renderLineHighlight: 'all',
              scrollbar: {
                vertical: 'visible',
                horizontal: 'visible',
              },
            }}
          />
        ) : (
          <div className="h-full flex items-center justify-center text-gray-500 dark:text-gray-400">
            Select a file to edit
          </div>
        )}
      </div>
    </div>
  );
}; 