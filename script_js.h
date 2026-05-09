#pragma once

#include <Arduino.h>

const char SCRIPT_JS[] PROGMEM = R"rawliteral(
const API_URL = "/api/voltage";
const MIN_ANGLE = -90;
const MAX_ANGLE = 90;
const ARC_LENGTH = 502.65;
const POLL_MS = 700;
const THEME_STORAGE_KEY = "esp32-voltmeter-theme";

const voltageValue = document.querySelector("#voltageValue");
const rawValue = document.querySelector("#rawValue");
const needleGroup = document.querySelector("#needleGroup");
const arcValue = document.querySelector(".arc-value");
const ticksGroup = document.querySelector("#ticks");
const rangeSelect = document.querySelector("#rangeSelect");
const customMax = document.querySelector("#customMax");
const customRangeField = document.querySelector("#customRangeField");
const connectionStatus = document.querySelector("#connectionStatus");
const statusText = document.querySelector("#statusText");
const themeToggle = document.querySelector("#themeToggle");

let simulatedPhase = 0;
let currentNeedleAngle = MIN_ANGLE;
let needleAnimationFrame = null;

function clamp(value, min, max) {
  return Math.min(Math.max(value, min), max);
}

function getPreferredTheme() {
  const savedTheme = window.localStorage.getItem(THEME_STORAGE_KEY);

  if (savedTheme === "dark" || savedTheme === "light") {
    return savedTheme;
  }

  return window.matchMedia("(prefers-color-scheme: dark)").matches ? "dark" : "light";
}

function applyTheme(theme) {
  const isDark = theme === "dark";

  document.body.classList.toggle("theme-dark", isDark);
  themeToggle.checked = isDark;
  window.localStorage.setItem(THEME_STORAGE_KEY, theme);
}

function getMaxVoltage() {
  if (rangeSelect.value === "custom") {
    const custom = Number.parseFloat(customMax.value);
    return Number.isFinite(custom) && custom > 0 ? custom : 3.3;
  }

  return Number.parseFloat(rangeSelect.value);
}

function setStatus(state, label) {
  connectionStatus.dataset.state = state;
  statusText.textContent = label;
}

function animateNeedle(targetAngle) {
  const startAngle = currentNeedleAngle;
  const startTime = performance.now();
  const duration = 450;

  if (needleAnimationFrame) {
    window.cancelAnimationFrame(needleAnimationFrame);
  }

  function step(now) {
    const progress = clamp((now - startTime) / duration, 0, 1);
    const eased = 1 - Math.pow(1 - progress, 3);
    const angle = startAngle + (targetAngle - startAngle) * eased;

    needleGroup.setAttribute("transform", `rotate(${angle} 210 210)`);
    currentNeedleAngle = angle;

    if (progress < 1) {
      needleAnimationFrame = window.requestAnimationFrame(step);
    }
  }

  needleAnimationFrame = window.requestAnimationFrame(step);
}

function updateGauge(voltage, raw) {
  const maxVoltage = getMaxVoltage();
  const percent = clamp(voltage / maxVoltage, 0, 1);
  const angle = MIN_ANGLE + percent * (MAX_ANGLE - MIN_ANGLE);

  animateNeedle(angle);
  arcValue.style.strokeDashoffset = String(ARC_LENGTH * (1 - percent));
  voltageValue.textContent = voltage.toFixed(2);
  rawValue.textContent = raw === null || raw === undefined ? "sim" : String(raw);
}

function makeTick(angle, length, className) {
  const centerX = 210;
  const centerY = 210;
  const radius = 160;
  const halfLength = length / 2;
  const radians = (angle * Math.PI) / 180;
  const outerX = centerX + Math.cos(radians) * (radius + halfLength);
  const outerY = centerY + Math.sin(radians) * (radius + halfLength);
  const innerX = centerX + Math.cos(radians) * (radius - halfLength);
  const innerY = centerY + Math.sin(radians) * (radius - halfLength);
  const line = document.createElementNS("http://www.w3.org/2000/svg", "line");

  line.setAttribute("x1", innerX.toFixed(2));
  line.setAttribute("y1", innerY.toFixed(2));
  line.setAttribute("x2", outerX.toFixed(2));
  line.setAttribute("y2", outerY.toFixed(2));
  line.setAttribute("class", className);
  return line;
}

function makeLabel(angle, text) {
  const centerX = 210;
  const centerY = 210;
  const radius = 115;
  const radians = (angle * Math.PI) / 180;
  const label = document.createElementNS("http://www.w3.org/2000/svg", "text");

  label.setAttribute("x", (centerX + Math.cos(radians) * radius).toFixed(2));
  label.setAttribute("y", (centerY + Math.sin(radians) * radius).toFixed(2));
  label.setAttribute("class", "tick-label");
  label.textContent = text;
  return label;
}

function renderTicks() {
  ticksGroup.replaceChildren();
  const maxVoltage = getMaxVoltage();

  for (let index = 0; index <= 10; index += 1) {
    const percent = index / 10;
    const angle = 180 + percent * 180;
    const isMajor = index % 2 === 0;

    ticksGroup.appendChild(makeTick(angle, isMajor ? 30 : 20, isMajor ? "tick major" : "tick"));

    if (isMajor) {
      const labelValue = maxVoltage * percent;
      ticksGroup.appendChild(makeLabel(angle, labelValue.toFixed(labelValue >= 10 ? 0 : 1)));
    }
  }
}

function simulateVoltage() {
  simulatedPhase += 0.12;
  const maxVoltage = getMaxVoltage();
  return (Math.sin(simulatedPhase) * 0.5 + 0.5) * maxVoltage;
}

async function fetchVoltage() {
  try {
    const response = await fetch(API_URL, { cache: "no-store" });

    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const data = await response.json();
    const voltage = Number(data.voltage);

    if (!Number.isFinite(voltage)) {
      throw new Error("Invalid reading");
    }

    setStatus("online", "Online");
    updateGauge(voltage, data.raw);
  } catch (error) {
    const simulated = simulateVoltage();
    setStatus("offline", "Simulation");
    updateGauge(simulated, null);
  }
}

themeToggle.addEventListener("change", () => {
  applyTheme(themeToggle.checked ? "dark" : "light");
});

rangeSelect.addEventListener("change", () => {
  customRangeField.classList.toggle("is-visible", rangeSelect.value === "custom");
  renderTicks();
});

customMax.addEventListener("input", renderTicks);

applyTheme(getPreferredTheme());
renderTicks();
updateGauge(0, 0);
fetchVoltage();
window.setInterval(fetchVoltage, POLL_MS);
)rawliteral";
