# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <sstream>

std::string		code_to_string(short code);

//SOCKET
# define DEFAULT_PROTOCOL	0
# define IPV4				AF_INET
# define PORT				htons(std::atoi(av[1]))
# define ANY_ADDRESS		htonl(INADDR_ANY)

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

# define NICK_TEST "hkeromne"

# define RPL_HEADER(code, nick)	":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " :"

# define RPL_WELCOME_STR(nick)	"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR		"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR		"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)	std::string(nick) + SERVER_NAME VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR		""

class User
{
};

class Channel
{
};

//server:
//	- channels:
//		- users
//		- channel settings
//		- user settings
//	- users:
//		- name
//		- socket
//	- services:

std::string		code_to_string(short code)
{
	std::stringstream	stream;

	if (code < 10)
		stream << "00";
	else if (code >= 10 && code < 100)
		stream << "0";
	stream << code;
	return (stream.str());
}

std::string		getMessage(short code, std::string nick)
{
	if (code == RPL_WELCOME)
		return (RPL_WELCOME_STR(nick));
	else if (code == RPL_YOURHOST)
		return (RPL_YOURHOST_STR);
	else if (code == RPL_CREATED)
		return (RPL_CREATED_STR);
	else if (code == RPL_MYINFO)
		return (RPL_MYINFO_STR(nick));
	else if (code == RPL_ISUPPORT)
		return (RPL_ISUPPORT_STR);

	return ("");
}

std::string		createMessage(short code, std::string nick)
{
	std::stringstream	stream;

	stream << RPL_HEADER(code_to_string(code), nick);
	stream << getMessage(code, nick);
	stream << "\r\n";

	return (stream.str());
}

void			sendConnectionMessage(int fd, std::string nick)
{
		std::string sent_message = createMessage(RPL_WELCOME,	nick);
		send(fd, sent_message.c_str(), sent_message.size(), 0);
		sent_message = createMessage(RPL_YOURHOST,	nick);
		send(fd, sent_message.c_str(), sent_message.size(), 0);
		sent_message = createMessage(RPL_CREATED,	nick);
		send(fd, sent_message.c_str(), sent_message.size(), 0);
		sent_message = createMessage(RPL_MYINFO,	nick);
		send(fd, sent_message.c_str(), sent_message.size(), 0);
		sent_message = createMessage(RPL_ISUPPORT,	nick);
		send(fd, sent_message.c_str(), sent_message.size(), 0);
}

void		initServer(int &sock, struct sockaddr_in &sockaddr, socklen_t &socklen, char **av)
{
	sock = socket(IPV4, SOCK_STREAM, DEFAULT_PROTOCOL);
	sockaddr.sin_family			= IPV4;
	sockaddr.sin_addr.s_addr	= ANY_ADDRESS;
	sockaddr.sin_port			= PORT;
	
	socklen = sizeof(sockaddr);
}

int	main(int ac, char **av)
{
	if (ac != 2) { std::cout << "arg error" << std::endl; return (EXIT_FAILURE); };

	int					sock;
	socklen_t			socklen;
	struct sockaddr_in	servaddr;
	int					client_fd;
	char				buffer[514];

	initServer(sock, servaddr, socklen, av);
	bind(sock, (sockaddr *) &servaddr, socklen);
	listen(sock, 10);
	while (true)
	{
		std::cout << "waiting connection..." << std::endl;
		client_fd = accept(sock, (sockaddr *) NULL, NULL);
		std::cout << "connection made" << std::endl;

		sendConnectionMessage(client_fd, NICK_TEST);
		while (recv(client_fd, buffer, sizeof(char) * 514, 0))
		{
			std::cout << buffer;
			bzero(buffer, sizeof(char) * 300);
		}
		close(client_fd);
	}
}
