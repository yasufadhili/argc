
/** FUTURE
 * Export project - Each project has a UUID for better export, Exported in ZIP file as project name
 * View directoies as is in the local file system
 * Create accounts to save projects
 * Better Top Section, Console, and FileExplorer
 * Think of adding a top tab bar
 * Switch the Console section to tabs for console output, assembly output, memory view
 */

import React, { useState, useRef, useCallback } from 'react';
import { useFileSystemStore } from '../stores/fileSystemStore';
import { FileType } from '../types/filesystem.types';

interface ContextMenuState {
  visible: boolean;
  x: number;
  y: number;
  nodeId: string | null;
}

interface FileIconProps {
  type: FileType;
  name: string;
}

const FileIcon: React.FC<FileIconProps> = ({ type, name }) => {
  const getIcon = () => {
    if (type === 'folder') {
      return '📁';
    }
    // File type icons based on extension
    const ext = name.split('.').pop()?.toLowerCase();
    switch (ext) {
      case 'ar':
        return '📜';
      case 'json':
        return '📋';
      case 'md':
        return '📝';
      default:
        return '📄';
    }
  };

  return <span className="mr-2">{getIcon()}</span>;
};

export const FileExplorer: React.FC = () => {
  const {
    nodes,
    currentFileId,
    rootId,
    setCurrentFile,
    createFile,
    deleteFile,
    renameFile,
    moveFile,
  } = useFileSystemStore();

  const [contextMenu, setContextMenu] = useState<ContextMenuState>({
    visible: false,
    x: 0,
    y: 0,
    nodeId: null,
  });

  const [expandedFolders, setExpandedFolders] = useState<Set<string>>(
    new Set(rootId ? [rootId] : [])
  );
  const [renamingNode, setRenamingNode] = useState<string | null>(null);
  const [newName, setNewName] = useState('');
  const [draggedNode, setDraggedNode] = useState<string | null>(null);
  const [dropTarget, setDropTarget] = useState<string | null>(null);
  const fileInputRef = useRef<HTMLInputElement>(null);

  const toggleFolder = (folderId: string) => {
    setExpandedFolders((prev) => {
      const next = new Set(prev);
      if (next.has(folderId)) {
        next.delete(folderId);
      } else {
        next.add(folderId);
      }
      return next;
    });
  };

  const handleContextMenu = (e: React.MouseEvent, nodeId: string) => {
    e.preventDefault();
    setContextMenu({
      visible: true,
      x: e.clientX,
      y: e.clientY,
      nodeId,
    });
  };

  const handleCreateFile = (parentId: string | null, type: FileType) => {
    const baseName = type === 'file' ? 'new_file.ar' : 'new_folder';
    let name = baseName;
    let counter = 1;
    
    // Avoid name conflicts
    while (Object.values(nodes).some(node => 
      node.parentId === parentId && node.name === name
    )) {
      name = `${baseName.replace(/(\.[^.]+)?$/, '')}${counter}${type === 'file' ? '.ar' : ''}`;
      counter++;
    }
    
    createFile({ name, type, parentId });
    setContextMenu({ ...contextMenu, visible: false });
  };

  const handleDeleteFile = (id: string) => {
    deleteFile(id);
    setContextMenu({ ...contextMenu, visible: false });
  };

  const handleRenameStart = (id: string) => {
    setRenamingNode(id);
    setNewName(nodes[id].name);
    setContextMenu({ ...contextMenu, visible: false });
  };

  const handleRenameSubmit = (e: React.FormEvent, id: string) => {
    e.preventDefault();
    try {
      renameFile(id, newName);
      setRenamingNode(null);
    } catch (error) {
      console.error('Failed to rename:', error);
    }
  };

  const handleDragStart = (e: React.DragEvent, nodeId: string) => {
    e.dataTransfer.setData('text/plain', nodeId);
    setDraggedNode(nodeId);
  };

  const handleDragOver = (e: React.DragEvent, nodeId: string) => {
    e.preventDefault();
    if (draggedNode && draggedNode !== nodeId) {
      setDropTarget(nodeId);
    }
  };

  const handleDrop = (e: React.DragEvent, targetId: string) => {
    e.preventDefault();
    const sourceId = e.dataTransfer.getData('text/plain');
    
    if (sourceId && targetId !== sourceId) {
      const targetNode = nodes[targetId];
      if (targetNode.type === 'folder') {
        moveFile(sourceId, targetId);
      } else {
        moveFile(sourceId, targetNode.parentId);
      }
    }
    
    setDraggedNode(null);
    setDropTarget(null);
  };

  const handleDragEnd = () => {
    setDraggedNode(null);
    setDropTarget(null);
  };

  const handleOpenLocalFile = () => {
    fileInputRef.current?.click();
  };

  const handleFileInputChange = async (e: React.ChangeEvent<HTMLInputElement>) => {
    const files = e.target.files;
    if (!files) return;

    for (const file of Array.from(files)) {
      const content = await file.text();
      createFile({
        name: file.name,
        type: 'file',
        parentId: contextMenu.nodeId,
        content,
      });
    }
    
    if (fileInputRef.current) {
      fileInputRef.current.value = '';
    }
  };

  const renderNode = useCallback((nodeId: string, depth: number = 0) => {
    const node = nodes[nodeId];
    if (!node) return null;

    const isExpanded = expandedFolders.has(nodeId);
    const isFolder = node.type === 'folder';
    const isDragging = draggedNode === nodeId;
    const isDropTarget = dropTarget === nodeId;

    return (
      <div
        key={nodeId}
        className={`select-none ${isDragging ? 'opacity-50' : ''}`}
        style={{ paddingLeft: `${depth * 16}px` }}
      >
        <div
          className={`flex items-center p-1 hover:bg-gray-100 dark:hover:bg-gray-800 cursor-pointer rounded ${
            currentFileId === nodeId ? 'bg-blue-100 dark:bg-blue-900' : ''
          } ${isDropTarget ? 'border-2 border-blue-500' : ''}`}
          onClick={() => isFolder ? toggleFolder(nodeId) : setCurrentFile(nodeId)}
          onContextMenu={(e) => handleContextMenu(e, nodeId)}
          draggable
          onDragStart={(e) => handleDragStart(e, nodeId)}
          onDragOver={(e) => handleDragOver(e, nodeId)}
          onDrop={(e) => handleDrop(e, nodeId)}
          onDragEnd={handleDragEnd}
        >
          {isFolder && (
            <span className="mr-1">{isExpanded ? '▼' : '▶'}</span>
          )}
          <FileIcon type={node.type} name={node.name} />
          {renamingNode === nodeId ? (
            <form
              onSubmit={(e) => handleRenameSubmit(e, nodeId)}
              className="flex-1"
              onClick={(e) => e.stopPropagation()}
            >
              <input
                type="text"
                value={newName}
                onChange={(e) => setNewName(e.target.value)}
                onBlur={() => setRenamingNode(null)}
                autoFocus
                className="w-full bg-transparent border-b border-gray-300 dark:border-gray-600 focus:outline-none px-1"
              />
            </form>
          ) : (
            <span className="flex-1 truncate">{node.name}</span>
          )}
        </div>
        {isFolder && isExpanded && node.children?.map((childId) =>
          renderNode(childId, depth + 1)
        )}
      </div>
    );
  }, [nodes, currentFileId, expandedFolders, draggedNode, dropTarget, renamingNode, newName]);

  return (
    <div className="h-full flex flex-col">
      {/* Header */}
      <div className="p-2 border-b border-gray-200 dark:border-gray-700 flex justify-between items-center">
        <div className="font-semibold">Files</div>
        <button
          className="p-1 hover:bg-gray-100 dark:hover:bg-gray-800 rounded"
          onClick={() => handleCreateFile(null, 'folder')}
          title="New Root Folder"
        >
          📁+
        </button>
      </div>

      {/* File Tree */}
      <div className="flex-1 overflow-auto p-2">
        {Object.values(nodes)
          .filter((node) => node.parentId === null)
          .map((node) => renderNode(node.id))}
      </div>

      {/* Context Menu */}
      {contextMenu.visible && (
        <div
          className="fixed bg-white dark:bg-gray-800 shadow-lg rounded-md py-1 z-50"
          style={{ top: contextMenu.y, left: contextMenu.x }}
        >
          <button
            className="w-full px-4 py-2 text-left hover:bg-gray-100 dark:hover:bg-gray-700 flex items-center"
            onClick={() => handleCreateFile(contextMenu.nodeId, 'file')}
          >
            <span className="mr-2">📄</span> New File
          </button>
          <button
            className="w-full px-4 py-2 text-left hover:bg-gray-100 dark:hover:bg-gray-700 flex items-center"
            onClick={() => handleCreateFile(contextMenu.nodeId, 'folder')}
          >
            <span className="mr-2">📁</span> New Folder
          </button>
          <button
            className="w-full px-4 py-2 text-left hover:bg-gray-100 dark:hover:bg-gray-700 flex items-center"
            onClick={handleOpenLocalFile}
          >
            <span className="mr-2">📥</span> Open Local File
          </button>
          {contextMenu.nodeId && (
            <>
              <hr className="my-1 border-gray-200 dark:border-gray-700" />
              <button
                className="w-full px-4 py-2 text-left hover:bg-gray-100 dark:hover:bg-gray-700 flex items-center"
                onClick={() => handleRenameStart(contextMenu.nodeId!)}
              >
                <span className="mr-2">✏️</span> Rename
              </button>
              <button
                className="w-full px-4 py-2 text-left text-red-500 hover:bg-gray-100 dark:hover:bg-gray-700 flex items-center"
                onClick={() => handleDeleteFile(contextMenu.nodeId!)}
              >
                <span className="mr-2">🗑️</span> Delete
              </button>
            </>
          )}
        </div>
      )}

      {/* Hidden file input for opening local files */}
      <input
        type="file"
        ref={fileInputRef}
        className="hidden"
        onChange={handleFileInputChange}
        multiple
      />

      {/* Click anywhere to close context menu */}
      {contextMenu.visible && (
        <div
          className="fixed inset-0 z-40"
          onClick={() => setContextMenu({ ...contextMenu, visible: false })}
        />
      )}
    </div>
  );
}; 

