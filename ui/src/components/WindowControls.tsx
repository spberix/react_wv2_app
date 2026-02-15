import React, { useState, useEffect } from 'react';
import { bridge } from '../bridge/bridge';

interface WindowControlsProps {
  className?: string;
}

export const WindowControls: React.FC<WindowControlsProps> = ({ className }) => {
  const [isMaximized, setIsMaximized] = useState(false);

  useEffect(() => {
    // Check initial maximized state
    checkMaximizedState();
  }, []);

  const checkMaximizedState = async () => {
    const maximized = await bridge.isMaximized();
    setIsMaximized(maximized);
  };

  const handleMinimize = async () => {
    const result = await bridge.minimize();
    if (result.status === 'not_implemented') {
      console.warn('Minimize not implemented on this platform');
    }
  };

  const handleMaximize = async () => {
    const result = await bridge.maximize();
    if (result.status === 'not_implemented') {
      console.warn('Maximize not implemented on this platform');
    }
    // Update state after maximize/restore
    await checkMaximizedState();
  };

  const handleClose = async () => {
    await bridge.close();
  };

  return (
    <div className={`window-controls ${className || ''}`}>
      <button
        className="window-control-btn minimize"
        onClick={handleMinimize}
        title="Minimize"
        aria-label="Minimize window"
      >
        <svg width="12" height="12" viewBox="0 0 12 12">
          <path d="M0 6h12" stroke="currentColor" strokeWidth="1" />
        </svg>
      </button>

      <button
        className="window-control-btn maximize"
        onClick={handleMaximize}
        title={isMaximized ? 'Restore' : 'Maximize'}
        aria-label={isMaximized ? 'Restore window' : 'Maximize window'}
      >
        {isMaximized ? (
          <svg width="12" height="12" viewBox="0 0 12 12">
            <path
              d="M3 3v6h6V3H3z M2 2h8v8H2V2z"
              fill="none"
              stroke="currentColor"
              strokeWidth="1"
            />
          </svg>
        ) : (
          <svg width="12" height="12" viewBox="0 0 12 12">
            <rect
              x="1"
              y="1"
              width="10"
              height="10"
              fill="none"
              stroke="currentColor"
              strokeWidth="1"
            />
          </svg>
        )}
      </button>

      <button
        className="window-control-btn close"
        onClick={handleClose}
        title="Close"
        aria-label="Close window"
      >
        <svg width="12" height="12" viewBox="0 0 12 12">
          <path
            d="M1 1l10 10M11 1L1 11"
            stroke="currentColor"
            strokeWidth="1"
          />
        </svg>
      </button>
    </div>
  );
};
