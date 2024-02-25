  const io = require('socket.io');
  const socket = io();
  const radarContainer = document.querySelector('.radar-container');

  socket.on('newPoint', (data) => {
    const div = document.createElement('div');
    div.classList.add('radar-point');

    // Polar to Cartesian conversion
    const radius = data.distance;
    const x = radius * Math.cos(data.angle * Math.PI / 180);
    const y = radius * Math.sin(data.angle * Math.PI / 180);

    // Center based on container size
    const offsetX = radarContainer.offsetWidth / 2;
    const offsetY = radarContainer.offsetHeight / 2;

    div.style.left = (offsetX + x) + 'px';
    div.style.top = (offsetY + y) + 'px';

    radarContainer.appendChild(div);

    // Remove old points
    setTimeout(() => {
      div.remove();
    }, 1000);
  });
