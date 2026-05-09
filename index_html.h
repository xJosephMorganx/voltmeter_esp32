#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Voltmeter</title>
    <link rel="stylesheet" href="/style.css">
  </head>
  <body>
    <main class="app-shell">
      <section class="dashboard" aria-label="ESP32 voltage meter">
        <header class="topbar">
          <div>
            <p class="eyebrow">ESP32 ADC</p>
            <h1>Voltmeter</h1>
          </div>
          <div class="header-actions">
            <label class="theme-toggle" for="themeToggle">
              <span>Dark mode</span>
              <input id="themeToggle" type="checkbox">
              <span class="toggle-track" aria-hidden="true">
                <span class="toggle-thumb"></span>
              </span>
            </label>
            <div class="status" id="connectionStatus" data-state="pending">
              <span class="status-dot"></span>
              <span id="statusText">Connecting</span>
            </div>
          </div>
        </header>
        <section class="gauge-panel">
          <div class="gauge-wrap">
            <svg class="gauge" viewBox="0 0 420 260" role="img" aria-labelledby="gaugeTitle gaugeDesc">
              <title id="gaugeTitle">Voltage gauge</title>
              <desc id="gaugeDesc">A semicircular needle gauge that shows the voltage measured by the ADC.</desc>
              <defs>
                <linearGradient id="arcGradient" x1="0%" x2="100%" y1="0%" y2="0%">
                  <stop offset="0%" stop-color="#1fbf75"></stop>
                  <stop offset="50%" stop-color="#f7c948"></stop>
                  <stop offset="100%" stop-color="#f45d48"></stop>
                </linearGradient>
                <filter id="needleShadow" x="-30%" y="-30%" width="160%" height="160%">
                  <feDropShadow dx="0" dy="7" stdDeviation="6" flood-color="#111827" flood-opacity="0.28"></feDropShadow>
                </filter>
              </defs>
              <path class="arc-bg" d="M 50 210 A 160 160 0 0 1 370 210"></path>
              <path class="arc-value" d="M 50 210 A 160 160 0 0 1 370 210"></path>
              <g id="ticks"></g>
              <g class="needle-group" id="needleGroup">
                <line class="needle" x1="210" y1="210" x2="210" y2="68"></line>
                <circle class="needle-hub-outer" cx="210" cy="210" r="20"></circle>
                <circle class="needle-hub-inner" cx="210" cy="210" r="9"></circle>
              </g>
            </svg>
            <div class="readout">
              <span id="voltageValue">--</span>
              <span class="unit">V</span>
            </div>
          </div>
        </section>
        <section class="controls" aria-label="Meter settings">
          <label class="field">
            <span>Max range</span>
            <select id="rangeSelect">
              <option value="3.3" selected>3.3 V</option>
              <option value="5">5 V</option>
              <option value="12">12 V</option>
              <option value="custom">Custom</option>
            </select>
          </label>
          <label class="field custom-range" id="customRangeField">
            <span>Custom maximum</span>
            <input id="customMax" type="number" min="0.1" step="0.1" value="3.3">
          </label>
          <div class="metric">
            <span>Raw ADC</span>
            <strong id="rawValue">----</strong>
          </div>
        </section>
      </section>
    </main>
    <script src="/script.js"></script>
  </body>
</html>
)rawliteral";
