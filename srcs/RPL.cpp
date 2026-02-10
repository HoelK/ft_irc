/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/10 06:48:16 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"

static std::map<std::string, void (*)(Server &server)> rpls = {
	{CMD_NICK, &RPL::Default},
	{CMD_QUIT, &RPL::Default},
	{CMD_JOIN, &RPL::Join},
	{CMD_PRIV, &RPL::Priv},
	{CMD_KICK, &RPL::Kick},
	{CMD_TOPIC, &RPL::Topic}
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

	msg = getError(package.error, package.oldClient.getNick()) + "\r\n";
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
	(void) server;
	std::string	msg;

	if (!package.client->getChannel())
		return ;
	msg = getRPL() + RPL_PRIV(package.cmd_data[PRIV_MSG]) + "\r\n";

	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	if (package.client->getChannel())
		package.client->getChannel()->broadcastMessage(package.client, msg);
}


void	RPL::Kick(Server &server)
{
	if (!package.client->getChannel())
		return ;
	Client		*client =	server.getClient(package.cmd_data[KICK_USER]);
	std::string	msg = getRPL() + RPL_KICK(package.cmd_data[KICK_USER], package.cmd_data[KICK_MSG]) + "\r\n";

	send(client->getFd(), msg.c_str(), msg.size(), 0);
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	if (package.client->getChannel())
		package.client->getChannel()->broadcastMessage(package.client, msg);
}

void	RPL::Join(Server &server)
{
	(void) server;
	std::string msg;

	msg = HEADER_STR("332", package.client->getNick(), " ", package.rpl_data) + RPL_NOTOPIC + "\r\n";
	if (!package.rpl_data.empty())
		msg = HEADER_STR("332", package.client->getNick(), " ", package.rpl_data) + RPL_TOPIC(package.rpl_data) + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("353", package.client->getNick(), " = ", package.rpl_data) + package.client->getChannel()->getNameList() + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("366", package.client->getNick(), " ", package.rpl_data) + RPL_ENDOFNAMES + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}

void	RPL::Topic(Server &server)
{
	(void) server;
	std::string	msg;

	if (package.cmd_data.size() < 2)
		msg = HEADER_STR("331", package.client->getName(), " :", package.cmd_data[TOPIC_CHANNEL]) + "\r\n";
	else if (package.cmd_data[TOPIC_NEW].empty())
		msg = HEADER_STR("331", package.client->getName(), " :", RPL_NOTOPIC) + "\r\n";
	else 
		msg = RPL_STR(package.client->getNick(), package.client->getUser(), package.cmd, package.cmd_data[TOPIC_CHANNEL]) + RPL_TOP(package.cmd_data[TOPIC_NEW]) + "\r\n";
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
	if (package.client->getChannel())
		package.client->getChannel()->broadcastMessage(package.client, msg);
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
