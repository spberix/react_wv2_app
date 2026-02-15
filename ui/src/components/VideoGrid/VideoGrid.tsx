import React, { useState, useEffect } from 'react';
import { videoBridge, ParticipantLayout, ParticipantType } from '../../bridge/video-bridge';
import { VideoControls } from './VideoControls';
import { VideoTile } from './VideoTile';
import { WebAppTile } from './WebAppTile';
import './VideoGrid.css';

export const VideoGrid: React.FC = () => {
  const [participants, setParticipants] = useState<ParticipantLayout[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const [gridOffset, setGridOffset] = useState({ top: 0, left: 0 });
  const gridRef = React.useRef<HTMLDivElement>(null);

  // Calculate video grid offset dynamically
  useEffect(() => {
    const updateOffset = () => {
      if (gridRef.current) {
        const computedStyle = window.getComputedStyle(gridRef.current);
        const top = parseInt(computedStyle.top) || 0;
        const left = parseInt(computedStyle.left) || 0;
        console.log('Grid offset from computed style:', { top, left });
        setGridOffset({ top, left });
      }
    };

    // Use requestAnimationFrame to ensure DOM is ready
    requestAnimationFrame(() => {
      updateOffset();
    });

    // Also update after a small delay to ensure layout is complete
    setTimeout(updateOffset, 100);

    window.addEventListener('resize', updateOffset);
    return () => window.removeEventListener('resize', updateOffset);
  }, []);

  // Recalculate offset when loading completes
  useEffect(() => {
    if (!isLoading && gridRef.current) {
      const computedStyle = window.getComputedStyle(gridRef.current);
      const top = parseInt(computedStyle.top) || 0;
      const left = parseInt(computedStyle.left) || 0;
      console.log('Grid offset after load:', { top, left });
      setGridOffset({ top, left });
    }
  }, [isLoading]);

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

  const handleAddWebApp = async () => {
    try {
      const name = `Web App ${participants.length + 1}`;
      const url = 'https://microsoft.com';
      const success = await videoBridge.addWebApp(name, url);

      if (!success) {
        console.error('Failed to add web app');
      }
      // No need to manually update state - C++ will push notification
    } catch (err) {
      console.error('Error adding web app:', err);
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
        onAddWebApp={handleAddWebApp}
        onRemoveParticipant={handleRemoveParticipant}
        canAdd={participants.length < 9}
        canRemove={participants.length > 0}
        participantCount={participants.length}
      />
      <div className="video-grid" ref={gridRef}>
        {participants.map(p => {
          if (p.type === ParticipantType.WEB_APP) {
            return <WebAppTile key={p.id} participant={p} gridOffset={gridOffset} />;
          } else {
            return <VideoTile key={p.id} participant={p} gridOffset={gridOffset} />;
          }
        })}
      </div>
    </div>
  );
};
