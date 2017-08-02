const express = require('express')
const app = express()
const mqtt = require('mqtt') 


var broker = "m21.cloudmqtt.com";
var username = "ppamouil"
var passwrd = "DWpVcuXoiQTS"

var url = "mqtt://" + broker;
 url.port=15997

var options = {
  port: url.port,
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  username: username,
  password: passwrd,
};

const client = mqtt.connect(url, options)  

app.use('/css', express.static(__dirname + '/node_modules/bootstrap/dist/'));
app.use(express.static('public'))
// app.get('/', function (req, res) {
//   res.send()
// })

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})