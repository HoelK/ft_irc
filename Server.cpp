/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:25:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 18:28:19 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::~Server(void) {};
Server::Server(std::string password, int port): fd(0), port(port), password(password), nfds(0) {};

void	Server::addPollFd(const int &fd)
{
	this->fds[nfds].fd = fd;
	this->fds[nfds].events = POLLIN;
	this->fds[nfds].revents = 0;
	this->nfds++;
}

bool	Server::init(void)
{
	this->fd = socket(IPV4, SOCK_STREAM, DEFAULT_PROTOCOL);
	if (this->fd == -1 || fcntl(this->fd, F_SETFL, O_NONBLOCK))
		return (false);
	this->addr.sin_family		= IPV4;
	this->addr.sin_addr.s_addr	= ANY_ADDRESS;
	this->addr.sin_port			= PORT;
	
	socklen_t socklen = sizeof(this->addr);
	if (bind(this->fd, (sockaddr *) &this->addr, socklen) == -1
	|| listen(this->fd, 10) == -1)
		return (false);

	this->addPollFd(this->fd);
	return (true);
}

void	Server::launch(void)
{
	int	fds_ready = 0;

	while (true)
	{
		fds_ready = poll(this->fds, this->nfds, 10);
		for (int id = 0; id < (int)this->nfds && fds_ready > 0; id++)
		{
			if (fds[id].revents & POLLIN)
			{
				if (id == 0)
					this->acceptClient();
				else
					this->acceptMessage(this->clients_i[id]);
				fds_ready--;
			}
		}
	}
}

void	Server::acceptMessage(Client &client)
{
	std::string	line;
	std::string	buffer;

	if (!client.getAuth())
		return (this->authenticate(client));
	while (true)
	{
		buffer = Ft::getFdContent(client.getFd());
		if (buffer.empty())
			break ;
		while (!buffer.empty())
		{
			line = Ft::extractLine(buffer);
			MSG::sendData(&client, line);
			CMD::apply();
			RPL::reply();
		}
	}
}

void	Server::acceptClient(void)
{
	std::string line;
	std::string	buffer;

	if (nfds >= MAX_CLIENT)
		return ;
	Client client(accept(this->fd, (sockaddr *) NULL, NULL));
	fcntl(client.getFd(), F_SETFL, O_NONBLOCK);
	client.setId(nfds);
	this->clients_i[nfds] =	client;
	this->clients_s[client.getName()] =	client;
	this->addPollFd(client.getFd());
	this->authenticate(client);
}

void	Server::authenticate(Client &client)
{
	std::string line;
	std::string	buffer;

	if (client.getAuth())
		return ;
	buffer = Ft::getFdContent(client.getFd());
	if (buffer.empty())
		return ;
	while (!buffer.empty())
	{
		line = Ft::extractLine(buffer);
		MSG::sendData(&client, line);
		CMD::apply();
	}
	if (!client.isAuth(this->password))
		return ;
	RPL::connection(client.getFd(), client.getNick());
	client.setAuth(true);
	this->clients_i[client.getId()].setAuth(true);
	this->clients_s[client.getName()].setAuth(true);
}

bool										Server::disconnectClient(std::string const &name, const int &id) { return (this->clients_s.erase(name) && this->clients_i.erase(id)); };
std::map<std::string, Client>::iterator		Server::getClient(std::string const &name) { return (this->clients_s.find(name)); };
std::map<int, Client>::iterator				Server::getClient(int const &id) { return (this->clients_i.find(id)); };

void										Server::createChannel(std::string const &name, Channel &channel) { this->channels[name] = channel; };
bool										Server::deleteChannel(std::string const &name) { return (this->channels.erase(name)); };
std::map<std::string, Channel>::iterator	Server::getChannel(std::string const &name) { return (this->channels.find(name)); };
