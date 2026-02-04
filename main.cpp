# include <arpa/inet.h>
# include <poll.h>
# include <netinet/in.h>
# include <unistd.h>
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include "Server.hpp"

std::string		code_to_string(short code);

// SERVER
# define VERSION "1.0"
# define CMODES "itkol"
# define SERVER_NAME "ft.irc"
# define NETWORK_NAME "CACA"

//RPL MESSAGES
# define RPL_WELCOME	1
# define RPL_YOURHOST	2
# define RPL_CREATED	3
# define RPL_MYINFO		4
# define RPL_ISUPPORT	5

void	acceptClient()
{
}

bool	argCheck(int ac, char **av)
{
	int					port;
	std::stringstream	stream;

	if (ac < 3)
	{
		std::cout << "Not enough arguments" << std::endl;
		return (false);
	}
	else if (ac > 3)
	{
		std::cout << "Too much arguments" << std::endl;
		return (false);
	}
	stream << std::string(av[2]);
	stream >> port;
	if (stream.fail() || port < 1 || port > 65535)
	{
		std::cout << "Invalid Port" << std::endl;
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	if (!argCheck(ac, av))
		return (EXIT_FAILURE);

	Server server(std::string(av[1]), std::atoi(av[2]));
	server.init();
	server.launch();
}
