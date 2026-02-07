# include "Client.hpp"

Client::Client(void): fd(0), channel(NULL) {};
Client::~Client(void) {};
Client::Client(int fd): fd(fd), channel(NULL) {};
Client::Client(Client const &copy) { (*this) = copy; };

Client &Client::operator=(Client const &copy)
{
	this->fd = copy.fd;
	this->nick = copy.nick;
	this->username = copy.username;
	this->realname = copy.realname;
	this->buffer = copy.buffer;
	this->channel = copy.channel;

	return (*this);
}

const int			&Client::getFd(void) const { return (this->fd); };
const unsigned int	&Client::getId(void) const { return (this->id); }; 
const std::string	&Client::getNick(void) const { return (this->nick); };
const std::string	&Client::getName(void) const { return (this->realname); };
const std::string	&Client::getUser(void) const { return (this->username); };
const std::string	&Client::getBuffer(void) const { return (this->buffer); };
void				Client::setNick(std::string const &nick) { this->nick = nick; };
void				Client::setName(std::string const &name) { this->realname = name; };
void				Client::setUser(std::string const &user) { this->username = user; };
void				Client::setBuffer(std::string const &buff) { this->buffer = buff; };
void				Client::setId(const int id) { this->id = id; };
