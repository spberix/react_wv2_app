import React, { useState, useEffect } from 'react';
import { videoBridge, ParticipantLayout } from '../../bridge/video-bridge';
import { VideoControls } from './VideoControls';
import { VideoTile } from './VideoTile';
import './VideoGrid.css';

export const VideoGrid: React.FC = () => {
  const [participants, setParticipants] = useState<ParticipantLayout[]>([]);
  const [isLoading, setIsLoading] = useState(true);

  // Load initial layout from C++ and listen for C++ push notifications
  useEffect(() => {
    // Initial load
    videoBridge.getGridLayout()
      .then(layouts => {
        setParticipants(layouts);
        setIsLoading(false);
      })
      .catch(err => {
        console.error('Failed to load initial grid layout:', err);
        setIsLoading(false);
      });

    // Listen for C++ position updates (resize, add, remove)
    const unsubscribe = videoBridge.onPositionsChanged((updatedParticipants) => {
      console.log('C++ notified position change:', updatedParticipants);
      setParticipants(updatedParticipants);
    });

    return unsubscribe; // Cleanup on unmount
  }, []);

  const handleAddParticipant = async () => {
    try {
      const name = `Participant ${participants.length + 1}`;
      const success = await videoBridge.addParticipant(name);

      if (!success) {
        console.error('Failed to add participant');
      }
      // No need to manually update state - C++ will push notification
    } catch (err) {
      console.error('Error adding participant:', err);
    }
  };

  const handleRemoveParticipant = async () => {
    if (participants.length === 0) return;

    try {
      const lastParticipant = participants[participants.length - 1];
      const success = await videoBridge.removeParticipant(lastParticipant.id);

      if (!success) {
        console.error('Failed to remove participant');
      }
      // No need to manually update state - C++ will push notification
    } catch (err) {
      console.error('Error removing participant:', err);
    }
  };

  if (isLoading) {
    return (
      <div className="video-grid-container">
        <div className="loading">Loading...</div>
      </div>
    );
  }

  return (
    <div className="video-grid-container">
      <VideoControls
        onAddParticipant={handleAddParticipant}
        onRemoveParticipant={handleRemoveParticipant}
        canAdd={participants.length < 9}
        canRemove={participants.length > 0}
        participantCount={participants.length}
      />
      <div className="video-grid">
        {participants.map(p => (
          <VideoTile key={p.id} participant={p} />
        ))}
      </div>
    </div>
  );
};
