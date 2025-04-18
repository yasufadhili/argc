

export type FileType = 'file' | 'folder';

export interface FileSystemNode {
  id: string;
  name: string;
  type: FileType;
  content?: string;
  parentId: string | null;
  children?: string[]; // Array of IDs for folders
  createdAt: number;
  updatedAt: number;
}

export interface FileSystem {
  nodes: Record<string, FileSystemNode>;
  rootId: string;
  currentFileId: string | null;
}

export interface CreateFileOptions {
  name: string;
  type: FileType;
  parentId: string | null;
  content?: string;
}

export interface RenameFileOptions {
  id: string;
  newName: string;
}

export interface MoveFileOptions {
  id: string;
  newParentId: string | null;
}