import React from 'react';
import { ParticipantLayout } from '../../bridge/video-bridge';
import { ParticipantLabel } from './ParticipantLabel';

interface VideoTileProps {
  participant: ParticipantLayout;
  gridOffset: { top: number; left: number };
}

export const VideoTile: React.FC<VideoTileProps> = ({ participant, gridOffset }) => {
  const { position } = participant;

  // Position exactly where C++ rendered the video
  // Adjust for the grid container offset (dynamically calculated)
  const adjustedLeft = position.x - gridOffset.left;
  const adjustedTop = position.y - gridOffset.top;

  console.log(`Tile ${participant.id}: C++ pos (${position.x}, ${position.y}), offset (${gridOffset.left}, ${gridOffset.top}), adjusted (${adjustedLeft}, ${adjustedTop})`);

  const style: React.CSSProperties = {
    position: 'absolute',
    left: adjustedLeft,
    top: adjustedTop,
    width: position.width,
    height: position.height,
    pointerEvents: 'none',
  };

  return (
    <div className="video-tile" style={style}>
      <ParticipantLabel name={participant.name} />
    </div>
  );
};
