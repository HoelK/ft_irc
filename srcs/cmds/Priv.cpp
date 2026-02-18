/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Priv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:43:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:08:32 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Priv.hpp"
# include "Errors.hpp"

static bool checkChannelSend(Server &server, std::string const &target)
{
	package.errChanName = target;
	if (!server.isChannel(target))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel *channel =	server.getChannel(target);
	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL), false);

	return (true);
}

static bool checkClientSend(Server &server, std::string const &target)
{
	package.errNick = target;
	if (!server.isClient(target))
		return (package.setError(ERR_NOSUCHNICK), false);
	return (true);
}

bool Priv::Check(Server &server)
{
	package.errNick	=	package.client->getNick();
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string target = package.cmdData[PRIV_TARGET];
	if (target[0] == '#' && !checkChannelSend(server, target))
		return (false);
	else if (target[0] != '#' && !checkClientSend(server, target))
		return (false);
	return (true);
}

void Priv::Send(Server &server)
{
	std::string target =	package.cmdData[PRIV_TARGET];
	Channel	*channel =		server.getChannel(target);

	if (target[0] == '#' && channel)
		package.channel = channel;
}
