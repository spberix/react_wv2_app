// Type definitions for C++ bridge functions

export interface PlatformInfo {
  platform: 'windows' | 'macos' | 'linux';
}

export interface WindowState {
  maximized: boolean;
}

export interface BridgeResponse {
  status: 'ok' | 'error' | 'not_implemented';
  message?: string;
}

// Extend the Window interface with C++ bridge functions
declare global {
  interface Window {
    // Window control functions
    minimize: () => Promise<string>;
    maximize: () => Promise<string>;
    closeApp: () => Promise<string>;
    isMaximized: () => Promise<string>;

    // Platform information
    getPlatform: () => Promise<string>;

    // Demo functions
    reverseString: (text: string) => Promise<string>;
    getRandomNumber: () => Promise<string>;
  }
}

export {};
