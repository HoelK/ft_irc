/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:25:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 22:23:53 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

Server::~Server(void) {};
Server::Server(std::string password, int port): fd(0), port(port), password(password) {};

bool	Server::init(void)
{
	struct pollfd	p;

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
	p.fd = fd;
	p.events = POLLIN;
	p.revents = 0;
	this->fds.push_back(p);
	return (true);
}

void	Server::launch(void)
{
	int	fds_ready = 0;

	while (true)
	{
		fds_ready = poll(this->fds.data(), this->fds.size(), 10);
		for (int x = 0; x < (int)this->fds.size() && fds_ready > 0; x++)
		{
			if (fds[x].revents & POLLIN)
			{
				if (x == 0)
					this->acceptClient();
				else
					this->acceptMessage(this->clients_i[this->ids[x]]);
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
			line = client.getBuffer() + Ft::extractLine(buffer);
			client.setBuffer("");
			this->clients_i[client.getId()].setBuffer("");
			this->clients_s[client.getName()].setBuffer("");
			if (!Ft::endsWithCRLF(line))
			{
				client.setBuffer(line);
				this->clients_i[client.getId()].setBuffer(line);
				this->clients_s[client.getName()].setBuffer(line);
				return ;
			}
			MSG::sendData(&client, line);
			CMD::apply();
			RPL::reply();
		}
	}
}

void	Server::acceptClient(void)
{
	struct pollfd	p;

	Client client(accept(this->fd, (sockaddr *) NULL, NULL));
	fcntl(client.getFd(), F_SETFL, O_NONBLOCK);
	client.setId(this->fds.size());
	this->ids.push_back(this->fds.size());
	this->clients_i[client.getId()] = client;
	this->clients_s[client.getName()] = client;

	p.fd = fd;
	p.events = POLLIN;
	p.revents = 0;
	this->fds.push_back(p);
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
		line = client.getBuffer() + Ft::extractLine(buffer);
		client.setBuffer("");
		this->clients_i[client.getId()].setBuffer("");
		this->clients_s[client.getName()].setBuffer("");
		if (!Ft::endsWithCRLF(line))
		{
			client.setBuffer(line);
			this->clients_i[client.getId()].setBuffer(line);
			this->clients_s[client.getName()].setBuffer(line);
			return ;
		}
		MSG::sendData(&client, line);
		CMD::apply();
		if (package.quit)
			return ((void)this->disconnectClient(client.getName(), client.getId()));
	}
	if (!client.isAuth(this->password))
		return ;
	RPL::connection(client.getFd(), client.getNick());
	client.setAuth(true);
	this->clients_i[client.getId()].setAuth(true);
	this->clients_s[client.getName()].setAuth(true);
}

void										Server::disconnectClient(std::string const &name, const int &id)
{
	this->fds.erase(this->fds.begin() + id);
	this->ids.erase(this->ids.begin() + id);
	this->clients_s.erase(name);
	this->clients_i.erase(id);

}
std::map<std::string, Client>::iterator		Server::getClient(std::string const &name) { return (this->clients_s.find(name)); };
std::map<int, Client>::iterator				Server::getClient(int const &id) { return (this->clients_i.find(id)); };

void										Server::createChannel(std::string const &name, Channel &channel) { this->channels[name] = channel; };
bool										Server::deleteChannel(std::string const &name) { return (this->channels.erase(name)); };
std::map<std::string, Channel>::iterator	Server::getChannel(std::string const &name) { return (this->channels.find(name)); };
