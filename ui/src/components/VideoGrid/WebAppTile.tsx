import React from 'react';
import { ParticipantLayout } from '../../bridge/video-bridge';
import { ParticipantLabel } from './ParticipantLabel';

interface WebAppTileProps {
  participant: ParticipantLayout;
  gridOffset: { top: number; left: number };
}

export const WebAppTile: React.FC<WebAppTileProps> = ({ participant, gridOffset }) => {
  const { position } = participant;

  // Position exactly where C++ calculated the position
  // Adjust for the grid container offset (dynamically calculated)
  const adjustedLeft = position.x - gridOffset.left;
  const adjustedTop = position.y - gridOffset.top;

  console.log(`Web App Tile ${participant.id}: C++ pos (${position.x}, ${position.y}), offset (${gridOffset.left}, ${gridOffset.top}), adjusted (${adjustedLeft}, ${adjustedTop})`);

  const containerStyle: React.CSSProperties = {
    position: 'absolute',
    left: adjustedLeft,
    top: adjustedTop,
    width: position.width,
    height: position.height,
    pointerEvents: 'auto', // Enable iframe interaction
  };

  return (
    <div className="web-app-tile" style={containerStyle}>
      <iframe
        className="web-app-iframe"
        src={participant.contentUrl}
        title={participant.name}
        sandbox="allow-scripts allow-same-origin"
        allow="fullscreen"
      />
      <ParticipantLabel name={participant.name} />
    </div>
  );
};
