/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:35:28 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/04 01:45:29 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <map>
# include <vector>
# include <string>
# include <poll.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "MSG.hpp"
# include "RPL.hpp"
# include "Client.hpp"
# include "Channel.hpp"
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
		std::map<std::string, Client>		clients;
		std::map<std::string, Channel>		channels;
		struct pollfd						fds[MAX_CLIENT];
		nfds_t								nfds;

	
	public:
		Server(std::string password, int port);

		void	launch(void);
		void	kill(void);
		bool	init(void);

		void									acceptClient(void);
		bool									disconnectClient(std::string const &name);
		std::map<std::string, Client>::iterator	getClient(std::string const &name);

		void										createChannel(std::string const &name, Channel &channel);
		bool										deleteChannel(std::string const &name);
		std::map<std::string, Channel>::iterator	getChannel(std::string const &name);

		~Server(void);
};
