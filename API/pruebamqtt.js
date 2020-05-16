var mqtt = require('mqtt');
var client  = mqtt.connect('mqtt://35.208.61.217');
client.on('connect', function () {
setInterval(function() {
client.publish('myTopic', 'Hello Matius');
console.log('Message Sent');
}, 5000);
});
