import { PlatformInfo, BridgeResponse } from './types';

/**
 * Bridge API for communicating with C++ backend
 */
class NativeBridge {
  /**
   * Check if the bridge is available
   */
  isAvailable(): boolean {
    return (
      typeof window.minimize === 'function' &&
      typeof window.maximize === 'function' &&
      typeof window.closeApp === 'function'
    );
  }

  /**
   * Minimize the window
   */
  async minimize(): Promise<BridgeResponse> {
    try {
      const result: any = await window.minimize();
      return result;
    } catch (error) {
      console.error('Failed to minimize window:', error);
      return { status: 'error', message: String(error) };
    }
  }

  /**
   * Maximize or restore the window
   */
  async maximize(): Promise<BridgeResponse> {
    try {
      const result: any = await window.maximize();
      return result;
    } catch (error) {
      console.error('Failed to maximize window:', error);
      return { status: 'error', message: String(error) };
    }
  }

  /**
   * Close the application
   */
  async close(): Promise<BridgeResponse> {
    try {
      const result: any = await window.closeApp();
      return result;
    } catch (error) {
      console.error('Failed to close application:', error);
      return { status: 'error', message: String(error) };
    }
  }

  /**
   * Check if the window is maximized
   */
  async isMaximized(): Promise<boolean> {
    try {
      const data: any = await window.isMaximized();
      return data.maximized;
    } catch (error) {
      console.error('Failed to check window state:', error);
      return false;
    }
  }

  /**
   * Get platform information
   */
  async getPlatform(): Promise<PlatformInfo | null> {
    try {
      const result: any = await window.getPlatform();
      return result;
    } catch (error) {
      console.error('Failed to get platform info:', error);
      return null;
    }
  }

  /**
   * Demo: Reverse a string using C++
   */
  async reverseString(text: string): Promise<string> {
    try {
      // Don't JSON.stringify - webview does it automatically
      // Result is already an object, not a string!
      const data: any = await window.reverseString(text);
      console.log('C++ returned:', data);
      return data.reversed;
    } catch (error) {
      console.error('Failed to reverse string:', error);
      return '';
    }
  }

  /**
   * Demo: Get random number from C++
   */
  async getRandomNumber(): Promise<number> {
    try {
      console.log('Calling window.getRandomNumber()...');
      // Result is already an object, not a JSON string!
      const data: any = await window.getRandomNumber();
      console.log('C++ returned (already parsed):', data, 'Type:', typeof data);
      console.log('Number value:', data.number);
      return data.number;
    } catch (error) {
      console.error('Failed to get random number:', error);
      return 0;
    }
  }
}

// Export a singleton instance
export const bridge = new NativeBridge();
