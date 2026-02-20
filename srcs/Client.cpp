/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:10 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 01:45:19 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"
# include "Server.hpp"

Client::Client(void): auth(false), fd(-1) {};
Client::~Client(void) {};
Client::Client(int fd): auth(false), fd(fd) {};
Client::Client(Client const &copy) { (*this) = copy; };

Client &Client::operator=(Client const &copy)
{
	this->fd = copy.fd;
	this->nick = copy.nick;
	this->auth = copy.auth;
	this->username = copy.username;
	this->realname = copy.realname;
	this->password = copy.password;
	this->sendBuffer = copy.sendBuffer;
	this->recvBuffer = copy.recvBuffer;
	this->channels = copy.channels;

	return (*this);
}

const int			&Client::getFd(void)		const			{ return (this->fd); };
const bool			&Client::getAuth(void)		const			{ return (this->auth); };
const std::string	&Client::getNick(void)		const			{ return (this->nick); };
const std::string	&Client::getName(void)		const			{ return (this->realname); };
const std::string	&Client::getUser(void)		const			{ return (this->username); };
const std::string	&Client::getPass(void)		const			{ return (this->password); };
const std::string	&Client::getSendBuffer(void)	const		{ return (this->sendBuffer); };
const std::string	&Client::getRecvBuffer(void)	const		{ return (this->recvBuffer); };
Channel				*Client::getChannel(std::string const &topic) const { return (this->channels.find(topic)->second); };
void				Client::setAuth(bool auth)					{ this->auth = auth; };
void				Client::setNick(std::string const &nick)	{ this->nick = nick; };
void				Client::setName(std::string const &name)	{ this->realname = name; };
void				Client::setUser(std::string const &user)	{ this->username = user; };
void				Client::setPass(std::string const &pass)	{ this->password = pass; };
void				Client::appendSendBuffer(std::string const &buff)	{ this->sendBuffer = this->sendBuffer + buff; };
void				Client::appendRecvBuffer(std::string const &buff)	{ this->recvBuffer = this->recvBuffer + buff; };
void				Client::clearSendBuffer(void) { this->sendBuffer.clear(); };
void				Client::clearRecvBuffer(void) { this->recvBuffer.clear(); };
void				Client::addChannel(Channel *channel)		{ this->channels[channel->getName()] = channel; };
void				Client::delChannel(std::string const &topic){ this->channels.erase(topic); };
bool				Client::isChannel(std::string const &topic) { return (this->channels.find(topic) != this->channels.end()); };
bool				Client::inChannel(void) const				{ return (!this->channels.size()); };
const std::map<std::string, Channel *> &Client::getChannels(void) const { return (this->channels); }

void				Client::sendMsg(void)
{
	if (send(this->getFd(), this->sendBuffer.c_str(), this->sendBuffer.size(), 0) != -1)
		this->sendBuffer.clear();
}

void				Client::updateInChannel(std::string const &oldNick)
{
	Channel *channel;
	for (std::map<std::string, Channel *>::const_iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		channel = it->second;
		channel->updateClient(oldNick);
	}
}

bool				Client::isAuth(std::string const &password)
{
	return (!this->nick.empty()
			&& !this->username.empty()
			&& !this->realname.empty()
			&& this->password == password);
}

void	Client::disconnection(Server &server)
{
	this->sendMsg();
	while (!this->channels.empty())
	{
		Channel *channel = this->channels.begin()->second;
		channel->removeOperator(this->nick);
		channel->removeInvited(this->nick);
		channel->removeClient(this->nick);
		this->channels.erase(channel->getName());
		if (channel->getClientAmount() == 0)
			server.deleteChannel(channel->getName());
	}
}

void	Client::broadcastMsg(std::string const &msg)
{
	for (std::map<std::string, Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		Channel *channel = it->second;
		channel->broadcastMessage(this, msg);
	}
}

std::ostream &operator<<(std::ostream &stream, std::map<std::string, Channel *> const &channels)
{
	stream << "===== CHANNELS ======" << std::endl;
	for (std::map<std::string, Channel *>::const_iterator it = channels.begin(); it != channels.end(); it++)
		stream << *(it->second);
	return (stream);
}

std::ostream &operator<<(std::ostream &stream, Client const &client)
{
	stream <<	"==== CLIENT ====" << std::endl;
	stream << "Fd : "	<<	client.getFd() << std::endl; 
	stream << "Name : "	<<	client.getName() << std::endl;
stream << "Nick : "	<<	client.getNick() << std::endl;
	stream << "User : "	<<	client.getUser() << std::endl;
	stream << client.getChannels();

	return (stream);
}
