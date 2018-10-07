#include "socket.h"

#include "machine.h"
#include "debug.h"


namespace Socket
{

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

const u32 BUF_SIZE = 500;
const u32 PORT = 62626;

static int server = -1;


bool
init_client()
{
  bool success = true;

  sockaddr_in client_addr;
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = 0;

  if (bind(server, (sockaddr *) &client_addr, sizeof(sockaddr)) == -1)
  {
     perror("Error binding");
     success &= false;
  }
  else
  {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.188");
    server_addr.sin_port = htons(PORT);

    if (connect(server, (sockaddr *) &server_addr, sizeof(sockaddr)) == -1)
    {
       perror("Error connecting");
       success &= false;
    }
  }

  return success;
}

bool
init_server()
{
  bool success = true;

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server, (sockaddr *) &server_addr, sizeof(sockaddr)) == -1)
  {
     perror("Error binding");
     success &= false;
  }
  else
  {
    const u32 LISTEN_BACKLOG = 10;
    if (listen(server, LISTEN_BACKLOG) == -1)
    {
       perror("Error listening");
       success &= false;
    }
  }

  return success;
}

bool
init(bool receive)
{
  bool success = true;

  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server == -1)
  {
     perror("Error creating socket");
     success &= false;
     return success;
  }

  if (fcntl(server, F_SETFL, O_NONBLOCK))
  {
    perror("Error setting non blocking");
    success &= false;
    return success;
  }

  if (receive)
  {
    success &= init_client();
  }
  else
  {
    success &= init_server();
  }

  if (success)
  {
    printf("Socket initialised\n");
  }

  return success;
}


void
close_socket(s32 socket_fd)
{
  if (socket_fd != -1)
  {
    close(socket_fd);
    socket_fd = -1;
  }
}


static bool connected = false;
static s32 client = -1;


bool
write_data(s32 socket_fd, void* data, u32 size)
{
  bool success = true;

  if (write(client, data, size) < 0)
  {
    if (errno == EBADF)
    {
      printf("Client disconnecting\n");
      close_socket(client);
    }
    else
    {
      perror("Error sending message");
    }
    success &= false;
  }

  return success;
}


bool
advance(bool receive, Machine::Machine& machine)
{
  bool success = true;

  if (receive)
  {
    u16 size;
    if (read(server, &size, sizeof(u16)) == -1)
    {
      perror("Error reading size");
      success &= false;
    }
    else
    {
      printf("%u\n", size);
      // if (size != sizeof(machine.memory))
      // {
      //   printf("TOO BIG %u\n", size);
      //   return false;
      // }

      if (read(server, &machine.memory, sizeof(machine.memory)) == -1)
      {
        perror("Error writing memory");
        success &= false;
      }
    }

    return success;
  }

  if (!connected && server != -1)
  {
    socklen_t client_addr_size;
    sockaddr client_addr;
    client_addr_size = sizeof(sockaddr);
    client = accept(server, &client_addr, &client_addr_size);
    if (client == -1 )
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        connected = false;
        perror("Error accepting");
        success &= false;
      }
    }
    else
    {
      connected = true;
      printf("Successfully connected.\n");
    }
  }

  if (connected)
  {
    printf("Sending message\n");

    assert(sizeof(machine.memory) < (1<<16));
    u16 message_size = sizeof(machine.memory);

    success &= write_data(client, &message_size, sizeof(message_size));
    if (success)
    {
      success &= write_data(client, &machine.memory, message_size);
    }
  }

  return success;
}


void
destroy()
{
  connected = false;
  if (client != -1)
  {
    close_socket(client);
  }

  if (server != -1)
  {
    close_socket(server);
  }
}

} // namespace Socket