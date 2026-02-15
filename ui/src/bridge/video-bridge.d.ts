/**
 * Video Bridge - TypeScript interface for C++ video functionality
 */
export declare enum ParticipantType {
    VIDEO = "video",
    WEB_APP = "web_app"
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
declare global {
    interface Window {
        videoAddParticipant: (args: string) => Promise<string>;
        videoAddWebApp: (args: string) => Promise<string>;
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
     * Add a web app participant
     */
    addWebApp(name: string, url: string): Promise<boolean>;
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