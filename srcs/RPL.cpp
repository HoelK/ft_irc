/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/09 03:38:07 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"

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

	return ("");
}

static std::string	getError(short code, std::string const &nick) { return (HEADER_STR(codeStr(code), nick, "", "") + codeToErr(code)); };
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

void	RPL::Join(const int &fd, std::string const &nick, std::string const &topic, Channel &channel)
{
	std::string msg;
	std::string users;

	msg = HEADER_STR("332", nick, " ", topic) + RPL_NOTOPIC + "\r\n";
	if (!topic.empty())
		msg = HEADER_STR("332", nick, " ", topic) + RPL_TOPIC(topic) + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("353", nick, " = ", topic) + channel.getNameList() + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
	msg = HEADER_STR("366", nick, " ", topic) + RPL_ENDOFNAMES + "\r\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

void RPL::reply(Server &server)
{
	std::string	msg;

	msg = getRPL();
	if (package.error)
		msg = getError(package.error, package.oldClient.getNick());
	else if (package.cmd == CMD_JOIN)
		return (RPL::Join(package.client->getFd(), package.client->getNick(), package.rpl_data, *(server.getChannel(package.rpl_data))));
	msg = msg + "\r\n";

	std::cout << "REPLY : " << msg;
	send(package.client->getFd(), msg.c_str(), msg.size(), 0);
}
