import { create } from 'zustand';
import { nanoid } from 'nanoid';
import { persist } from 'zustand/middleware';
import { 
  FileSystem, 
  FileSystemNode, 
  CreateFileOptions, 
  RenameFileOptions, 
  MoveFileOptions 
} from '../types/filesystem.types';

export interface FileNode {
  id: string;
  name: string;
  type: 'file' | 'folder';
  parentId: string | null;
  content?: string;
  children?: string[];
  createdAt: number;
  updatedAt: number;
}

interface FileSystemState {
  nodes: Record<string, FileNode>;
  currentFileId: string | null;
  rootId: string | null;
  
  // File operations
  createFile: (options: { name: string; type: 'file' | 'folder'; parentId: string | null; content?: string }) => string;
  deleteFile: (id: string) => void;
  renameFile: (id: string, newName: string) => void;
  moveFile: (id: string, newParentId: string | null) => void;
  updateFileContent: (id: string, content: string) => void;
  setCurrentFile: (id: string | null) => void;
  
  // Helper functions
  getFileContent: (id: string) => string | undefined;
  getNodePath: (id: string) => string;
  getNodeChildren: (id: string) => FileNode[];
  isValidFileName: (name: string, parentId: string | null) => boolean;
}

interface FileSystemStore extends FileSystem {
  // Actions
  createFile: (options: CreateFileOptions) => string;
  deleteFile: (id: string) => void;
  renameFile: (options: RenameFileOptions) => void;
  moveFile: (options: MoveFileOptions) => void;
  updateFileContent: (id: string, content: string) => void;
  setCurrentFile: (id: string | null) => void;
  
  // Helper functions
  getNodePath: (id: string) => string;
  getNodeChildren: (id: string) => FileSystemNode[];
  isValidFileName: (name: string, parentId: string | null) => boolean;
}

// Initial file system with a default project structure
const createInitialFileSystem = (): FileSystem => {
  const rootId = nanoid();
  const srcFolderId = nanoid();
  const mainFileId = nanoid();
  const timestamp = Date.now();
  
  return {
    nodes: {
      [rootId]: {
        id: rootId,
        name: 'project',
        type: 'folder',
        parentId: null,
        children: [srcFolderId],
        createdAt: timestamp,
        updatedAt: timestamp,
      },
      [srcFolderId]: {
        id: srcFolderId,
        name: 'src',
        type: 'folder',
        parentId: rootId,
        children: [mainFileId],
        createdAt: timestamp,
        updatedAt: timestamp,
      },
      [mainFileId]: {
        id: mainFileId,
        name: 'main.argon',
        type: 'file',
        parentId: srcFolderId,
        content: '// Welcome to ArgonIDE!\n\nfn main() {\n    println("Hello, Argon!");\n}\n',
        createdAt: timestamp,
        updatedAt: timestamp,
      },
    },
    rootId,
    currentFileId: mainFileId,
  };
};

export const useFileSystemStore = create<FileSystemState>()(
  persist(
    (set, get) => ({
      ...createInitialFileSystem(),
      
      createFile: (options) => {
        const { name, type, parentId, content = '' } = options;
        const id = nanoid();
        const timestamp = Date.now();
        
        if (!get().isValidFileName(name, parentId)) {
          throw new Error(`Invalid file name or file already exists: ${name}`);
        }
        
        set((state) => {
          const newNodes = { ...state.nodes };
          
          // Create the new node
          newNodes[id] = {
            id,
            name,
            type,
            content: type === 'file' ? content : undefined,
            parentId,
            children: type === 'folder' ? [] : undefined,
            createdAt: timestamp,
            updatedAt: timestamp,
          };
          
          // Update parent's children if parent exists
          if (parentId !== null && newNodes[parentId]) {
            newNodes[parentId] = {
              ...newNodes[parentId],
              children: [...(newNodes[parentId].children || []), id],
              updatedAt: timestamp,
            };
          }
          
          return { nodes: newNodes };
        });
        
        return id;
      },
      
      deleteFile: (id) => {
        const node = get().nodes[id];
        if (!node) return;
        
        // Helper to recursively get all descendant ids
        const getAllDescendantIds = (nodeId: string): string[] => {
          const node = get().nodes[nodeId];
          if (!node || !node.children || node.children.length === 0) return [nodeId];
          
          return [
            nodeId,
            ...node.children.flatMap(childId => getAllDescendantIds(childId))
          ];
        };
        
        const idsToRemove = getAllDescendantIds(id);
        
        set((state) => {
          const newNodes = { ...state.nodes };
          
          // Update parent's children list
          if (node.parentId && newNodes[node.parentId]) {
            newNodes[node.parentId] = {
              ...newNodes[node.parentId],
              children: (newNodes[node.parentId].children || []).filter(childId => childId !== id),
              updatedAt: Date.now(),
            };
          }
          
          // Remove all nodes
          idsToRemove.forEach(idToRemove => {
            delete newNodes[idToRemove];
          });
          
          // Update current file if needed
          let currentFileId = state.currentFileId;
          if (currentFileId && idsToRemove.includes(currentFileId)) {
            currentFileId = null;
          }
          
          return { nodes: newNodes, currentFileId };
        });
      },
      
      renameFile: (id, newName) => {
        const node = get().nodes[id];
        if (!node) return;
        
        if (!get().isValidFileName(newName, node.parentId)) {
          throw new Error(`Invalid file name or file already exists: ${newName}`);
        }
        
        set((state) => ({
          nodes: {
            ...state.nodes,
            [id]: {
              ...state.nodes[id],
              name: newName,
              updatedAt: Date.now(),
            },
          },
        }));
      },
      
      moveFile: (id, newParentId) => {
        const node = get().nodes[id];
        const newParent = newParentId !== null ? get().nodes[newParentId] : null;
        
        if (!node) return;
        if (newParentId !== null && (!newParent || newParent.type !== 'folder')) return;
        
        // Check for circular reference
        if (node.type === 'folder') {
          let currentId = newParentId;
          while (currentId !== null) {
            if (currentId === id) return; // Would create a cycle
            currentId = get().nodes[currentId]?.parentId || null;
          }
        }
        
        // Check if a file with the same name already exists in the target folder
        if (newParentId !== null && newParent && 
            newParent.children?.some(childId => 
              get().nodes[childId].name === node.name)) {
          throw new Error(`A file with name "${node.name}" already exists in the target folder`);
        }
        
        set((state) => {
          const newNodes = { ...state.nodes };
          const timestamp = Date.now();
          
          // Remove from old parent
          if (node.parentId !== null && newNodes[node.parentId]) {
            newNodes[node.parentId] = {
              ...newNodes[node.parentId],
              children: (newNodes[node.parentId].children || []).filter(childId => childId !== id),
              updatedAt: timestamp,
            };
          }
          
          // Add to new parent
          if (newParentId !== null && newNodes[newParentId]) {
            newNodes[newParentId] = {
              ...newNodes[newParentId],
              children: [...(newNodes[newParentId].children || []), id],
              updatedAt: timestamp,
            };
          }
          
          // Update the node itself
          newNodes[id] = {
            ...newNodes[id],
            parentId: newParentId,
            updatedAt: timestamp,
          };
          
          return { nodes: newNodes };
        });
      },
      
      updateFileContent: (id, content) => {
        const node = get().nodes[id];
        if (!node || node.type !== 'file') return;
        
        set((state) => ({
          nodes: {
            ...state.nodes,
            [id]: {
              ...state.nodes[id],
              content,
              updatedAt: Date.now(),
            },
          },
        }));
      },
      
      setCurrentFile: (id) => {
        set({ currentFileId: id });
      },
      
      getFileContent: (id) => {
        const state = get();
        return state.nodes[id]?.content;
      },
      
      getNodePath: (id) => {
        const node = get().nodes[id];
        if (!node) return '';
        
        if (node.parentId === null) return node.name;
        
        return `${get().getNodePath(node.parentId)}/${node.name}`;
      },
      
      getNodeChildren: (id) => {
        const node = get().nodes[id];
        if (!node || !node.children) return [];
        
        return node.children
          .map(childId => get().nodes[childId])
          .filter(Boolean)
          .sort((a, b) => {
            // Folders first, then files
            if (a.type !== b.type) {
              return a.type === 'folder' ? -1 : 1;
            }
            // Alphabetical within same type
            return a.name.localeCompare(b.name);
          });
      },
      
      isValidFileName: (name, parentId) => {
        // Basic validation
        if (!name || name.trim() === '') return false;
        if (name.includes('/') || name.includes('\\')) return false;
        
        // Check for duplicates in the same directory
        if (parentId !== null) {
          const parent = get().nodes[parentId];
          if (!parent || !parent.children) return true;
          
          return !parent.children.some(childId => 
            get().nodes[childId]?.name === name
          );
        }
        
        return true;
      },
    }),
    {
      name: 'argon-ide-filesystem',
    }
  )
);