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
declare global {
    interface Window {
        minimize: () => Promise<string>;
        maximize: () => Promise<string>;
        closeApp: () => Promise<string>;
        isMaximized: () => Promise<string>;
        getPlatform: () => Promise<string>;
        reverseString: (text: string) => Promise<string>;
        getRandomNumber: () => Promise<string>;
    }
}
export {};
//# sourceMappingURL=types.d.ts.map