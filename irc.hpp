#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <netdb.h>
# include <sys/uio.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <fstream>
# include <deque>
# include <vector>

# define MAX_CLIENT 5
# define BUFFER_SIZE 1024

typedef struct	s_server
{
	sockaddr_in	servAddr;
	int			serverSd;
	fd_set		readfds;
}				t_server;

typedef struct	s_client
{
	sockaddr_in	sendSockAddr;
	int			clientSd;
}				t_client;

#endif