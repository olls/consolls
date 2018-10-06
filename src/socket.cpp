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
     perror("socket error");
     success &= false;
     return success;
  }

  if (fcntl(server, F_SETFL, O_NONBLOCK))
  {
    perror("setting non blocking");
    success &= false;
    return success;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server, (sockaddr *) &server_addr, sizeof(sockaddr)) == -1)
  {
     perror("bind error");
     success &= false;
     return success;
  }

  const u32 LISTEN_BACKLOG = 10;
  if (listen(server, LISTEN_BACKLOG) == -1)
  {
     perror("listen error");
     success &= false;
     return success;
  }

  printf("Socket initialised\n");

  return success;
}


static bool connected = false;
static int client = -1;

bool
advance(Machine::Machine& machine)
{
  static socklen_t client_addr_size;
  static sockaddr client_addr;

  bool success = true;

  if (!connected && server != -1)
  {
    client_addr_size = sizeof(sockaddr);
    client = accept(server, &client_addr, &client_addr_size);
    if (client == -1 )
    {
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        connected = false;
        perror("accept error");
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
    assert(sizeof(machine.memory) < ((1<<16)-1));
    u16 message_size = sizeof(machine.memory);
    if (write(client, &message_size, sizeof(message_size)) < 0)
    {
      perror("sending header");
    }
    if (write(client, &machine.memory, message_size) < 0)
    {
      perror("sending message");
    }
  }

  return success;
}


void
close_socket(int socket_fd)
{
  if (socket_fd != -1)
  {
    close(socket_fd);
    socket_fd = -1;
  }
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