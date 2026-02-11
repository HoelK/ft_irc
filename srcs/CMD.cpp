/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/11 20:49:24 by hkeromne         ###   ########.fr       */
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
	{CMD_TOPIC, &CMD::Topic},
	{CMD_INVITE, &CMD::Invite}

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
	if (package.cmd_data.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS));
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED));
	package.client->setPass(package.cmd_data[PASS_PASS]);
}

void	CMD::Nick(Server &server)
{
	std::string	nick = package.cmd_data[NICK_NICK];

	package.rpl_data = nick;
	if (package.cmd_data.size() < 2)
		return (package.setError(ERR_NONICKNAMEGIVEN));
	if (!Ft::isValidNickname(nick))
		return (package.setError(ERR_ONEUSNICKNAME));
	if (server.isClient(nick))
		return (package.setError(ERR_NICKNAMEINUSE));
	package.client->setNick(nick);
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
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED));
	if (package.cmd_data.size() < 5)
		return (package.setError(ERR_NEEDMOREPARAMS));
	package.client->setUser(package.cmd_data[USER_USERNAME]);
	package.client->setName(package.cmd_data[USER_REALNAME]);
}

void	CMD::Join(Server &server)
{
	Channel		channel;
	std::string	joinChannel;

	if (package.cmd_data.size() < JOIN_CHANNEL)
		return (package.error = ERR_NEEDMOREPARAMS, (void)channel);
	package.rpl_data	= package.cmd_data[JOIN_CHANNEL];
	joinChannel			= package.rpl_data;

	if (server.isChannel(joinChannel))
	{
		Channel *ch = server.getChannel(joinChannel);

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
	channel.setName(joinChannel);
	channel.addClient(package.client);
	server.createChannel(channel);
	package.client->addChannel(server.getChannel(joinChannel));
	package.channel = package.client->getChannel(joinChannel);
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

	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS));

	std::string	kick_user		= package.cmd_data[KICK_USER];
	std::string	kick_channel	= package.cmd_data[KICK_CHANNEL];
	if (!package.client->isChannel(kick_channel))
		return (package.setError(ERR_NOSUCHCHANNEL));
	Channel *channel	= package.client->getChannel(kick_channel);
	package.channel		= channel;

	if (!channel->isClient(kick_user))
	{
		package.rpl_data = kick_user;
		return (package.setError(ERR_USERNOTINCHANNEL));
	}
	Client	*client		= channel->getClient(kick_user);
	package.channel		= channel;
	package.rpl_data	= kick_channel;
	channel->removeClient(client->getNick());
}

void	CMD::Topic(Server &server)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS));
	std::string	name		= package.cmd_data[TOPIC_CHANNEL];
	std::string newTopic	= package.cmd_data[TOPIC_NEW];
	Channel		*channel	= server.getChannel(name);

	channel->setTopic(newTopic);
	package.channel		= channel;
	package.rpl_data	= newTopic;
}

void	CMD::Invite(Server &server)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS));
	std::string	invited = package.cmd_data[INVITE_NICK];
	std::string	chan_name = package.cmd_data[INVITE_CHANNEL];
	if (!server.isClient(invited))
		return (package.setError(ERR_NOSUCHNICK));
	if (!server.isChannel(chan_name))
		return (package.setError(ERR_NOSUCHCHANNEL));
	Channel		*channel	= server.getChannel(chan_name);
	package.channel			= channel;
	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL));
	if (channel->isClient(invited))
		return (package.setError(ERR_USERONCHANNEL));
	Client *invitedCli = server.getClient(invited);
	channel->addInvited(invitedCli);
}
