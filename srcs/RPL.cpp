/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/11 20:31:56 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"

static std::map<std::string, void (*)(Server &server)> rpls = {
	{CMD_NICK, &RPL::Default},
	{CMD_QUIT, &RPL::Default},
	{CMD_JOIN, &RPL::Join},
	{CMD_PRIV, &RPL::Priv},
	{CMD_KICK, &RPL::Kick},
	{CMD_TOPIC, &RPL::Topic},
	{CMD_INVITE, &RPL::Invite}
};

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
		return (ERR_USERNOTINCHANNEL_STR(package.rpl_data, package.channel->getName()));
	if (code == ERR_NOSUCHCHANNEL)
		return (ERR_NOSUCHCHANNEL_STR(package.rpl_data));
	if (code == ERR_NONICKNAMEGIVEN)
		return (ERR_NONICKNAMEGIVEN_STR);
	if (code == ERR_ONEUSNICKNAME)
		return (ERR_ONEUSNICKNAME_STR(package.rpl_data));
	if (code == ERR_NICKNAMEINUSE)
		return (ERR_NICKNAMEINUSE_STR(package.rpl_data));
	if (code == ERR_NOTONCHANNEL)
		return (ERR_NOTONCHANNEL_STR(package.channel->getName()));
	if (code == ERR_ALREADYREGISTRED)
		return (ERR_ALREADYREGISTRED_STR);
	if (code == ERR_CHANNELISFULL)
		return (ERR_CHANNELISFULL_STR(package.channel->getName()));
	if (code == ERR_INVITEONLYCHAN)
		return (ERR_INVITEONLYCHAN_STR(package.channel->getName()));
	if (code == ERR_BADCHANNELKEY)
		return (ERR_BADCHANNELKEY_STR(package.channel->getName()));
	if (code == ERR_CHANOPRIVSNEEDED)
		return (ERR_CHANOPRIVSNEEDED_STR(package.channel->getName()));
	
	return ("");
}

static std::string	getError(short code, std::string const &nick) { return (HEADER_ERROR(codeStr(code), nick) + codeToErr(code)); };
static std::string	getRPL(void) { return (RPL_STR(package.oldClient.getNick(), package.client->getUser(), package.cmd, package.rpl_data)); };

void RPL::Welcome(const int &fd, std::string const &nick)
{
	std::string msg;

	std::cout << "Connected" << std::endl;
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

void	RPL::Error(Server &server)
{
	(void) server;
	std::string	msg;

	msg = getError(package.error, package.client->getNick()) + "\r\n";
	std::cout << msg;
	std::cout << *package.client;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Default(Server &server)
{
	(void) server;
	std::string	msg = getRPL() + "\r\n";

	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Priv(Server &server)
{
	std::string	msg;
	Client		*receiver;

	msg = getRPL() + RPL_PRIV(package.cmd_data[PRIV_MSG]) + "\r\n";

	receiver = server.getClient(package.rpl_data);
	if (package.rpl_data[0] == '#')
		package.channel->broadcastMessage(package.client, msg);
	else
		send(receiver->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Kick(Server &server)
{
	if (!package.channel)
		return ;
	Client		*client =	server.getClient(package.cmd_data[KICK_USER]);
	std::string	msg = getRPL() + RPL_KICK(package.cmd_data[KICK_USER], package.cmd_data[KICK_MSG]) + "\r\n";

	send(client->getFd(), msg.c_str(), msg.size(), 0);
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	package.channel->broadcastMessage(package.client, msg);
}

void	RPL::Join(Server &server)
{
	(void) server;
	std::string msg;

	msg = HEADER_STR("332", package.client->getNick(), " ", package.rpl_data) + RPL_NOTOPIC + "\r\n";
	if (!package.rpl_data.empty())
		msg = HEADER_STR("332", package.client->getNick(), " ", package.rpl_data) + RPL_TOPIC(package.rpl_data) + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("353", package.client->getNick(), " = ", package.rpl_data) + package.channel->getNameList() + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("366", package.client->getNick(), " ", package.rpl_data) + RPL_ENDOFNAMES + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Topic(Server &server)
{
	(void) server;
	std::string	msg;

	msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmd_data[TOPIC_CHANNEL])
		+ RPL_TOP(package.cmd_data[TOPIC_NEW]) + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	if (package.channel)
		package.channel->broadcastMessage(package.client, msg);
}

void	RPL::Invite(Server &server)
{
	(void) server;
	std::string msg;

	msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmd_data[INVITE_NICK])
		+ RPL_INVITE(package.channel->getName()) + "\r\n";
	std::cout << "RPLY INVITED : " << msg << std::endl;
	send(server.getClient(package.cmd_data[INVITE_NICK])->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_ERROR("341", package.client->getNick()) + package.cmd_data[INVITE_NICK] + " " + package.cmd_data[INVITE_CHANNEL] + "\r\n";
	std::cout << "RPLY INVITER : " << msg << std::endl;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::reply(Server &server)
{
	std::map<std::string, void (*)(Server &server)>::iterator it;

	if (package.error)
		return (RPL::Error(server));

	it = rpls.find(package.cmd);
	if (it == rpls.end())
		return ;
	it->second(server);
}
