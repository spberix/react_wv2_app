import React, { useState } from 'react';
import { bridge } from '../bridge/bridge';

export const BridgeDemo: React.FC = () => {
  const [inputText, setInputText] = useState('Hello World');
  const [reversedText, setReversedText] = useState('');
  const [randomNumber, setRandomNumber] = useState<number | null>(null);
  const [loading, setLoading] = useState(false);
  const [ipcLatency, setIpcLatency] = useState<number | null>(null);

  const handleReverse = async () => {
    setLoading(true);
    const result = await bridge.reverseString(inputText);
    setReversedText(result);
    setLoading(false);
  };

  const handleRandom = async () => {
    console.log('handleRandom called');
    setLoading(true);
    try {
      const num = await bridge.getRandomNumber();
      console.log('Got number from bridge:', num, 'Type:', typeof num);
      setRandomNumber(num);
      console.log('State should update to:', num);
    } catch (error) {
      console.error('Error in handleRandom:', error);
    } finally {
      setLoading(false);
    }
  };

  const benchmarkIPC = async () => {
    setLoading(true);
    const iterations = 100;
    const start = performance.now();

    for (let i = 0; i < iterations; i++) {
      await bridge.getPlatform();
    }

    const end = performance.now();
    const avgLatency = (end - start) / iterations;
    setIpcLatency(avgLatency);
    setLoading(false);
  };

  return (
    <div className="bridge-demo">
      <h2>🌉 Bridge Communication Demo</h2>
      <p>Test real-time React ↔ C++ communication</p>

      <div className="demo-section">
        <h3>JavaScript → C++ → JavaScript</h3>

        <div className="demo-card">
          <h4>String Reversal</h4>
          <p className="demo-description">
            Type text in React → Send to C++ → C++ reverses it → Return to React
          </p>
          <input
            type="text"
            value={inputText}
            onChange={(e) => setInputText(e.target.value)}
            placeholder="Enter text to reverse"
            className="demo-input"
          />
          <button
            onClick={handleReverse}
            disabled={loading}
            className="demo-button"
          >
            {loading ? 'Processing in C++...' : 'Reverse String (C++)'}
          </button>
          {reversedText && (
            <div className="demo-result">
              <strong>C++ Response:</strong> {reversedText}
            </div>
          )}
        </div>

        <div className="demo-card">
          <h4>Random Number Generator</h4>
          <p className="demo-description">
            Call C++ rand() function from React
          </p>
          <button
            onClick={handleRandom}
            disabled={loading}
            className="demo-button"
          >
            {loading ? 'Calling C++...' : 'Generate Random (C++)'}
          </button>
          {randomNumber !== null && (
            <div className="demo-result">
              <strong>C++ Generated:</strong> {randomNumber}
            </div>
          )}
        </div>

        <div className="demo-card">
          <h4>⚡ IPC Performance Benchmark</h4>
          <p className="demo-description">
            Measure IPC latency by calling C++ 100 times
          </p>
          <button
            onClick={benchmarkIPC}
            disabled={loading}
            className="demo-button"
          >
            {loading ? 'Benchmarking...' : 'Benchmark IPC Latency'}
          </button>
          {ipcLatency !== null && (
            <div className="demo-result">
              <strong>Average IPC Round-Trip:</strong> {ipcLatency.toFixed(2)}ms
              <br />
              <small style={{ color: '#666' }}>
                (100 calls across process boundary)
              </small>
            </div>
          )}
        </div>
      </div>

      <div className="demo-flow">
        <h3>How It Works</h3>
        <div className="flow-steps">
          <div className="flow-step">
            <div className="step-number">1</div>
            <div className="step-content">
              <strong>React calls:</strong><br/>
              <code>bridge.reverseString("Hello")</code>
            </div>
          </div>
          <div className="flow-arrow">→</div>
          <div className="flow-step">
            <div className="step-number">2</div>
            <div className="step-content">
              <strong>Bridge serializes:</strong><br/>
              <code>JSON.stringify(args)</code>
            </div>
          </div>
          <div className="flow-arrow">→</div>
          <div className="flow-step">
            <div className="step-number">3</div>
            <div className="step-content">
              <strong>C++ executes:</strong><br/>
              <code>bindings::reverseString()</code>
            </div>
          </div>
          <div className="flow-arrow">→</div>
          <div className="flow-step">
            <div className="step-number">4</div>
            <div className="step-content">
              <strong>C++ returns:</strong><br/>
              <code>{`{"reversed": "olleH"}`}</code>
            </div>
          </div>
          <div className="flow-arrow">→</div>
          <div className="flow-step">
            <div className="step-number">5</div>
            <div className="step-content">
              <strong>React receives:</strong><br/>
              <code>Parse & display</code>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};
