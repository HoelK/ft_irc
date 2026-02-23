/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:35:28 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/20 02:58:07 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <map>
# include <vector>
# include <string>
# include <poll.h>
# include <fcntl.h>
# include <sstream>
# include <cstdlib>
# include <csignal>
# include <unistd.h>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Ft.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# define PORT_ID 1
# define PASS_ID 2
# define FD_MAX 1024 - 3
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
# define DEFAULT_PROTOCOL	0
# define IPV4				AF_INET
# define PORT				htons(this->port)
# define ANY_ADDRESS		htonl(INADDR_ANY)

extern sig_atomic_t	sigShutdown;

class Server
{
	private:
		int								fd;
		struct sockaddr_in				addr;
		int								port;
		std::string						password;
		std::string						startTime;
		std::map<int, Client *>			clients;
		std::map<std::string, Channel>	channels;
		std::vector<struct pollfd>		fds;
	
	public:
		Server(std::string password, int port);

		void	launch(void);
		void	kill(void);
		bool	init(void);

		void	addPollFd(const int &fd);
		void	authenticate(Client &client);

		void	acceptClient(void);
		void	disconnectClient(const int &id);
		Client	*getClient(int const &id);
		Client	*getClient(std::string const &nick);

		void	createChannel(Channel &channel);
		bool	deleteChannel(std::string const &name);
		Channel	*getChannel(std::string const &name);

		void	acceptMessage(Client &client);
		bool	isChannel(std::string const &name);
		bool	isClient(std::string const &nick);

		bool	passCheck(std::string const &pass);

		std::string	const &getStartTime(void) const;

		~Server(void);
};
