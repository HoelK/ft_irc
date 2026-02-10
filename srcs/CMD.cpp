/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/10 22:37:07 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD.hpp"

static std::map<std::string, void (*)(Server &server)> cmds = {
	{CMD_NICK, &CMD::Nick},
	{CMD_USER, &CMD::User},
	{CMD_PASS, &CMD::Pass},
	{CMD_QUIT, &CMD::Quit},
	{CMD_JOIN, &CMD::Join},
	{CMD_PRIV, &CMD::Priv},
	{CMD_KICK, &CMD::Kick},
	{CMD_TOPIC, &CMD::Topic}
};

void	CMD::apply(Server &server)
{
	std::map<std::string, void (*)(Server &server)>::iterator it;

	it = cmds.find(package.cmd);
	if (it == cmds.end())
		return ;
	it->second(server);
}

void	CMD::Pass(Server &server)
{
	(void) server;
	package.client->setPass(package.cmd_data[PASS_PASS]);
}

void	CMD::Nick(Server &server)
{
	(void) server;
	package.client->setNick(package.cmd_data[NICK_NICK]);
	package.rpl_data = package.client->getNick();
}

void	CMD::Quit(Server &server)
{
	(void) server;
	package.quit = true;
	package.rpl_data = package.cmd_data[QUIT_REASON];
}

void	CMD::User(Server &server)
{
	(void) server;
	package.client->setUser(package.cmd_data[USER_USERNAME]);
	package.client->setName(package.cmd_data[USER_REALNAME]);
}

void	CMD::Join(Server &server)
{
	Channel	channel;

	if (package.cmd_data.size() < JOIN_CHANNEL)
		return (package.error = ERR_NEEDMOREPARAMS, (void)channel);
	package.rpl_data = package.cmd_data[JOIN_CHANNEL];

	if (server.isChannel(package.cmd_data[JOIN_CHANNEL]))
	{
		Channel *ch = server.getChannel(package.cmd_data[JOIN_CHANNEL]);

		if (ch->getOpInvite())
			return (package.setError(ERR_NEEDMOREPARAMS));
		else if (ch->isFull())
			return (package.setError(ERR_CHANNELISFULL));
		else if (ch->getOpKey() && !ch->Auth(package.cmd_data[JOIN_KEY]))
			return (package.setError(ERR_BADCHANNELKEY));

		ch->addClient(package.client);
		package.client->addChannel(ch);
		package.channel = ch;

		return ;
	}
	package.client->setOp(true);
	channel.setName(package.cmd_data[JOIN_CHANNEL]);
	channel.addClient(package.client);
	server.createChannel(channel);
	package.client->addChannel(server.getChannel(package.cmd_data[JOIN_CHANNEL]));
	package.channel = package.client->getChannel(package.cmd_data[JOIN_CHANNEL]);
}

void	CMD::Priv(Server &server)
{
	(void) server;
	package.rpl_data = package.cmd_data[PRIV_TARGET];
	Channel	*channel = server.getChannel(package.rpl_data);
	if (package.rpl_data[0] == '#' && channel)
		package.channel = channel;

}

void	CMD::Kick(Server &server)
{
	(void) server;

	if (!package.client->isChannel(package.cmd_data[KICK_CHANNEL]))
		return (package.setError(ERR_NOSUCHCHANNEL));
	Channel *channel =	package.client->getChannel(package.cmd_data[KICK_CHANNEL]);
	package.channel = channel;
	if (package.cmd_data[KICK_USER].empty())
		return (package.setError(ERR_NEEDMOREPARAMS));
	else if (!channel->isClient(package.cmd_data[KICK_USER]))
	{
		package.rpl_data = package.cmd_data[KICK_USER];
		return (package.setError(ERR_USERNOTINCHANNEL));
	}
	Client	*client =	channel->getClient(package.cmd_data[KICK_USER]);
	package.channel = channel;
	
	package.rpl_data = package.cmd_data[KICK_CHANNEL];
	channel->removeClient(client->getNick());
}

void	CMD::Topic(Server &server)
{
	if (package.cmd_data.size() == 3)
	{
		std::string	name		= package.cmd_data[TOPIC_CHANNEL];
		Channel		*channel	= server.getChannel(name);

		channel->setTopic(package.cmd_data[TOPIC_NEW]);
		package.channel = channel;
		package.rpl_data = package.cmd_data[TOPIC_NEW];
	}
}
