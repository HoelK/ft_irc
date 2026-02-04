# include "Server.hpp"

Server::~Server(void) {};
Server::Server(std::string password, int port): fd(0), port(port), password(password), nfds(0) {};

bool	Server::init(void)
{
	this->fd = socket(IPV4, SOCK_STREAM, DEFAULT_PROTOCOL);
	if (this->fd == -1) //|| fcntl(this->fd, F_SETFL, fcntl(this->fd, F_GETFL) | O_NONBLOCK) == -1)
		return (false);
	this->addr.sin_family		= IPV4;
	this->addr.sin_addr.s_addr	= ANY_ADDRESS;
	this->addr.sin_port			= PORT;
	
	socklen_t socklen = sizeof(this->addr);
	if (bind(this->fd, (sockaddr *) &this->addr, socklen) == -1 || listen(this->fd, 10) == -1)
		return (false);
	this->fds[0].fd = this->fd;
	this->fds[0].events = POLLIN;
	this->fds[0].revents = 0;
	this->nfds++;

	return (true);
}

void	Server::launch(void)
{
	int	fds_ready = 0;

	while (true)
	{
		fds_ready = poll(this->fds, this->nfds, 10);
		for (int i = 0; i < (int)this->nfds && fds_ready > 0; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (i == 0)
				{
					this->acceptClient();
					fds_ready--;
				}
			}
		}
	}
}

void	Server::acceptClient()
{
	std::string token;
	char		buffer[4096];

	std::cerr << "waiting" << std::endl;
	Client client(accept(this->fd, (sockaddr *) NULL, NULL));
	std::cout << "accepted" << std::endl;
	if (recv(client.getFd(), buffer, 4096, 0) > 0)
	{
		std::cout << buffer;
		std::stringstream	stream(buffer);
		MSG::getToCmd(stream, "NICK");
		stream >> token;
		client.setNick(token);
		MSG::getToCmd(stream, "USER");
		stream >> token;
		client.setName(token);
	}
	RPL::connection(client.getFd(), client.getNick());

	//this->clients[name] = client;
};

bool	Server::disconnectClient(std::string const &name) { return (this->clients.erase(name)); };
std::map<std::string, Client>::iterator Server::getClient(std::string const &name) { return (this->clients.find(name)); };

void	Server::createChannel(std::string const &name, Channel &channel) { this->channels[name] = channel; };
bool	Server::deleteChannel(std::string const &name) { return (this->channels.erase(name)); };
std::map<std::string, Channel>::iterator Server::getChannel(std::string const &name) { return (this->channels.find(name)); };
