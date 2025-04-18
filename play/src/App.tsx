import React, { useEffect, useRef } from 'react';
import { MainLayout } from './components/Layout';
import { useCompiler } from './hooks/useCompiler';
import { useFileSystemStore } from './stores/fileSystemStore';

type MessageType = 'info' | 'error' | 'output';

// Global event bus for simple cross-component communication
export const AppEventBus = {
  runCode: new EventTarget(),
  saveProject: new EventTarget(),
  clearConsole: new EventTarget(),
  // Will add more events later
};

const App: React.FC = () => {
  const { compileAndRun } = useCompiler();
  const { currentFileId } = useFileSystemStore();
  const consoleMethods = useRef<{
    addMessage?: (content: string, type: MessageType) => void;
    clearConsole?: () => void;
  }>({});
  

  useEffect(() => {
    const handleRunCode = async () => {
      if (!currentFileId) {
        consoleMethods.current.addMessage?.('No file selected for execution', 'error');
        return;
      }
      
      consoleMethods.current.addMessage?.('Compiling and running...', 'info');
      
      const result = await compileAndRun();
      
      if (result.success) {
        consoleMethods.current.addMessage?.('Compilation successful', 'info');
        if (result.output) {
          consoleMethods.current.addMessage?.(result.output, 'output');
        }
      } else {
        consoleMethods.current.addMessage?.(result.error || 'Unknown error', 'error');
      }
    };
    
    const runListener = () => handleRunCode();
    AppEventBus.runCode.addEventListener('run', runListener);
    
    return () => {
      AppEventBus.runCode.removeEventListener('run', runListener);
    };
  }, [currentFileId, compileAndRun]);
  
  // Register console methods for global usage
  const registerConsoleMethods = (methods: {
    addMessage: (content: string, type: MessageType) => void;
    clearConsole: () => void;
  }) => {
    consoleMethods.current = methods;
  };

  // Handle save project events
  useEffect(() => {
    const handleSaveProject = () => {
      // Implement project saving logic here later
      consoleMethods.current.addMessage?.('Project saved successfully', 'info');
    };

    const saveListener = () => handleSaveProject();
    AppEventBus.saveProject.addEventListener('save', saveListener);

    return () => {
      AppEventBus.saveProject.removeEventListener('save', saveListener);
    };
  }, []);

  // Handle clear console events
  useEffect(() => {
    const handleClearConsole = () => {
      consoleMethods.current.clearConsole?.();
    };

    const clearListener = () => handleClearConsole();
    AppEventBus.clearConsole.addEventListener('clear', clearListener);

    return () => {
      AppEventBus.clearConsole.removeEventListener('clear', clearListener);
    };
  }, []);

  return (
    <div className="h-screen w-screen bg-gray-100 dark:bg-gray-900">
      <MainLayout onConsoleMethodsRegister={registerConsoleMethods} />
    </div>
  );
};

export default App;