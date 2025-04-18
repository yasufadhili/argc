export interface CompilationResult {
  success: boolean;
  output?: string;
  error?: string;
  assembly?: string;
  memoryMap?: MemorySnapshot;
}

export interface MemorySnapshot {
  stack: MemoryRegion[];
  heap: MemoryRegion[];
  globals: MemoryRegion[];
}

export interface MemoryRegion {
  address: string;
  size: number;
  name?: string;
  value?: string;
  type?: string;
}
