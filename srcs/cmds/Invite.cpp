/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:44:04 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:08:02 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Invite.hpp"
# include "Errors.hpp"

bool	Invite::Check(Server &server)
{
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	invited =	package.cmdData[INVITE_NICK];
	std::string	chanName =	package.cmdData[INVITE_CHANNEL];
	package.errChanName = chanName;
	if (!server.isChannel(chanName))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel *channel = server.getChannel(chanName);
	package.errNick = package.client->getNick();
	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (!channel->isOperator(package.client->getNick()))
		return (package.setError(ERR_CHANOPRIVSNEEDED), false);
	package.errNick = invited;
	if (!server.isClient(invited))
		return (package.setError(ERR_NOSUCHNICK), false);
	if (channel->isClient(invited))
		return (package.setError(ERR_USERONCHANNEL), false);
	return (true);
}

void	Invite::Send(Server &server)
{
	std::string	invited =	package.cmdData[INVITE_NICK];
	package.channel	=		server.getChannel(package.cmdData[INVITE_CHANNEL]);

	package.channel->addInvited(server.getClient(invited));
}
