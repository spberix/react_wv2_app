import React from 'react';
import { WindowControls } from './WindowControls';

interface TitleBarProps {
  title?: string;
}

export const TitleBar: React.FC<TitleBarProps> = ({ title = 'Teams BIC Sandbox' }) => {
  return (
    <div className="title-bar">
      <div className="title-bar-content">
        <div className="title-bar-icon">
          <svg width="20" height="20" viewBox="0 0 20 20" fill="currentColor">
            <circle cx="10" cy="10" r="8" fill="none" stroke="currentColor" strokeWidth="2" />
            <path d="M10 6v8M6 10h8" stroke="currentColor" strokeWidth="2" />
          </svg>
        </div>
        <div className="title-bar-text">{title}</div>
      </div>
      <WindowControls />
    </div>
  );
};
