const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const express = require('express');
const http = require('http');
const socketIO = require('socket.io');

const portName = '/dev/cu.usbmodem14201'; // Replace with your COM port name
const baudRate = 9600;

const app = express();
const server = http.createServer(app);
const io = socketIO(server);

const port = new SerialPort(portName, { baudRate });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

const MAX_POINTS = 250; // Adjust as needed
const ringBuffer = [];

parser.on('data', (data) => {
  const [angleStr, distanceStr] = data.split('@');
  const angle = parseFloat(angleStr);
  const distance = parseFloat(distanceStr);

  if (!isNaN(angle) && !isNaN(distance)) {
    ringBuffer.push({ angle, distance, timestamp: Date.now() });

    // Emit data over WebSocket
    io.emit('newPoint', { angle, distance });
  }
});

// Ring buffer cleanup
setInterval(() => {
  const oneSecondAgo = Date.now() - 1000;
  while (ringBuffer.length > 0 && ringBuffer[0].timestamp < oneSecondAgo) {
    ringBuffer.shift(); // Remove oldest element
  }
}, 100); // Check every 100 milliseconds

app.use(express.static('public')); // Serve the webpage files from 'public' folder

server.listen(3000, () => console.log('Server listening on port 3000'));
