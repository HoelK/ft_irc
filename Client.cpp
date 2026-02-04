# include "Client.hpp"

Client::Client(void) {};
Client::~Client(void) {};
Client::Client(int fd): fd(fd) {};
Client::Client(Client const &copy) { (*this) = copy; };

Client &Client::operator=(Client const &copy)
{
	this->fd = copy.fd;
	this->nick = copy.nick;
	this->realname = copy.realname;
	this->buffer = copy.buffer;

	return (*this);
}

const int &Client::getFd(void) const { return (this->fd); };
const std::string& Client::getNick(void) const { return (this->nick); };
const std::string& Client::getName(void) const { return (this->realname); };
void Client::setNick(std::string const &nick) { this->nick = nick; };
void Client::setName(std::string const &name) { this->realname = name; };
