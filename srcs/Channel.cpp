/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:48:22 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/20 00:53:23 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Channel.hpp"
# include "Client.hpp"

Channel::Channel(void): opKey(false), opTopic(false), opLimit(0), opInvite(false) {};
Channel::~Channel(void) {};
Channel::Channel(Channel const &copy): opKey(false), opTopic(false), opLimit(0), opInvite(false) { (*this) = copy; };
Channel::Channel(const std::string& name): name(name) {};
Channel	&Channel::operator=(Channel const &copy)
{
	this->opKey = copy.opKey;
	this->opTopic = copy.opTopic;
	this->opLimit = copy.opLimit;
	this->opInvite = copy.opInvite;
	this->name = copy.name;
	this->topic = copy.topic;
	this->invited = copy.invited;
	this->operators = copy.operators;
	this->clients = copy.clients;
	return (*this);
}

const bool			&Channel::getOpKey(void)	const			{ return (this->opKey); };
const bool			&Channel::getOpTopic(void)	const			{ return (this->opTopic); };
const bool			&Channel::getOpInvite(void)	const			{ return (this->opInvite); };
const int			&Channel::getOpLimit(void)	const			{ return (this->opLimit); };
const std::string	&Channel::getPassword(void)	const			{ return (this->password); };
const std::string	&Channel::getName(void)		const			{ return (this->name); };
const std::string	&Channel::getTopic(void)	const			{ return (this->topic); };
int					Channel::getClientAmount(void) const		{ return (this->clients.size()); };
void				Channel::setOpKey(bool const &key, std::string const &password) { this->opKey = key; this->password = password; };
void				Channel::setOpTopic(bool const &topic)		{ this->opTopic = topic; };
void				Channel::setOpInvite(bool const &invite)	{ this->opInvite = invite; };
void				Channel::setOpLimit(int const &limit)		{ this->opLimit = limit; };
void				Channel::setName(std::string const &name)	{ this->name = name; };
void				Channel::setTopic(std::string const &topic)	{ this->topic = topic; };

void				Channel::addInvited(Client *client)		{ this->invited[client->getFd()] = client; };
bool				Channel::removeInvited(const int &fd)	{ return (this->invited.erase(fd)); };
bool				Channel::isInvited(const int &fd) const		{ return (this->invited.count(fd)); };

void				Channel::addOperator(Client *client)	{ this->operators[client->getFd()] = client; };
bool				Channel::removeOperator(const int &fd)	{ return (this->operators.erase(fd)); };
bool				Channel::isOperator(const int &fd) const		{ return (this->operators.count(fd)); };

Client				*Channel::getClient(const int &fd)		{ return (this->clients.find(fd)->second); };
void				Channel::addClient(Client *client)		{ this->clients[client->getFd()] = client; };
bool				Channel::removeClient(const int &fd)	{ return (this->clients.erase(fd)); };
bool				Channel::isClient(const int &fd)		{ return (this->clients.count(fd)); };

bool				Channel::isFull(void) { return (this->opLimit && (int)this->clients.size() >= this->opLimit); }
bool				Channel::Auth(std::string const &password) { return (this->password == password); };

std::string			Channel::getModes(void)
{
	std::string		result("+");
	std::string		params;

	if (this->opKey)
	{
		result = result + MODE_KEY;
		params = params + this->password;
	}
	if (this->opTopic)
		result = result + MODE_TOPIC;
	if (this->opInvite)
		result = result + MODE_INVITE;
	if (this->opLimit)
	{
		result = result + MODE_LIMIT;
		params = params + Ft::intToStr(this->opLimit);
	}
	return (result + " " + params);
}

void				Channel::broadcastMessage(Client *sender, std::string const &msg)
{
	Client *client;

	if (this->clients.empty())
		return ;
	for (std::map<const int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		client = it->second;
		if (client == sender)
			continue ;
		client->appendSendBuffer(msg);
	}

}

std::string			Channel::getNameList(void) const
{
	Client		*client;
	std::string	res;
	std::string user;

	for (std::map<const int, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		client = it->second;
		if (this->isOperator(client->getFd()))
			user = "@";
		user = user + client->getNick();
		res = res + " " + user;
		user.clear();
	}

	return (res);
}

std::ostream &operator<<(std::ostream &stream, Channel const &channel)
{
	std::cout << "===== CHANNEL : " << channel.getName() << " =====" << std::endl;
	std::cout << "USERS : " << channel.getNameList() << std::endl;

	return (stream);
}
