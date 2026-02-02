/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:48:22 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/02 05:00:26 by sbonneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string& name): _name(name), _userLimit(-1),  _inviteOnly(false), _topicRestricted(true)
{
    std::cout << "[Channel] New Channel instance : " << name << std::endl;
}

Channel::~Channel(void)
{
    std::cout << "[Channel] Channel destroyed : " << this->_name << std::endl;
}

const std::string& Channel::getName(void) const
{
    return (this->_name);
}

bool Channel::isMember(Client* c) const
{
    if (!c)
        return (false);
    return (this->_clients.find(c) != this->_clients.end());
}

bool Channel::hasClient(Client* c) const
{
    return (this->isMember(c));
}

bool Channel::isOperator(Client* c) const
{
    if (!c)
        return (false);
    return (this->_operators.find(c) != this->_operators.end());
}

void Channel::addClient(Client* c)
{
    if (c)
        this->_clients.insert(c);
}

void Channel::removeClient(Client* c)
{
    if (c)
    {
        this->_clients.erase(c);
        this->_operators.erase(c);
    }
}

void Channel::addOperator(Client* c)
{
    if (c && this->isMember(c))
        this->_operators.insert(c);
}

void Channel::removeOperator(Client* c)
{
    if (c)
        this->_operators.erase(c);
}

void Channel::setTopic(const std::string& t)
{
    this->_topic = t;
}

const std::string& Channel::getTopic() const
{
    return (this->_topic);
}

void Channel::setKey(const std::string& k)
{
    this->_key = k;
}

const std::string& Channel::getKey() const
{
    return (this->_key);
}

void Channel::setInviteOnly(bool v)
{
    this->_inviteOnly = v;
}

void Channel::setTopicRestricted(bool v)
{
    this->_topicRestricted = v;
}

bool Channel::isTopicRestricted(void) const
{
    return (this->_topicRestricted);
}

void Channel::setUserLimit(int limit)
{
    this->_userLimit = limit;
}

bool Channel::canJoin(Client* c, const std::string& key) const
{
    if (!c)
        return (false);
    
    if (this->_inviteOnly)
    {
        if (this->_invited.find(c->getNick()) == this->_invited.end())
            return (false);
    }
    
    if (!this->_key.empty() && this->_key != key)
        return (false);
    
    if (this->_userLimit > 0 && 
        static_cast<int>(this->_clients.size()) >= this->_userLimit)
        return (false);
    
    return (true);
}

void Channel::invite(const std::string& nick)
{
    this->_invited.insert(nick);
}

std::string Channel::getModes() const
{
    std::string modes = "+";
    
    if (this->_inviteOnly)
        modes += "i";
    if (this->_topicRestricted)
        modes += "t";
    if (!this->_key.empty())
        modes += "k";
    if (this->_userLimit > 0)
        modes += "l";
    
    return (modes);
}

