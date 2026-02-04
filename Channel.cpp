/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:48:22 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/03 18:19:36 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void) {};
Channel::~Channel(void) {};
Channel::Channel(Channel const &copy) { (*this) = copy; };
Channel::Channel(const std::string& name): name(name) {};
Channel	&Channel::operator=(Channel const &copy) { this->name = copy.name; this->clients = copy.clients; return (*this); };

const std::string& Channel::getName(void) const { return (this->name); };
void Channel::addClient(std::string const &name, Client &client) { this->clients[name] = client; };
bool Channel::removeClient(std::string const &name) { return (this->clients.erase(name)); };
