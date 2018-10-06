const net = require('net');
const fs = require('fs');

const host = process.argv[2];
const port = process.argv[3];

const socket = net.createConnection(port, host);
const message = {
  size: 0,
  buffer: new Buffer(0)
};


function handle_data(data) {
  console.log('RECEIVED BYTES', data.length);
  fs.writeFileSync('machine.bin', data);
}

function process_chunk(data) {
  let offset = 0;

  if (!message.size)
  {
    offset = 2;
    message.size = data.readUIntLE(0, offset);
    message.buffer = new Buffer(0);
  }

  const remaining = message.size - message.buffer.length;

  message.buffer = Buffer.concat([
    message.buffer,
    data.slice(offset, offset + remaining)
  ]);

  if (message.buffer.length === message.size)
  {
    handle_data(message.buffer);
    message.size = 0;
  }

  if (offset + remaining < data.length)
  {
    process_chunk(data.slice(offset + remaining, data.length));
  }
}


socket.on('data', (data) => {
  process_chunk(data, message);
});

socket.on('connect', () => {
  console.log('SOCKET CONNECTED');
});

socket.on('end', () => {
  console.log('SOCKET CLOSED');
});
