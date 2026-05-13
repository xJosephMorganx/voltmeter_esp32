const MIN_ANGLE = -90;
const MAX_ANGLE = 90;
const ARC_LENGTH = 502.65;
const POLL_MS = 700;
const THEME_STORAGE_KEY = "esp32-voltmeter-mockup-theme";

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

  return "dark";
}

function applyTheme(theme) {
  const isDark = theme === "dark";

  document.body.classList.toggle("theme-dark", isDark);
  document.body.classList.toggle("theme-light", !isDark);
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
  renderLcdValue(voltageValue, voltage.toFixed(2));
  renderLcdValue(rawValue, raw === null || raw === undefined ? "sim" : String(raw).padStart(4, "0"));
}

function makeTick(angle, innerRadius, outerRadius, className) {
  const centerX = 210;
  const centerY = 210;
  const radians = (angle * Math.PI) / 180;
  const outerX = centerX + Math.cos(radians) * outerRadius;
  const outerY = centerY + Math.sin(radians) * outerRadius;
  const innerX = centerX + Math.cos(radians) * innerRadius;
  const innerY = centerY + Math.sin(radians) * innerRadius;
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
  const radius = 129;
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

  for (let index = 0; index <= 40; index += 1) {
    const percent = index / 40;
    const angle = 180 + percent * 180;
    const isMajor = index % 10 === 0;
    const isMedium = index % 5 === 0;

    if (isMajor) {
      ticksGroup.appendChild(makeTick(angle, 146, 171, "tick major"));
    } else {
      ticksGroup.appendChild(makeTick(angle, isMedium ? 145 : 149, 156, "tick"));
    }

    if (isMajor) {
      const labelValue = maxVoltage * percent;
      ticksGroup.appendChild(makeLabel(angle, labelValue.toFixed(labelValue >= 10 ? 0 : 1)));
    }
  }
}

function createSegmentDigit(character) {
  const digit = document.createElement("span");
  const segmentMap = {
    "0": "abcdef",
    "1": "bc",
    "2": "abged",
    "3": "abgcd",
    "4": "fgbc",
    "5": "afgcd",
    "6": "afgecd",
    "7": "abc",
    "8": "abcdefg",
    "9": "abfgcd"
  };
  const activeSegments = segmentMap[character] || "";

  digit.className = "lcd-digit";
  digit.setAttribute("aria-hidden", "true");

  for (const segment of ["a", "b", "c", "d", "e", "f", "g"]) {
    const segmentElement = document.createElement("span");
    segmentElement.className = `lcd-seg lcd-seg-${segment}${activeSegments.includes(segment) ? " is-on" : ""}`;
    digit.appendChild(segmentElement);
  }

  return digit;
}

function renderLcdValue(element, value) {
  const text = String(value);

  element.textContent = "";
  element.classList.add("lcd-display");
  element.setAttribute("aria-label", text);

  for (const character of text) {
    if (character >= "0" && character <= "9") {
      element.appendChild(createSegmentDigit(character));
    } else if (character === ".") {
      const dot = document.createElement("span");
      dot.className = "lcd-dot";
      dot.setAttribute("aria-hidden", "true");
      element.appendChild(dot);
    } else if (character === "-") {
      const minus = document.createElement("span");
      minus.className = "lcd-minus";
      minus.setAttribute("aria-hidden", "true");
      element.appendChild(minus);
    } else {
      const fallback = document.createElement("span");
      fallback.className = "lcd-text";
      fallback.textContent = character;
      element.appendChild(fallback);
    }
  }
}

function calculateRawFromVoltage(voltage) {
  return Math.round(clamp(voltage / 3.3, 0, 1) * 4095);
}

function getSimulatedVoltage() {
  simulatedPhase += 0.11;
  const maxVoltage = getMaxVoltage();
  const wave = Math.sin(simulatedPhase) * 0.5 + 0.5;
  const drift = Math.sin(simulatedPhase * 0.27) * 0.08;
  return clamp((wave * 0.86 + 0.07 + drift) * maxVoltage, 0, maxVoltage);
}

function fetchVoltage() {
  const voltage = getSimulatedVoltage();
  const raw = calculateRawFromVoltage(voltage);

  setStatus("online", "Online");
  updateGauge(voltage, raw);
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
