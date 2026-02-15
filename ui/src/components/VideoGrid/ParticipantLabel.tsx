import React from 'react';

interface ParticipantLabelProps {
  name: string;
}

export const ParticipantLabel: React.FC<ParticipantLabelProps> = ({ name }) => {
  return (
    <div className="participant-label">
      {name}
    </div>
  );
};
