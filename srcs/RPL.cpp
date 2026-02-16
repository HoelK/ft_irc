/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 16:12:54 by dedavid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"

static std::map<std::string, void (*)(Server &)> initRpls()
{
    std::map<std::string, void (*)(Server &)> m;

	m[CMD_QUIT] = &RPL::Default;
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

static std::string	codeStr(short code)
{
	std::stringstream	s;
	std::string			res;

	s << code;
	s >> res;
	if (code < 10)
		res = "00" + s.str();
	else if (code < 100)
		res = "0" + s.str();

	return (res);
}

static std::string	codeToErr(short code)
{
	if (code == ERR_PASSWDMISMATCH)
		return (ERR_PASSWDMISMATCH_STR);
	if (code == ERR_NEEDMOREPARAMS)
		return (ERR_NEEDMOREPARAMS_STR(package.cmd));
	if (code == ERR_USERNOTINCHANNEL)
		return (ERR_USERNOTINCHANNEL_STR(package.errNick, package.errChanName));
	if (code == ERR_NOSUCHCHANNEL)
		return (ERR_NOSUCHCHANNEL_STR(package.errChanName));
	if (code == ERR_NOSUCHNICK)
		return (ERR_NOSUCHNICK_STR(package.errNick));
	if (code == ERR_USERONCHANNEL)
		return (ERR_USERONCHANNEL_STR(package.errNick, package.errChanName));
	if (code == ERR_NONICKNAMEGIVEN)
		return (ERR_NONICKNAMEGIVEN_STR);
	if (code == ERR_ONEUSNICKNAME)
		return (ERR_ONEUSNICKNAME_STR(package.rplData));
	if (code == ERR_NICKNAMEINUSE)
		return (ERR_NICKNAMEINUSE_STR(package.rplData));
	if (code == ERR_NOTONCHANNEL)
		return (ERR_NOTONCHANNEL_STR(package.errChanName));
	if (code == ERR_ALREADYREGISTRED)
		return (ERR_ALREADYREGISTRED_STR);
	if (code == ERR_CHANNELISFULL)
		return (ERR_CHANNELISFULL_STR(package.channel->getName()));
	if (code == ERR_INVITEONLYCHAN)
		return (ERR_INVITEONLYCHAN_STR(package.channel->getName()));
	if (code == ERR_BADCHANNELKEY)
		return (ERR_BADCHANNELKEY_STR(package.channel->getName()));
	if (code == ERR_CHANOPRIVSNEEDED)
		return (ERR_CHANOPRIVSNEEDED_STR(package.errChanName));
	
	return ("");
}

static std::string	getError(short code, std::string const &nick)	{ return (HEADER_ERROR(codeStr(code), nick) + codeToErr(code)); };
static std::string	getRPL(void)									{ return (RPL_STR(package.oldClient.getNick(), package.client->getUser(), package.cmd, package.rplData)); };

void RPL::Welcome(const int &fd, std::string const &nick)
{
	std::string msg;

	msg = HEADER_STR("001", nick, "", "") + RPL_WELCOME_STR(nick) + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("002", nick, "", "") + RPL_YOURHOST_STR + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("003", nick, "", "") + RPL_CREATED_STR + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("004", nick, "", "") + RPL_MYINFO_STR(nick) + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("005", nick, "", "") + RPL_ISUPPORT_STR + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

void	RPL::Default(Server &server)
{
	(void) server;
	std::string	msg = getRPL() + "\r\n";

	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Error(Server &server)
{
	(void) server;
	std::string	msg;

	msg = getError(package.error, package.client->getNick()) + "\r\n";
	std::cout << "[RPL][ERROR] " << msg;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Nick(Server &server)
{
	(void) server;
	std::string msg = getRPL() + RPL_NICK(package.client->getNick()) + "\r\n";

	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
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
    send(package.client->getFd(), botMessage.c_str(), botMessage.size(), 0);
}

void RPL::Priv(Server &server)
{
    std::string messageContent = package.cmdData[PRIV_MSG];
    std::string recipient = package.rplData;
    
    std::string formattedMessage = getRPL() + RPL_PRIV(messageContent) + "\r\n";
    
    bool isChannelMessage = (recipient[0] == '#');
    
    if (isChannelMessage)
    {
        package.channel->broadcastMessage(package.client, formattedMessage);
    }
    else
    {
        Client *receiver = server.getClient(recipient);
        {
            send(receiver->getFd(), formattedMessage.c_str(), formattedMessage.size(), 0);
        }
    }
    
    if (shouldTriggerFeurBot(messageContent))
    {
        sendFeurBotResponse();
    }
}

void	RPL::Kick(Server &server)
{
	if (!package.channel)
		return ;
	Client		*client =	server.getClient(package.cmdData[KICK_USER]);
	std::string	msg =		getRPL() + RPL_KICK(package.cmdData[KICK_USER], package.cmdData[KICK_MSG]) + "\r\n";

	send(client->getFd(), msg.c_str(), msg.size(), 0);
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	package.channel->broadcastMessage(package.client, msg);
}

void	RPL::Join(Server &server)
{
	(void) server;
	std::string msg;
	int			clientFd = package.client->getFd();
	std::string clientNick = package.client->getNick();
	std::string channelJoin = package.cmdData[JOIN_CHANNEL];

	msg = RPL_STR(clientNick, package.client->getUser(), package.cmd, "") + RPL_JOIN(channelJoin) + "\r\n";
	send(clientFd, msg.c_str(), msg.size(), 0);
	package.channel->broadcastMessage(package.client, msg);
	msg = (channelJoin.empty())
		? HEADER_STR("332", clientNick, " ", channelJoin) + RPL_NOTOPIC
		: HEADER_STR("332", clientNick, " ", channelJoin) + RPL_TOPIC(package.rplData);
	msg = msg + "\r\n";
	std::cout << "[RPL] " << msg;
	send(clientFd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("353", clientNick, " = ", channelJoin) + package.channel->getNameList() + "\r\n";
	send(clientFd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("366", clientNick, " ", channelJoin) + RPL_ENDOFNAMES + "\r\n";
	send(clientFd, msg.c_str(), msg.size(), 0);
}

void	RPL::Topic(Server &server)
{
	(void) server;
	std::string	msg;

	msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmdData[TOPIC_CHANNEL])
		+ RPL_TOP(package.cmdData[TOPIC_NEW]) + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
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
	send(server.getClient(package.cmdData[INVITE_NICK])->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_ERROR("341", package.client->getNick()) + package.cmdData[INVITE_NICK] + " " + package.cmdData[INVITE_CHANNEL] + "\r\n";
	std::cout << "[RPL] " << msg << std::endl;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Mode(Server &server)
{
	(void) server;
	std::string	msg;

	if (package.error < 0)
		return ;
	if (package.cmdData.size() == 2)
		msg = package.channel->getModes() + "\r\n";
	else
		msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.channel->getName())
			+ RPL_MODE(package.cmdData[MODE_MODES]) + "\r\n";
	std::cout << "[RPL] " << msg;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
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
