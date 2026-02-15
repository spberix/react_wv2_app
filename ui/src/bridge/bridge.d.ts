import { PlatformInfo, BridgeResponse } from './types';
/**
 * Bridge API for communicating with C++ backend
 */
declare class NativeBridge {
    /**
     * Check if the bridge is available
     */
    isAvailable(): boolean;
    /**
     * Minimize the window
     */
    minimize(): Promise<BridgeResponse>;
    /**
     * Maximize or restore the window
     */
    maximize(): Promise<BridgeResponse>;
    /**
     * Close the application
     */
    close(): Promise<BridgeResponse>;
    /**
     * Check if the window is maximized
     */
    isMaximized(): Promise<boolean>;
    /**
     * Get platform information
     */
    getPlatform(): Promise<PlatformInfo | null>;
    /**
     * Demo: Reverse a string using C++
     */
    reverseString(text: string): Promise<string>;
    /**
     * Demo: Get random number from C++
     */
    getRandomNumber(): Promise<number>;
}
export declare const bridge: NativeBridge;
export {};
//# sourceMappingURL=bridge.d.ts.map