/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:25:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/20 02:58:23 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"
# include "MSG.hpp"
# include "RPL.hpp"
# include "CMD.hpp"
# include "Errors.hpp"

sig_atomic_t	sigShutdown = 0;

Server::~Server(void)
{
	close(this->fd);
    while (!this->clients.empty())
        this->disconnectClient(this->clients.begin()->second->getFd());
}

Server::Server(std::string password, int port): fd(0), port(port), password(password) {};

bool	Server::init(void)
{
	struct pollfd	p;

	this->fd = socket(IPV4, SOCK_STREAM, DEFAULT_PROTOCOL);
	this->fds.reserve(1034);
	if (this->fd == -1 || fcntl(this->fd, F_SETFL, O_NONBLOCK))
		return (false);
	this->addr.sin_family		= IPV4;
	this->addr.sin_addr.s_addr	= ANY_ADDRESS;
	this->addr.sin_port			= PORT;
	
	socklen_t socklen = sizeof(this->addr);
	if (bind(this->fd, (sockaddr *) &this->addr, socklen) == -1
	|| listen(this->fd, 10) == -1)
		return (false);
	p.fd = fd;
	p.events = POLLIN;
	p.revents = 0;
	this->fds.push_back(p);
	this->startTime = Ft::getTime();
	return (true);
}

void	Server::launch(void)
{
	int		fds_ready = 0;

	while (!sigShutdown)
	{
		fds_ready = poll(this->fds.data(), this->fds.size(), 10);
		for (int x = 0; x < (int)this->fds.size() && fds_ready > 0; x++)
		{
			if (fds[x].revents & POLLIN)
			{
				if (x == 0)
					this->acceptClient();
				else if  (this->clients[this->fds[x].fd]->getAuth())
					this->acceptMessage(*(this->clients[this->fds[x].fd]));
				else
					this->authenticate(*(this->clients[this->fds[x].fd]));
				fds_ready--;
			}
			if ((int)this->fds.size() > x && this->fds[x].revents & POLLOUT)
			{
				Client *client = this->clients[this->fds[x].fd];
				if (!(client->getSendBuffer().empty()))
					client->sendMsg();
			}
		}
	}
}

void	Server::acceptMessage(Client &client)
{
	std::string	line;
	std::string	buffer;

	buffer = Ft::getFdContent(client.getFd());
	if (buffer.empty())
		return (this->disconnectClient(client.getFd()));
	while (!buffer.empty())
	{
		line = client.getRecvBuffer() + Ft::extractLine(buffer);
		client.clearRecvBuffer();
		if (!Ft::endsWithCRLF(line))
			return (client.appendRecvBuffer(line));
		std::cout << "[CMD] " << line;
		MSG::sendData(&client, line);
		CMD::apply(*this);
		RPL::reply(*this);
		if (package.quit)
			return (this->disconnectClient(client.getFd()));
	}
}

void	Server::acceptClient(void)
{
	int				fd;
	struct pollfd	pfd;

	if (this->clients.size() >= FD_MAX)
		return ;
	fd = accept(this->fd, (sockaddr *) NULL, NULL);
	if (fd == -1 || fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		return ;

	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	this->fds.push_back(pfd);
	this->clients[pfd.fd] = new Client(this->fds.back().fd);
}

bool	Server::passCheck(std::string const &pass) { return (this->password == pass); };

void	Server::authenticate(Client &client)
{
	std::string line;
	std::string	buffer;

	buffer = Ft::getFdContent(client.getFd());
	if (buffer.empty())
		return (this->disconnectClient(client.getFd()));
	while (!buffer.empty())
	{
		line = client.getRecvBuffer() + Ft::extractLine(buffer);
		client.clearRecvBuffer();
		if (!Ft::endsWithCRLF(line))
			return (client.appendRecvBuffer(line));
		std::cout << "[AUTH][CMD] " << line;
		MSG::sendData(&client, line);

		if (!client.getPass().empty())
		{
			if (!this->passCheck(client.getPass()))
			{
				package.setError(ERR_PASSWDMISMATCH);
				RPL::reply(*this);
				return (this->disconnectClient(client.getFd()));
			}
			if ((client.getNick().empty()
				|| client.getUser().empty()
				|| client.getName().empty())
				&& (package.cmd != "NICK" && package.cmd != "USER"))
			{
				package.setError(ERR_NOTREGISTERED);
				RPL::reply(*this);
				continue ;
			}
		}
		else if (package.cmd != "PASS")
		{
			package.setError(ERR_NOTREGISTERED);
			RPL::reply(*this);
			continue ;
		}

		CMD::apply(*this);
		RPL::reply(*this);
	}
	if (!client.isAuth(this->password))
		return ;

	RPL::Welcome((*this), &client, client.getNick());
	client.setAuth(true);
	std::cout << "[AUTH][INFO] " << client.getNick() << " Connected" << std::endl;
}

void	Server::disconnectClient(const int &fd)
{
	Client *client = this->clients[fd];
	
	client->disconnection(*this);
	close(client->getFd());
	this->clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = this->fds.begin(); it != this->fds.end(); it++)
	{
		if (it->fd == fd)
		{
			this->fds.erase(it);
			break ;
		}
	}
	delete (client);
}

std::string const	&Server::getStartTime(void) const				{ return (this->startTime); };

Channel				*Server::getChannel(std::string const &name)	{ return (&(this->channels.find(name)->second)); };
void				Server::createChannel(Channel &channel)			{ this->channels[channel.getName()] = channel; };
bool				Server::deleteChannel(std::string const &name)	{ return (this->channels.erase(name)); };
bool				Server::isChannel(std::string const &name)		{ return (this->channels.find(name) != this->channels.end()); };

bool				Server::isClient(std::string const &nick)		{ return (this->getClient(nick) != NULL); };
Client				*Server::getClient(int const &id)				{ return (this->clients.find(id)->second); };
Client				*Server::getClient(std::string const &nick)
{
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->second->getNick() == nick)
			return (it->second);
	}
	return (NULL);
}
