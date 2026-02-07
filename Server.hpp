/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:35:28 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 17:56:45 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <map>
# include <vector>
# include <string>
# include <poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Ft.hpp"
# include "MSG.hpp"
# include "RPL.hpp"
# include "CMD.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
# define MAX_CLIENT			1024
# define DEFAULT_PROTOCOL	0
# define IPV4				AF_INET
# define PORT				htons(this->port)
# define ANY_ADDRESS		htonl(INADDR_ANY)

class Server
{
	private:
		int									fd;
		struct sockaddr_in					addr;
		int									port;
		std::string							password;
		std::map<int, Client>				clients_i;
		std::map<std::string, Client>		clients_s;
		std::map<std::string, Channel>		channels;
		struct pollfd						fds[MAX_CLIENT];
		nfds_t								nfds;

	
	public:
		Server(std::string password, int port);

		void	launch(void);
		void	kill(void);
		bool	init(void);

		void										addPollFd(const int &fd);
		void										authenticate(Client &client);

		void										acceptClient(void);
		bool										disconnectClient(std::string const &name, const int &id);
		std::map<std::string, Client>::iterator		getClient(std::string const &name);
		std::map<int, Client>::iterator				getClient(int const &id);

		void										createChannel(std::string const &name, Channel &channel);
		bool										deleteChannel(std::string const &name);
		std::map<std::string, Channel>::iterator	getChannel(std::string const &name);

		void	acceptMessage(Client &client);

		~Server(void);
};
