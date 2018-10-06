#include "socket.h"

#include "machine.h"
#include "debug.h"


namespace Socket
{

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

const u32 BUF_SIZE = 500;
const u32 PORT = 62626;

static int server = -1;


bool
init()
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

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server, (sockaddr *) &server_addr, sizeof(sockaddr)) == -1)
  {
     perror("Error binding");
     success &= false;
     return success;
  }

  const u32 LISTEN_BACKLOG = 10;
  if (listen(server, LISTEN_BACKLOG) == -1)
  {
     perror("Error listening");
     success &= false;
     return success;
  }

  printf("Socket initialised\n");

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
advance(Machine::Machine& machine)
{
  bool success = true;

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