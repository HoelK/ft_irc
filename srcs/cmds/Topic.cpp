/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:43:56 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 21:11:42 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Topic.hpp"
# include "RPL.hpp"

bool Topic::Check(Server &server)
{
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	topicChan =	package.cmdData[TOPIC_CHANNEL];
	package.errNick =		package.client->getNick();
	package.errChanName =	topicChan;

	if (!server.isChannel(topicChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	Channel *channel = server.getChannel(topicChan);

	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (channel->getOpTopic()
	&& !channel->isOperator(package.client->getNick()))
		return (package.setError(ERR_CHANOPRIVSNEEDED), false);

	return (true);
}

void Topic::Set(Server &server)
{
	std::string	topicChan	= package.cmdData[TOPIC_CHANNEL];
	std::string newTopic	= package.cmdData[TOPIC_NEW];
	Channel		*channel	= server.getChannel(topicChan);

	channel->setTopic(newTopic);
	package.channel	= channel;
	package.rplData	= newTopic;
}
