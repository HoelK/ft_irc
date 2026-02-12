/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:48:22 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/12 20:43:53 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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
void				Channel::setOpKey(bool const &key, std::string const &password) { this->opKey = key; this->password = password; };
void				Channel::setOpTopic(bool const &topic)		{ this->opTopic = topic; };
void				Channel::setOpInvite(bool const &invite)	{ this->opInvite = invite; };
void				Channel::setOpLimit(int const &limit)		{ this->opLimit = limit; };
void				Channel::setName(std::string const &name)	{ this->name = name; };
void				Channel::setTopic(std::string const &topic)	{ this->topic = topic; };

bool				Channel::isFull(void) { return (this->opLimit && (int)this->clients.size() >= this->opLimit); }
bool				Channel::Auth(std::string const &password) { return (this->password == password); };

std::string			Channel::getModes(void)
{
	std::string		result;
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

void				Channel::addInvited(Client *client) { this->invited.push_back(client); };
void				Channel::removeInvited(std::string const &nick)
{ 
	for (std::vector<Client *>::iterator it = this->invited.begin(); it != this->invited.end(); it++)
	{
		Client *client = *it;
		if (client->getNick() == nick)
			this->invited.erase(it);
	}
}

bool				Channel::isInvited(std::string const &nick)
{
	for (std::vector<Client *>::iterator it = this->invited.begin(); it != this->invited.end(); it++)
	{
		Client *client = *it;
		if (client->getNick() == nick)
			return (true);
	}
	return (false);
}

void				Channel::addClient(Client *client) { this->clients[client->getNick()] = client; };
bool				Channel::removeClient(std::string const &name) { return (this->clients.erase(name)); };
void				Channel::broadcastMessage(Client *sender, std::string const &msg)
{
	Client *client;

	if (this->clients.empty())
		return ;
	for (std::map<std::string, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		client = it->second;
		if (client == sender)
			continue ;
		send(client->getFd(), msg.c_str(), msg.size(), 0);
	}

}

std::string			Channel::getNameList(void) const
{
	Client		*client;
	std::string	res;
	std::string user;

	for (std::map<std::string, Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		client = it->second;
		if (client->getOp())
			user = "@";
		user = user + client->getNick();
		res = res + " " + user;
		user.clear();
	}

	return (res);
}


bool	Channel::isClient(std::string const &nick) { return (this->clients.find(nick) != this->clients.end()); };
Client	*Channel::getClient(std::string const &nick) { return (this->clients.find(nick)->second); };

std::ostream &operator<<(std::ostream &stream, Channel const &channel)
{
	std::cout << "===== CHANNEL : " << channel.getName() << " =====" << std::endl;
	std::cout << "USERS : " << channel.getNameList() << std::endl;

	return (stream);
}
