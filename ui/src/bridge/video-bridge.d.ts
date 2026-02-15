/**
 * Video Bridge - TypeScript interface for C++ video functionality
 */
export interface VideoPosition {
    x: number;
    y: number;
    width: number;
    height: number;
}
export interface ParticipantLayout {
    id: number;
    name: string;
    position: VideoPosition;
}
declare global {
    interface Window {
        videoAddParticipant: (args: string) => Promise<string>;
        videoRemoveParticipant: (args: string) => Promise<string>;
        videoGetGridLayout: (args: string) => Promise<string>;
        videoSelectFile: (args: string) => Promise<string>;
    }
}
declare class VideoBridge {
    /**
     * Add a participant (video is auto-selected from pre-generated pool)
     */
    addParticipant(name: string): Promise<boolean>;
    /**
     * Remove a participant by ID
     */
    removeParticipant(participantId: number): Promise<boolean>;
    /**
     * Get the current grid layout from C++
     */
    getGridLayout(): Promise<ParticipantLayout[]>;
    /**
     * Open native file picker to select a video file
     */
    selectVideoFile(): Promise<string | null>;
    /**
     * Listen for position changes pushed from C++ (resize, add, remove events)
     * Returns an unsubscribe function
     */
    onPositionsChanged(callback: (participants: ParticipantLayout[]) => void): () => void;
}
export declare const videoBridge: VideoBridge;
export {};
//# sourceMappingURL=video-bridge.d.ts.map