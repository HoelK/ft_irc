/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/10 03:06:25 by hkeromne         ###   ########.fr       */
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
	Channel channel;

	package.rpl_data = package.cmd_data[JOIN_CHANNEL];
	if (server.isChannel(package.cmd_data[JOIN_CHANNEL]))
	{
		Channel *ch = server.getChannel(package.cmd_data[JOIN_CHANNEL]);
		ch->addClient(package.client);
		package.client->setChannel(ch);
		return ;
	}
	package.client->setOp(true);
	channel.setName(package.cmd_data[JOIN_CHANNEL]);
	channel.addClient(package.client);
	server.createChannel(channel);
	package.client->setChannel(server.getChannel(package.cmd_data[JOIN_CHANNEL]));
}

void	CMD::Priv(Server &server)
{
	(void) server;
	package.rpl_data = package.cmd_data[PRIV_TARGET];
}

void	CMD::Kick(Server &server)
{
	(void) server;

	Channel *channel =	package.client->getChannel();
	Client	*client =	channel->getClient(package.cmd_data[KICK_USER]);
	
	if (!channel || !client)
		return ;
	package.rpl_data = package.cmd_data[KICK_CHANNEL];
	if (channel->getName() == package.cmd_data[KICK_CHANNEL]
		&& channel->isClient(package.cmd_data[KICK_USER]))
	{
		channel->removeClient(client->getNick());
	}
}

void	CMD::Topic(Server &server)
{
	if (package.cmd_data.size() == 2)
	{
		std::string	name = package.cmd_data[TOPIC_CHANNEL];
		Channel		*channel = server.getChannel(name);
		channel->setName(name);
		server.deleteChannel(channel->getName());
		server.createChannel(*channel);
	}
}
