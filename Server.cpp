# include "Server.hpp"

Server::~Server(void) {};
Server::Server(std::string password, int port): password(password), port(port) {};

bool	Server::deleteUser(std::string const &name) { (this->users.erase(name)); };
void	Server::addUser(std::string const &name, User &in_user) { this->users[name] = in_user };

User	*Server::getUser(std::string const &name)
{
	std::map<std::string, User>::iterator it;

	it = this->users.find(name);
	if (it == this->users.end())
		return (NULL);
	return (it);
}


bool	Server::deleteChannel(std::string const &name) { return (this->users.erase(name)); };
void	Server::addChannel(std::string &name, User &in_channel) { this->channels[name] = in_channel };

Channel	*Server::getChannel(std::string const &name)
{
	std::map<std::string, Channel>::iterator it;

	it = this->channels.find(name);
	if (it == this->channels.end())
		return (NULL);
	return (it);
}
