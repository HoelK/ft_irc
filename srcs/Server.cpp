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
	this->ids.push_back(0);
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
				else if  (this->clients[this->ids[x]]->getAuth())
					this->acceptMessage(*(this->clients[this->ids[x]]));
				else
					this->authenticate(*(this->clients[this->ids[x]]));
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
			CMD::apply();
			RPL::reply();
			if (package.quit)
				return ((void)this->disconnectClient(client.getId()));
		}
	}
}

void	Server::acceptClient(void)
{
	struct pollfd	p;

	Client client(accept(this->fd, (sockaddr *) NULL, NULL));
	fcntl(client.getFd(), F_SETFL, O_NONBLOCK);
	client.setId(this->fds.size());

	p.fd = client.getFd();
	p.events = POLLIN;
	p.revents = 0;
	this->fds.push_back(p);
	this->ids.push_back(this->ids.size());
	this->clients[client.getId()] = new Client(client);
	this->authenticate(*(this->clients[client.getId()]));
}

void	Server::authenticate(Client &client)
{
	std::string line;
	std::string	buffer;

	buffer = Ft::getFdContent(client.getFd());
	while (!buffer.empty())
	{
		line = client.getBuffer() + Ft::extractLine(buffer);
		client.setBuffer("");
		if (!Ft::endsWithCRLF(line))
			return (client.setBuffer(line));
		MSG::sendData(&client, line);
		CMD::apply();
		if (package.cmd == CMD_PASS
		&& this->password != client.getPass())
		{
			package.error = ERR_PASSWDMISMATCH;
			RPL::reply();
			return (this->disconnectClient(client.getId()));
		}
	}
	if (!client.isAuth(this->password))
		return ;
	RPL::Welcome(client.getFd(), client.getNick());
	client.setAuth(true);
}

void										Server::disconnectClient(const int &id)
{
	Client *client = this->clients[id];
	close(client->getFd());
	this->fds.erase(this->fds.begin() + id);
	this->ids.erase(this->ids.begin() + id);
	this->clients.erase(id);
	delete (client);
}
std::map<int, Client *>::iterator				Server::getClient(int const &id) { return (this->clients.find(id)); };

void										Server::createChannel(std::string const &name, Channel &channel) { this->channels[name] = channel; };
bool										Server::deleteChannel(std::string const &name) { return (this->channels.erase(name)); };
std::map<std::string, Channel>::iterator	Server::getChannel(std::string const &name) { return (this->channels.find(name)); };
