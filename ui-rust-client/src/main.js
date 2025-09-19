const { invoke } = window.__TAURI__.core;

let greetInputEl;
let greetMsgEl;

let targetServerIP;

let targetCtx;
let hitsCtx;

async function greet() {
  // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
  greetMsgEl.textContent = await invoke("greet", { name: greetInputEl.value });
}

async function get_position() {
  const text_position = await invoke("get_position");
  const coords = text_position.split(" ");
  const x = +coords[0];
  const y = +coords[1];
  drawHit(x, y);
}

async function target_connect() {
  await invoke("target_connect", { host: targetServerIP.value });
}

function drawSportPistolTarget(canvasId = 'targetCanvas') {
  const canvas = document.getElementById(canvasId);
  if (!canvas) throw new Error('Canvas not found: ' + canvasId);
  const ctx = canvas.getContext('2d');

  // Canvas size (assumes canvas.width/height are 1000)
  const W = canvas.width;
  const H = canvas.height;
  const cx = W / 2;
  const cy = H / 2;

  // clear
  ctx.clearRect(0, 0, W, H);

  // background
  ctx.fillStyle = '#fff';
  ctx.fillRect(0, 0, W, H);

  // geometry
  const outerRadius = Math.min(W, H) * 0.45; // 450 for 1000x1000
  const rings = 10;
  const ringWidth = outerRadius / rings; // equal widths

  // draw rings from outer -> inner
  for (let i = rings; i >= 1; i--) {
    const rOuter = ringWidth * i;
    // Coloring: typical sport pistol target has dark (black) rings nearer center.
    // We'll make ring 1..10 such that the inner half (6..10) are dark, outer half lighter.
    // Use subtle radial gradient to look nicer.
    const g = ctx.createRadialGradient(cx, cy, rOuter * 0.2, cx, cy, rOuter);
    if (i >= 6) {
      // inner darker rings
      g.addColorStop(0, '#111');      // center-ish
      g.addColorStop(1, '#000');      // outer edge of this ring
    } else {
      // outer lighter rings (mostly white background with slight gray)
      g.addColorStop(0, '#fff');
      g.addColorStop(1, '#666');      // subtle gray
    }

    // fill circle for ring outside boundary; then we'll punch inner circle to create ring thickness
    ctx.beginPath();
    ctx.arc(cx, cy, rOuter, 0, Math.PI * 2);
    ctx.closePath();
    ctx.fillStyle = g;
    ctx.fill();

    // punch out inner circle (so rings don't stack as full discs)
    const rInner = rOuter - ringWidth;
    if (rInner > 0) {
      ctx.globalCompositeOperation = 'destination-out';
      ctx.beginPath();
      ctx.arc(cx, cy, rInner, 0, Math.PI * 2);
      ctx.fill();
      ctx.closePath();
      ctx.globalCompositeOperation = 'source-over';

      // Draw thin white separator line to emphasize ring edges
      ctx.beginPath();
      ctx.arc(cx, cy, rInner + 0.5, 0, Math.PI * 2);
      ctx.lineWidth = Math.max(1, Math.round(W / 1000 * 2)); // keep separator visible on high-res
      ctx.strokeStyle = '#fff';
      ctx.stroke();
    }
  }

  // golden bullseye for inner 10 (small)
  const bullRadius = Math.max(6, Math.round(ringWidth * 0.28)); // small golden dot
  ctx.beginPath();
  ctx.arc(cx, cy, bullRadius, 0, Math.PI * 2);
  ctx.fillStyle = '#ffd700'; // gold
  ctx.fill();
  ctx.closePath();

  // thin black center tiny dot for contrast
  ctx.beginPath();
  ctx.arc(cx, cy, Math.max(1, Math.round(bullRadius * 0.18)), 0, Math.PI * 2);
  ctx.fillStyle = '#000';
  ctx.fill();
  ctx.closePath();

  // Draw center crosshair (subtle)
  ctx.strokeStyle = 'rgba(0,0,0,0.4)';
  ctx.lineWidth = 1;
  ctx.beginPath();
  ctx.moveTo(cx - outerRadius * 0.06, cy);
  ctx.lineTo(cx + outerRadius * 0.06, cy);
  ctx.moveTo(cx, cy - outerRadius * 0.06);
  ctx.lineTo(cx, cy + outerRadius * 0.06);
  ctx.stroke();

  ctx.font = `${Math.round(W / 30)}px sans-serif`; // adjust font with canvas size
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  for (let i = 1; i <= rings; i++) {
    const score = rings - i + 1;
    const offsetX = ringWidth * (i - 0.5);
    ctx.fillStyle = i < 6 ? "#000000" : "#FFFFFF";
    ctx.fillText(score.toString(), cx - offsetX, cy);
    ctx.fillText(score.toString(), cx + offsetX, cy);
  }

  // Outer black ring stroke for crisp boundary
  ctx.beginPath();
  ctx.arc(cx, cy, outerRadius + 0.5, 0, Math.PI * 2);
  ctx.lineWidth = 3;
  ctx.strokeStyle = '#000';
  ctx.stroke();

}

function drawHit(x, y) {
  hitsCtx.fillStyle = "red";
  hitsCtx.beginPath();
  hitsCtx.arc(x, y, 6, 0, Math.PI * 2);
  hitsCtx.fill();
}

window.addEventListener("DOMContentLoaded", () => {
  greetInputEl = document.querySelector("#greet-input");
  greetMsgEl = document.querySelector("#greet-msg");

  targetServerIP = document.querySelector("#serverAddrInput");

  // targetCtx = document.getElementById("targetCanvas").getContext("2d");
  hitsCtx = document.getElementById("hitsCanvas").getContext("2d");

  drawSportPistolTarget("targetCanvas");

  document.getElementById("addHitBtn").addEventListener("click", () => {
    get_position();
  });

  document.getElementById("connectBtn").addEventListener("click", () => {
    target_connect();
  });


  /*
    document.querySelector("#greet-form").addEventListener("submit", (e) => {
      e.preventDefault();
      get_position();
    });
    */
});
