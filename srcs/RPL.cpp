/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:05:27 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"
# include "Errors.hpp"

static std::map<std::string, void (*)(Server &)> initRpls()
{
    std::map<std::string, void (*)(Server &)> m;

	m[CMD_QUIT] = &RPL::Quit;
	m[CMD_NICK] = &RPL::Nick;
	m[CMD_JOIN] = &RPL::Join;
	m[CMD_PRIV] = &RPL::Priv;
	m[CMD_KICK] = &RPL::Kick;
	m[CMD_TOPIC] = &RPL::Topic;
	m[CMD_INVITE] = &RPL::Invite;
	m[CMD_MODE] = &RPL::Mode;

    return m;
}

std::map<std::string, void (*)(Server &)> rpls = initRpls();

static std::string	getRPL(void)									{ return (RPL_STR(package.oldClient.getNick(), package.client->getUser(), package.cmd, package.rplData)); };

void RPL::Welcome(Client *client, std::string const &nick)
{
	std::string msg;

	msg = HEADER_STR("001", nick, "", "") + RPL_WELCOME_STR(nick) + "\r\n";
	client->appendSendBuffer(msg);
	msg = HEADER_STR("002", nick, "", "") + RPL_YOURHOST_STR + "\r\n";
	client->appendSendBuffer(msg);
	msg = HEADER_STR("003", nick, "", "") + RPL_CREATED_STR(Ft::getTime()) + "\r\n";
	client->appendSendBuffer(msg);
	msg = HEADER_STR("004", nick, "", "") + RPL_MYINFO_STR(nick) + "\r\n";
	client->appendSendBuffer(msg);
	msg = HEADER_STR("005", nick, "", "") + RPL_ISUPPORT_STR + "\r\n";
	client->appendSendBuffer(msg);
}

void RPL::Quit(Server &server)
{
	(void) server;

	std::string	msg = getRPL();
	msg = msg + ((package.cmdData[QUIT_REASON].empty())
		? RPL_QUIT(package.cmdData[QUIT_REASON])
		: RPL_QUIT(DEFAULT_QUIT_MSG));
	msg = msg + "\r\n";
	
	std::cout << "[RPL] " << msg << std::endl;
	package.client->broadcastMsg(msg);
}

void	RPL::Default(Server &server)
{
	(void) server;
	std::string	msg = getRPL() + "\r\n";

	package.client->appendSendBuffer(msg);
}

void	RPL::Error(Server &server)
{
	(void) server;
	std::string	msg;

	msg = getError(package.error, package.client->getNick()) + "\r\n";
	std::cout << "[RPL][ERROR] " << msg;
	package.client->appendSendBuffer(msg);
}

void	RPL::Nick(Server &server)
{
	(void) server;
	std::string msg = getRPL() + RPL_NICK(package.client->getNick()) + "\r\n";

	package.client->appendSendBuffer(msg);
}

bool shouldTriggerFeurBot(const std::string &message)
{
    if (!package.channel || message.size() < 4)
        return false;
    
    std::string lastFourChars = message.substr(message.length() - 4);
    return (lastFourChars == "quoi");
}

void sendFeurBotResponse()
{
    std::string botMessage = RPL_STR("Feur-bot", "Feur-bot", package.cmd, package.rplData) 
                           + RPL_PRIV("feur") + "\r\n";
    
    package.channel->broadcastMessage(package.client, botMessage);
	package.client->appendSendBuffer(botMessage);
}

void RPL::Priv(Server &server)
{
    std::string messageContent = package.cmdData[PRIV_MSG];
    std::string recipient = package.rplData;
    
    std::string formattedMessage = getRPL() + RPL_PRIV(messageContent) + "\r\n";
    
    bool isChannelMessage = (recipient[0] == '#');
    
    if (isChannelMessage)
        package.channel->broadcastMessage(package.client, formattedMessage);
    else
    {
        Client *receiver = server.getClient(recipient);
		receiver->appendSendBuffer(formattedMessage);
    }
    
    if (shouldTriggerFeurBot(messageContent))
        sendFeurBotResponse();
}

void	RPL::Kick(Server &server)
{
	if (!package.channel)
		return ;
	Client		*client =	server.getClient(package.cmdData[KICK_USER]);
	std::string	msg =		getRPL() + RPL_KICK(package.cmdData[KICK_USER], package.cmdData[KICK_MSG]) + "\r\n";

	client->appendSendBuffer(msg);
	package.client->appendSendBuffer(msg);
	package.channel->broadcastMessage(package.client, msg);
}

void	RPL::Join(Server &server)
{
	(void) server;
	std::string msg;
	std::string clientNick = package.client->getNick();
	std::string channelJoin = package.cmdData[JOIN_CHANNEL];

	msg = RPL_STR(clientNick, package.client->getUser(), package.cmd, "") + RPL_JOIN(channelJoin) + "\r\n";
	package.client->appendSendBuffer(msg);
	package.channel->broadcastMessage(package.client, msg);
	msg = (channelJoin.empty())
		? HEADER_STR("332", clientNick, " ", channelJoin) + RPL_NOTOPIC
		: HEADER_STR("332", clientNick, " ", channelJoin) + RPL_TOPIC(package.rplData);
	msg = msg + "\r\n";
	std::cout << "[RPL] " << msg;
	package.client->appendSendBuffer(msg);
	msg = HEADER_STR("353", clientNick, " = ", channelJoin) + package.channel->getNameList() + "\r\n";
	package.client->appendSendBuffer(msg);
	msg = HEADER_STR("366", clientNick, " ", channelJoin) + RPL_ENDOFNAMES + "\r\n";
	package.client->appendSendBuffer(msg);
}

void	RPL::Topic(Server &server)
{
	(void) server;
	std::string	msg;

	msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmdData[TOPIC_CHANNEL])
		+ RPL_TOP(package.cmdData[TOPIC_NEW]) + "\r\n";
	package.client->appendSendBuffer(msg);
	if (package.channel)
		package.channel->broadcastMessage(package.client, msg);
}

void	RPL::Invite(Server &server)
{
	(void) server;
	std::string msg;

	msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmdData[INVITE_NICK])
		+ RPL_INVITE(package.channel->getName()) + "\r\n";
	std::cout << "[RPL] " << msg;
	server.getClient(package.cmdData[INVITE_NICK])->appendSendBuffer(msg);
	msg = HEADER_ERROR("341", package.client->getNick()) + package.cmdData[INVITE_NICK] + " " + package.cmdData[INVITE_CHANNEL] + "\r\n";
	package.client->appendSendBuffer(msg);
	std::cout << "[RPL] " << msg << std::endl;
}

void	RPL::Mode(Server &server)
{
	(void) server;
	std::string	msg;

	if (package.error < 0)
		return ;
	else if (package.cmdData.size() == 1)
		msg =  HEADER_STR("324", package.client->getNick(), " ", package.channel->getName()) + package.channel->getModes() + "\r\n";
	else
		msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.channel->getName())
			+ RPL_MODE(package.cmdData[MODE_MODES]) + "\r\n";
	std::cout << "[RPL] " << msg;
	package.client->appendSendBuffer(msg);
	if (package.channel)
		package.channel->broadcastMessage(package.client, msg);

}

void	RPL::reply(Server &server)
{
	std::map<std::string, void (*)(Server &server)>::iterator it;

	if (package.error > 0)
		return (RPL::Error(server));

	it = rpls.find(package.cmd);
	if (it == rpls.end())
		return ;
	it->second(server);
}
