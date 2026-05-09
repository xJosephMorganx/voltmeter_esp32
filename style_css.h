#pragma once

#include <Arduino.h>

const char STYLE_CSS[] PROGMEM = R"rawliteral(
:root {
  --bg: #eef3f8;
  --panel: #ffffff;
  --panel-soft: #f7fafc;
  --text: #152033;
  --muted: #65758b;
  --line: #dce5ee;
  --green: #1fbf75;
  --yellow: #f7c948;
  --red: #f45d48;
  --blue: #2563eb;
  --arc-bg: #e6edf4;
  --needle: #111827;
  --needle-inner: #ffffff;
  --shadow: 0 28px 70px rgba(22, 37, 62, 0.18);
}

body.theme-dark {
  --bg: #0c111d;
  --panel: #121a2a;
  --panel-soft: #172235;
  --text: #edf3fb;
  --muted: #9aa9bd;
  --line: #263348;
  --blue: #70a5ff;
  --arc-bg: #273449;
  --needle: #f8fafc;
  --needle-inner: #111827;
  --shadow: 0 28px 70px rgba(0, 0, 0, 0.38);
}

* {
  box-sizing: border-box;
}

body {
  min-height: 100vh;
  margin: 0;
  color: var(--text);
  font-family: Inter, ui-sans-serif, system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
  background:
    linear-gradient(135deg, rgba(31, 191, 117, 0.14), transparent 34%),
    linear-gradient(315deg, rgba(244, 93, 72, 0.12), transparent 32%),
    var(--bg);
}

body.theme-dark {
  background:
    linear-gradient(135deg, rgba(31, 191, 117, 0.16), transparent 34%),
    linear-gradient(315deg, rgba(244, 93, 72, 0.12), transparent 32%),
    var(--bg);
}

.app-shell {
  min-height: 100vh;
  display: grid;
  place-items: center;
  padding: 28px;
}

.dashboard {
  width: min(920px, 100%);
  padding: 28px;
  border: 1px solid rgba(220, 229, 238, 0.9);
  border-radius: 8px;
  background: rgba(255, 255, 255, 0.92);
  box-shadow: var(--shadow);
}

body.theme-dark .dashboard {
  border-color: rgba(38, 51, 72, 0.9);
  background: rgba(18, 26, 42, 0.94);
}

.topbar,
.controls {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 18px;
}

.eyebrow {
  margin: 0 0 4px;
  color: var(--blue);
  font-size: 0.78rem;
  font-weight: 800;
  letter-spacing: 0;
  text-transform: uppercase;
}

h1 {
  margin: 0;
  font-size: clamp(2rem, 6vw, 4rem);
  line-height: 0.95;
}

.header-actions {
  display: flex;
  align-items: center;
  gap: 12px;
}

.theme-toggle {
  display: inline-flex;
  align-items: center;
  gap: 10px;
  color: var(--muted);
  font-weight: 800;
  white-space: nowrap;
}

.theme-toggle input {
  position: absolute;
  width: 1px;
  height: 1px;
  min-height: 0;
  padding: 0;
  opacity: 0;
  pointer-events: none;
}

.toggle-track {
  width: 52px;
  height: 30px;
  padding: 3px;
  border: 1px solid var(--line);
  border-radius: 999px;
  background: var(--panel-soft);
  transition: background 180ms ease, border-color 180ms ease;
}

.toggle-thumb {
  display: block;
  width: 22px;
  height: 22px;
  border-radius: 999px;
  background: var(--blue);
  box-shadow: 0 5px 12px rgba(37, 99, 235, 0.26);
  transition: transform 180ms ease;
}

.theme-toggle input:checked + .toggle-track {
  border-color: rgba(112, 165, 255, 0.55);
  background: #223049;
}

.theme-toggle input:checked + .toggle-track .toggle-thumb {
  transform: translateX(22px);
}

.theme-toggle input:focus-visible + .toggle-track {
  outline: 3px solid rgba(37, 99, 235, 0.22);
}

.status {
  display: inline-flex;
  align-items: center;
  gap: 10px;
  min-width: 150px;
  justify-content: center;
  padding: 10px 14px;
  border: 1px solid var(--line);
  border-radius: 999px;
  color: var(--muted);
  background: var(--panel-soft);
  font-weight: 700;
}

.status-dot {
  width: 11px;
  height: 11px;
  border-radius: 999px;
  background: var(--yellow);
  box-shadow: 0 0 0 5px rgba(247, 201, 72, 0.18);
}

.status[data-state="online"] .status-dot {
  background: var(--green);
  box-shadow: 0 0 0 5px rgba(31, 191, 117, 0.18);
}

.status[data-state="offline"] .status-dot {
  background: var(--red);
  box-shadow: 0 0 0 5px rgba(244, 93, 72, 0.18);
}

.gauge-panel {
  margin: 18px 0 22px;
  padding: 26px 26px 34px;
  border: 1px solid var(--line);
  border-radius: 8px;
  background: linear-gradient(180deg, var(--panel), var(--panel-soft));
}

.gauge-wrap {
  position: relative;
  width: min(660px, 100%);
  margin: 0 auto;
  padding-bottom: 58px;
}

.gauge {
  display: block;
  width: 100%;
  height: auto;
  overflow: visible;
}

.arc-bg,
.arc-value {
  fill: none;
  stroke-linecap: round;
  stroke-width: 26;
}

.arc-bg {
  stroke: var(--arc-bg);
}

.arc-value {
  stroke: url("#arcGradient");
  stroke-dasharray: 502.65;
  stroke-dashoffset: 502.65;
  transition: stroke-dashoffset 450ms ease;
}

.tick {
  stroke: #8797aa;
  stroke-linecap: round;
  stroke-width: 3;
}

.tick.major {
  stroke: #344256;
  stroke-width: 5;
}

body.theme-dark .tick {
  stroke: #71839c;
}

body.theme-dark .tick.major {
  stroke: #c4d3e7;
}

.tick-label {
  fill: var(--muted);
  font-size: 15px;
  font-weight: 800;
  text-anchor: middle;
  dominant-baseline: middle;
}

.needle-group {
  filter: url("#needleShadow");
}

.needle {
  stroke: var(--needle);
  stroke-width: 8;
  stroke-linecap: round;
}

.needle-hub-outer {
  fill: var(--needle);
}

.needle-hub-inner {
  fill: var(--needle-inner);
}

.readout {
  position: absolute;
  left: 50%;
  bottom: 0;
  display: flex;
  align-items: baseline;
  gap: 6px;
  transform: translateX(-50%);
  font-weight: 900;
}

#voltageValue {
  min-width: 4ch;
  font-variant-numeric: tabular-nums;
  font-size: clamp(1.8rem, 6vw, 3.2rem);
  line-height: 1;
}

.unit {
  color: var(--muted);
  font-size: clamp(0.95rem, 2.7vw, 1.35rem);
}

.controls {
  flex-wrap: wrap;
}

.field,
.metric {
  display: grid;
  gap: 8px;
  min-width: 170px;
}

.field span,
.metric span {
  color: var(--muted);
  font-size: 0.82rem;
  font-weight: 800;
}

select,
input {
  min-height: 46px;
  border: 1px solid var(--line);
  border-radius: 8px;
  padding: 0 12px;
  color: var(--text);
  background: var(--panel);
  font: inherit;
  font-weight: 800;
}

select:focus,
input:focus {
  outline: 3px solid rgba(37, 99, 235, 0.18);
  border-color: var(--blue);
}

.custom-range {
  display: none;
}

.custom-range.is-visible {
  display: grid;
}

.metric {
  justify-items: end;
  margin-left: auto;
}

.metric strong {
  font-size: 1.6rem;
  font-variant-numeric: tabular-nums;
}

@media (max-width: 680px) {
  .app-shell {
    align-items: start;
    padding: 14px;
  }

  .dashboard {
    padding: 18px;
  }

  .topbar {
    align-items: flex-start;
    flex-direction: column;
  }

  .header-actions {
    width: 100%;
    align-items: stretch;
    flex-direction: column;
  }

  .theme-toggle,
  .status {
    width: 100%;
    justify-content: space-between;
  }

  .gauge-panel {
    padding: 14px 14px 26px;
  }

  .controls,
  .field,
  .metric {
    width: 100%;
  }

  .metric {
    justify-items: start;
    margin-left: 0;
  }
}
)rawliteral";
