/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:42:58 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:08:17 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Kick.hpp"
# include "Errors.hpp"

bool Kick::Check(Server &server)
{
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	
	std::string	kickNick =	package.cmdData[KICK_USER];
	std::string kickChan =	package.cmdData[KICK_CHANNEL];
	package.errNick =		package.client->getNick();
	package.errChanName =	kickChan;

	if (!server.isChannel(kickChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel		*channel =	server.getChannel(kickChan);

	if (!channel->isClient(package.client->getFd()))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (!channel->isOperator(package.client->getFd()))
		return (package.setError(ERR_CHANOPRIVSNEEDED), false);
	package.errNick = kickNick;
	if (!server.isClient(kickNick))
		return (package.setError(ERR_NOSUCHNICK), false);
	package.channel = channel;
	if (!channel->isClient(server.getClient(kickNick)->getFd()))
		return (package.setError(ERR_USERNOTINCHANNEL), false);
	return (true);
}

void Kick::Kicking(Server &server)
{
	std::string	kickNick =		package.cmdData[KICK_USER];
	std::string	kickChannel	=	package.cmdData[KICK_CHANNEL];
	package.channel =			package.client->getChannel(kickChannel);
	Client	*target	=			server.getClient(kickNick);

	package.channel->removeClient(target->getFd());
	target->delChannel(package.channel->getName());
}
