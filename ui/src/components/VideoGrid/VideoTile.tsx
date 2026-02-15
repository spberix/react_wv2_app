import React from 'react';
import { ParticipantLayout } from '../../bridge/video-bridge';
import { ParticipantLabel } from './ParticipantLabel';

interface VideoTileProps {
  participant: ParticipantLayout;
}

export const VideoTile: React.FC<VideoTileProps> = ({ participant }) => {
  const { position } = participant;

  // Position exactly where C++ rendered the video
  const style: React.CSSProperties = {
    position: 'absolute',
    left: position.x,
    top: position.y,
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
