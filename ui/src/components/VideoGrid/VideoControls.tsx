import React from 'react';

interface VideoControlsProps {
  onAddParticipant: () => void;
  onRemoveParticipant: () => void;
  canAdd: boolean;
  canRemove: boolean;
  participantCount: number;
}

export const VideoControls: React.FC<VideoControlsProps> = ({
  onAddParticipant,
  onRemoveParticipant,
  canAdd,
  canRemove,
  participantCount,
}) => {
  return (
    <div className="video-controls">
      <div className="controls-left">
        <h1 className="app-title">Video Grid</h1>
        <span className="participant-count">
          {participantCount} / 9 participants
        </span>
      </div>
      <div className="controls-right">
        <button
          className="control-button add-button"
          onClick={onAddParticipant}
          disabled={!canAdd}
          title={canAdd ? 'Add Participant' : 'Maximum capacity reached (9)'}
        >
          + Add Participant
        </button>
        <button
          className="control-button remove-button"
          onClick={onRemoveParticipant}
          disabled={!canRemove}
          title={canRemove ? 'Remove Participant' : 'No participants to remove'}
        >
          - Remove Participant
        </button>
      </div>
    </div>
  );
};
