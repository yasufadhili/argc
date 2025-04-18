import React, { useEffect, useRef, useState } from 'react';
import { AppEventBus } from '../App';
import ansiHTML from "ansi-html";
//import stripAnsi from 'strip-ansi';

// Set up custom ANSI colours
ansiHTML.setColors({
  reset: ['transparent', 'transparent'],
  black: '#3B3B3B',
  red: '#E74C3C',
  green: '#2ECC71',
  yellow: '#F1C40F',
  blue: '#3498DB',
  magenta: '#9B59B6',
  cyan: '#1ABC9C',
  lightgrey: '#BDC3C7',
  darkgrey: '#95A5A6'
});

type MessageType = 'info' | 'error' | 'output' | 'system' | 'command';

interface ConsoleMessage {
  id: string;
  content: string;
  type: MessageType;
  timestamp: number;
  hasAnsi?: boolean;
}

interface ConsolePanelProps {
  onMethodsRegister: (methods: {
    addMessage: (content: string, type: MessageType) => void;
    clearConsole: () => void;
  }) => void;
}

export const ConsolePanel: React.FC<ConsolePanelProps> = ({ onMethodsRegister }) => {
  const [messages, setMessages] = useState<ConsoleMessage[]>([]);
  const [commandHistory, setCommandHistory] = useState<string[]>([]);
  const [historyIndex, setHistoryIndex] = useState(-1);
  const [currentCommand, setCurrentCommand] = useState('');
  const consoleRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);
  const [isAutoScrollEnabled, setIsAutoScrollEnabled] = useState(true);

  const addMessage = (content: string, type: MessageType) => {
    const hasAnsi = content.includes('\x1b[');
    const processedContent = hasAnsi ? content : content;
    
    setMessages((prev) => [...prev, {
      id: Math.random().toString(36).substring(7),
      content: processedContent,
      type,
      timestamp: Date.now(),
      hasAnsi
    }]);

    if (isAutoScrollEnabled && consoleRef.current) {
      setTimeout(() => {
        consoleRef.current?.scrollTo({
          top: consoleRef.current.scrollHeight,
          behavior: 'smooth'
        });
      }, 0);
    }
  };

  const clearConsole = () => {
    setMessages([]);
  };

  useEffect(() => {
    onMethodsRegister({ addMessage, clearConsole });

    
    addMessage('Welcome to ArgonIDE Console', 'system');
    addMessage('Type "help" for available commands', 'system');
  }, []);

  const handleScroll = () => {
    if (!consoleRef.current) return;
    
    const { scrollTop, scrollHeight, clientHeight } = consoleRef.current;
    const isAtBottom = Math.abs(scrollHeight - clientHeight - scrollTop) < 10;
    setIsAutoScrollEnabled(isAtBottom);
  };

  const handleCommand = (command: string) => {
    // Add command to history
    setCommandHistory((prev) => [...prev, command]);
    setHistoryIndex(-1);
    addMessage(`$ ${command}`, 'command');

    // Process built-in commands
    switch (command.toLowerCase()) {
      case 'clear':
        clearConsole();
        break;
      case 'help':
        addMessage(
          'Available commands:\n' +
          '  clear - Clear the console\n' +
          '  help - Show this help message\n' +
          '  version - Show ArgonIDE version\n' +
          '  theme [dark|light] - Set console theme',
          'info'
        );
        break;
      case 'version':
        addMessage('ArgonIDE v0.1.0', 'info');
        break;
      default:
        if (command.startsWith('theme ')) {
          const theme = command.split(' ')[1];
          if (theme === 'dark' || theme === 'light') {
            document.documentElement.classList.toggle('dark', theme === 'dark');
            addMessage(`Theme set to ${theme}`, 'system');
          } else {
            addMessage('Invalid theme. Use "theme dark" or "theme light"', 'error');
          }
        } else {
          addMessage(`Command not found: ${command}`, 'error');
        }
    }
  };

  const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Enter' && currentCommand.trim()) {
      handleCommand(currentCommand.trim());
      setCurrentCommand('');
    } else if (e.key === 'ArrowUp') {
      e.preventDefault();
      if (historyIndex < commandHistory.length - 1) {
        const newIndex = historyIndex + 1;
        setHistoryIndex(newIndex);
        setCurrentCommand(commandHistory[commandHistory.length - 1 - newIndex]);
      }
    } else if (e.key === 'ArrowDown') {
      e.preventDefault();
      if (historyIndex > 0) {
        const newIndex = historyIndex - 1;
        setHistoryIndex(newIndex);
        setCurrentCommand(commandHistory[commandHistory.length - 1 - newIndex]);
      } else if (historyIndex === 0) {
        setHistoryIndex(-1);
        setCurrentCommand('');
      }
    }
  };

  const renderMessage = (message: ConsoleMessage) => {
    let className = 'font-mono text-sm mb-1 ';
    let content = message.content;

    switch (message.type) {
      case 'error':
        className += 'text-red-500';
        break;
      case 'info':
        className += 'text-blue-500';
        break;
      case 'system':
        className += 'text-gray-400 italic';
        break;
      case 'command':
        className += 'text-green-500 font-bold';
        break;
      default:
        className += 'text-gray-300';
    }

    if (message.hasAnsi) {
      return (
        <div
          key={message.id}
          className={className}
          dangerouslySetInnerHTML={{ __html: ansiHTML(content) }}
        />
      );
    }

    return (
      <div key={message.id} className={className}>
        <span className="text-gray-500 mr-2">
          {new Date(message.timestamp).toLocaleTimeString()}
        </span>
        {content}
      </div>
    );
  };

  return (
    <div className="h-full flex flex-col">
      <div className="p-2 border-b border-gray-200 dark:border-gray-700 flex justify-between items-center">
        <div className="font-semibold">Console</div>
        <div className="flex gap-2">
          <button
            className="text-sm px-2 py-1 bg-gray-200 dark:bg-gray-700 rounded hover:bg-gray-300 dark:hover:bg-gray-600"
            onClick={() => AppEventBus.clearConsole.dispatchEvent(new Event('clear'))}
            title="Clear Console (or type 'clear')"
          >
            Clear
          </button>
          <button
            className={`text-sm px-2 py-1 rounded ${
              isAutoScrollEnabled
                ? 'bg-blue-500 hover:bg-blue-600 text-white'
                : 'bg-gray-200 dark:bg-gray-700 hover:bg-gray-300 dark:hover:bg-gray-600'
            }`}
            onClick={() => setIsAutoScrollEnabled(!isAutoScrollEnabled)}
            title="Toggle Auto-scroll"
          >
            {isAutoScrollEnabled ? 'Auto-scroll: On' : 'Auto-scroll: Off'}
          </button>
        </div>
      </div>

      <div
        ref={consoleRef}
        className="flex-1 overflow-auto p-2 bg-gray-900 font-mono text-sm"
        onScroll={handleScroll}
      >
        {messages.map(renderMessage)}
      </div>

      <div className="border-t border-gray-200 dark:border-gray-700 p-2 bg-gray-900">
        <div className="flex items-center">
          <span className="text-green-500 mr-2">$</span>
          <input
            ref={inputRef}
            type="text"
            value={currentCommand}
            onChange={(e) => setCurrentCommand(e.target.value)}
            onKeyDown={handleKeyDown}
            className="flex-1 bg-transparent border-none outline-none text-gray-300 font-mono"
            placeholder="Type a command..."
            spellCheck={false}
          />
        </div>
      </div>
    </div>
  );
}; 