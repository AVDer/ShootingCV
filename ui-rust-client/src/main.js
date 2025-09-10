const { invoke } = window.__TAURI__.core;

let greetInputEl;
let greetMsgEl;

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

function drawTarget() {
  const cx = 200, cy = 200;
  for (let r = 200; r > 0; r -= 40) {
    targetCtx.fillStyle = r % 80 === 0 ? "#fff" : "#000";
    targetCtx.beginPath();
    targetCtx.arc(cx, cy, r, 0, Math.PI * 2);
    targetCtx.fill();
  }
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

  targetCtx = document.getElementById("targetCanvas").getContext("2d");
  hitsCtx = document.getElementById("hitsCanvas").getContext("2d");

  drawTarget();

  document.getElementById("addHitBtn").addEventListener("click", () => {
    get_position();
  });
  /*
    document.querySelector("#greet-form").addEventListener("submit", (e) => {
      e.preventDefault();
      get_position();
    });
    */
});
