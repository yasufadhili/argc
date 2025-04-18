import React from 'react';
import { FileExplorer } from './FileExplorer';
import { CodeEditor } from './CodeEditor';
import { ConsolePanel } from './ConsolePanel';

interface MainLayoutProps {
  onConsoleMethodsRegister: (methods: {
    addMessage: (content: string, type: string) => void;
    clearConsole: () => void;
  }) => void;
}

export const MainLayout: React.FC<MainLayoutProps> = ({ onConsoleMethodsRegister }) => {
  return (
    <div className="flex h-full w-full">
      {/* Left Panel - File Explorer */}
      <div className="w-64 border-r border-gray-200 dark:border-gray-700">
        <FileExplorer />
      </div>

      {/* Middle Panel - Code Editor */}
      <div className="flex-1">
        <CodeEditor />
      </div>

      {/* Right Panel - Console & Debug */}
      <div className="w-96 border-l border-gray-200 dark:border-gray-700">
        <ConsolePanel onMethodsRegister={onConsoleMethodsRegister} />
      </div>
    </div>
  );
}; 