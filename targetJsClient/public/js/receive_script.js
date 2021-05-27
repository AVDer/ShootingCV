
const SERVER_PORT = '8000'
const SERVER_ADDRESS = 'localhost'
const SERVER_URL = 'ws'

var targetCanvas = document.getElementById("targetCanvas");
var targetContext = targetCanvas.getContext("2d");

var client = new WebSocket('wss://' + SERVER_ADDRESS + ':' + SERVER_PORT + '/' + SERVER_URL);

client.onerror = function(err) {
    console.error('WebSocket error observed', err);
}

client.onopen = function(event) {
    console.log('WebSocket opened!')
}

client.onclose = function(event) {
    console.log(event);
}

drawTarget = () => {
    targetCanvas.width = targetCanvas.offsetWidth;
    targetCanvas.height = targetCanvas.width;

    const radius = targetCanvas.width / 22;
    const centerX = targetCanvas.width / 2;
    const centerY = targetCanvas.height / 2;
    targetContext.beginPath();
    targetContext.arc(centerX, centerY, radius * 4, 0, 2 * Math.PI);
    targetContext.fill();
    targetContext.strokeStyle = 'white';
    targetContext.fillStyle = 'white';
    for (i = 10; i >= 7; --i) {
        targetContext.beginPath();
        targetContext.arc(centerX, centerY, radius * (10 - i + 1), 0, 2 * Math.PI);
        targetContext.stroke();
        targetContext.fillText(i, centerX + radius * (10 - i + 0.5), centerY)
    }
    targetContext.strokeStyle = 'black';
    targetContext.fillStyle = 'black';
    for (i = 6; i >= 1; --i) {
        targetContext.beginPath();
        targetContext.arc(centerX, centerY, radius * (10 - i + 1), 0, 2 * Math.PI);
        targetContext.stroke();
        targetContext.fillText(i, centerX + radius * (10 - i + 0.5), centerY)
    }
}

drawPoint = (coord) => {
    const radius = targetCanvas.width / 22;
    const centerX = targetCanvas.width / 2;
    const centerY = targetCanvas.height / 2;
    targetContext.fillStyle = 'red';
    targetContext.beginPath();
    targetContext.arc(centerX + coord['r'] * radius* Math.cos(coord['t']), centerY + coord['r'] * radius * Math.sin(coord['t']), 5, 0, 2 * Math.PI);
    targetContext.fill();
} 

client.onmessage = function(event) {
    //console.log(event.data)
    data = JSON.parse(event.data);
    document.getElementById("data_text_r").value = data['r'];
    document.getElementById("data_text_t").value = data['t'];

    targetContext.clearRect(0, 0, targetCanvas.width, targetCanvas.height);
    drawTarget()
    drawPoint(data)
}
