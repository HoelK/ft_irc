/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:43:32 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/19 22:51:33 by dedavid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Join.hpp"
# include "Errors.hpp"

static bool checkInvite(Channel *channel)
{
	if (!channel->getOpInvite())
		return (true);
	if (!channel->isInvited(package.client->getFd()))
		return (package.setError(ERR_INVITEONLYCHAN), false);
	return (true);
}

static bool	checkKey(Channel *channel)
{
	if (!channel->getOpKey())
		return (true);
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_BADCHANNELKEY), false);
	if (!channel->Auth(package.cmdData[JOIN_KEY]))
		return (package.setError(ERR_BADCHANNELKEY), false);
	return (true);
}

bool	Join::Check()
{
	if (package.cmdData.size() < 1)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	std::string chanName = package.cmdData[JOIN_CHANNEL];
	package.errChanName = chanName;
	if (!Ft::isValidChannelName(chanName))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	return (true);
}

void	Join::Joining(Server &server)
{
	Channel		*channel;
	std::string	joinChannel;

	joinChannel	= package.cmdData[JOIN_CHANNEL];
	channel		= server.getChannel(joinChannel);
	package.channel = channel;

	if (channel->isFull())
		return (package.setError(ERR_CHANNELISFULL));
	if (!checkInvite(channel) || !checkKey(channel))
		return ;
	channel->addClient(package.client);
	package.client->addChannel(channel);
}

void	Join::Create(Server &server)
{
	Channel		channel;
	std::string	joinChannel = package.cmdData[JOIN_CHANNEL];

	channel.addOperator(package.client);
	channel.setName(joinChannel);
	channel.addClient(package.client);
	server.createChannel(channel);
	package.client->addChannel(server.getChannel(joinChannel));
	package.channel = package.client->getChannel(joinChannel);
}
