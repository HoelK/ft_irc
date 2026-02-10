/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:48:22 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/10 00:36:33 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void) {};
Channel::~Channel(void) {};
Channel::Channel(Channel const &copy) { (*this) = copy; };
Channel::Channel(const std::string& name): name(name) {};
Channel	&Channel::operator=(Channel const &copy) { this->name = copy.name; this->clients = copy.clients; return (*this); };

void				Channel::setName(std::string const &name) { this->name = name; };
const std::string&	Channel::getName(void) const { return (this->name); };
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


bool Channel::isClient(std::string const &nick) { return (this->clients.find(nick) != this->clients.end()); };
Client	*Channel::getClient(std::string const &nick) { return (this->clients.find(nick)->second); };


std::ostream &operator<<(std::ostream &stream, Channel const &channel)
{
	std::cout << "===== CHANNEL : " << channel.getName() << " =====" << std::endl;
	std::cout << "USERS : " << channel.getNameList() << std::endl;

	return (stream);
}
