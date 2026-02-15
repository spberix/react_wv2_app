/**
 * Video Bridge - TypeScript interface for C++ video functionality
 */

export enum ParticipantType {
  VIDEO = 'video',
  WEB_APP = 'web_app',
}

export interface VideoPosition {
  x: number;
  y: number;
  width: number;
  height: number;
}

export interface ParticipantLayout {
  id: number;
  name: string;
  type: ParticipantType;
  contentUrl: string;
  position: VideoPosition;
}

interface VideoGridLayoutResponse {
  participants: ParticipantLayout[];
}

interface AddParticipantResponse {
  success: boolean;
  participantId?: number;
  error?: string;
}

interface RemoveParticipantResponse {
  success: boolean;
  error?: string;
}

interface SelectVideoFileResponse {
  success: boolean;
  path?: string;
  error?: string;
}

// Extend Window interface to include video bindings
declare global {
  interface Window {
    videoAddParticipant: (args: string) => Promise<string>;
    videoAddWebApp: (args: string) => Promise<string>;
    videoRemoveParticipant: (args: string) => Promise<string>;
    videoGetGridLayout: (args: string) => Promise<string>;
    videoSelectFile: (args: string) => Promise<string>;
  }
}

class VideoBridge {
  /**
   * Add a participant (video is auto-selected from pre-generated pool)
   */
  async addParticipant(name: string): Promise<boolean> {
    try {
      const args = JSON.stringify({ name });
      const result = await window.videoAddParticipant(args);
      const response: AddParticipantResponse = JSON.parse(result);

      if (response.success) {
        console.log('Participant added:', response.participantId);
        return true;
      } else {
        console.error('Failed to add participant:', response.error);
        return false;
      }
    } catch (error) {
      console.error('Error in addParticipant:', error);
      return false;
    }
  }

  /**
   * Add a web app participant
   */
  async addWebApp(name: string, url: string): Promise<boolean> {
    try {
      const args = JSON.stringify({ name, url });
      const result = await window.videoAddWebApp(args);
      const response: AddParticipantResponse = JSON.parse(result);

      if (response.success) {
        console.log('Web app added:', response.participantId);
        return true;
      } else {
        console.error('Failed to add web app:', response.error);
        return false;
      }
    } catch (error) {
      console.error('Error in addWebApp:', error);
      return false;
    }
  }

  /**
   * Remove a participant by ID
   */
  async removeParticipant(participantId: number): Promise<boolean> {
    try {
      const args = JSON.stringify({ participantId });
      const result = await window.videoRemoveParticipant(args);
      const response: RemoveParticipantResponse = JSON.parse(result);

      if (response.success) {
        console.log('Participant removed:', participantId);
        return true;
      } else {
        console.error('Failed to remove participant:', response.error);
        return false;
      }
    } catch (error) {
      console.error('Error in removeParticipant:', error);
      return false;
    }
  }

  /**
   * Get the current grid layout from C++
   */
  async getGridLayout(): Promise<ParticipantLayout[]> {
    try {
      const result = await window.videoGetGridLayout('{}');
      const response: VideoGridLayoutResponse = JSON.parse(result);
      return response.participants;
    } catch (error) {
      console.error('Error in getGridLayout:', error);
      return [];
    }
  }

  /**
   * Open native file picker to select a video file
   */
  async selectVideoFile(): Promise<string | null> {
    try {
      const result = await window.videoSelectFile('{}');
      const response: SelectVideoFileResponse = JSON.parse(result);

      if (response.success && response.path) {
        return response.path;
      } else {
        console.log('Video file selection cancelled or failed:', response.error);
        return null;
      }
    } catch (error) {
      console.error('Error in selectVideoFile:', error);
      return null;
    }
  }

  /**
   * Listen for position changes pushed from C++ (resize, add, remove events)
   * Returns an unsubscribe function
   */
  onPositionsChanged(callback: (participants: ParticipantLayout[]) => void): () => void {
    const handler = (event: Event) => {
      const customEvent = event as CustomEvent;
      if (customEvent.detail && customEvent.detail.participants) {
        callback(customEvent.detail.participants);
      }
    };

    window.addEventListener('videoPositionsChanged', handler);

    // Return cleanup function
    return () => {
      window.removeEventListener('videoPositionsChanged', handler);
    };
  }
}

// Export singleton instance
export const videoBridge = new VideoBridge();
