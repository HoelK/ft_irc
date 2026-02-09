/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:25:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/09 02:28:18 by hkeromne         ###   ########.fr       */
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
				else if  (this->clients[this->fds[x].fd]->getAuth())
					this->acceptMessage(*(this->clients[this->fds[x].fd]));
				else
					this->authenticate(*(this->clients[this->fds[x].fd]));
				fds_ready--;
			}
		}
	}
}

void	Server::acceptMessage(Client &client)
{
	std::string	line;
	std::string	buffer;

	while (true)
	{
		buffer = Ft::getFdContent(client.getFd());
		std::cout << buffer;
		if (buffer.empty())
			break ;
		while (!buffer.empty())
		{
			line = client.getBuffer() + Ft::extractLine(buffer);
			client.setBuffer("");
			if (!Ft::endsWithCRLF(line))
				return (client.setBuffer(line));
			MSG::sendData(&client, line);
			CMD::apply(*this);
			RPL::reply(*this);
			if (package.quit)
				return ((void)this->disconnectClient(client.getFd()));
		}
	}
}

void	Server::acceptClient(void)
{
	struct pollfd	p;

	Client client(accept(this->fd, (sockaddr *) NULL, NULL));
	fcntl(client.getFd(), F_SETFL, O_NONBLOCK);

	p.fd = client.getFd();
	p.events = POLLIN;
	p.revents = 0;
	this->fds.push_back(p);
	this->clients[p.fd] = new Client(client);
	this->authenticate(*(this->clients[p.fd]));
}

void	Server::authenticate(Client &client)
{
	std::string line;
	std::string	buffer;

	buffer = Ft::getFdContent(client.getFd());
	std::cout << "BUFFER : " << buffer << std::endl;
	while (!buffer.empty())
	{
		line = client.getBuffer() + Ft::extractLine(buffer);
		client.setBuffer("");
		if (!Ft::endsWithCRLF(line))
			return (client.setBuffer(line));
		MSG::sendData(&client, line);
		CMD::apply(*this);
		if (package.cmd == CMD_PASS
		&& this->password != client.getPass())
		{
			package.error = ERR_PASSWDMISMATCH;
			RPL::reply(*this);
			return (this->disconnectClient(client.getFd()));
		}
	}
	if (!client.isAuth(this->password))
		return ;
	RPL::Welcome(client.getFd(), client.getNick());
	client.setAuth(true);
}

void	Server::disconnectClient(const int &fd)
{
	Client *client = this->clients[fd];
	close(client->getFd());
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
std::map<int, Client *>::iterator				Server::getClient(int const &id) { return (this->clients.find(id)); };

void										Server::createChannel(Channel &channel) { this->channels[channel.getName()] = channel; };
bool										Server::deleteChannel(std::string const &name) { return (this->channels.erase(name)); };
Channel										*Server::getChannel(std::string const &name) { return (&(this->channels.find(name)->second)); };
bool										Server::isChannel(std::string const &name) { return (this->channels.find(name) != this->channels.end()); };
