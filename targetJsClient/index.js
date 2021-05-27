const express = require('express');
const fs = require('fs');
const https = require('https');
const path = require('path');

var privateKey  = fs.readFileSync('ssl/localhost.key', 'utf8');
var certificate = fs.readFileSync('ssl/localhost.crt', 'utf8');
var credentials = {key: privateKey, cert: certificate};


const app = express();
var httpsServer = https.createServer(credentials, app);

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(`${__dirname}/public/html/index.html`);
});

/*
app.listen(3333, () => {
    console.log('Application listening on port 3333!');
});
*/

httpsServer.listen(3333);