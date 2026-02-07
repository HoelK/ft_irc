/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 21:46:06 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RPL.hpp"

std::string		RPL::getMessage(short code, std::string nick)
{
	if (code == RPL_WELCOME)
		return (RPL_WELCOME_STR(nick));
	else if (code == RPL_YOURHOST)
		return (RPL_YOURHOST_STR);
	else if (code == RPL_CREATED)
		return (RPL_CREATED_STR);
	else if (code == RPL_MYINFO)
		return (RPL_MYINFO_STR(nick));
	else if (code == RPL_ISUPPORT)
		return (RPL_ISUPPORT_STR);
	else if (code == RPL_NICK)
		return (RPL_NICK_STR(package.oldClient.getNick(), package.client->getNick(), package.client->getUser()));
	else if (code == RPL_QUIT)
		return (RPL_QUIT_STR(package.client->getNick(), package.client->getUser(), package.quit_message));

	return ("");
}

std::string		RPL::codeToStr(short code)
{
	std::stringstream	stream;

	if (code < 10)
		stream << "00";
	else if (code >= 10 && code < 100)
		stream << "0";
	stream << code;
	return (stream.str());
}

std::string		RPL::createMessage(short code, std::string const &nick)
{
	std::stringstream	stream;

	stream << RPL_HEADER(codeToStr(code), nick);
	stream << RPL::getMessage(code, nick);
	stream << "\r\n";

	return (stream.str());
}

void RPL::connection(int fd, std::string const &nick)
{
	std::string sent_message = RPL::createMessage(RPL_WELCOME,	nick);
	std::cout << sent_message;
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_YOURHOST, nick);
	std::cout << sent_message;
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_CREATED,	nick);
	std::cout << sent_message;
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_MYINFO,	nick);
	std::cout << sent_message;
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_ISUPPORT, nick);
	std::cout << sent_message;
	send(fd, sent_message.c_str(), sent_message.size(), 0);
}

static short	cmdToCode(std::string const &cmd)
{
	if (cmd == CMD_NICK)
		return (RPL_NICK);
	else if (cmd == CMD_USER)
		return (RPL_USER);
	else if (cmd == CMD_QUIT)
		return (RPL_QUIT);
	return (-1);
}

void RPL::reply(void)
{
	std::string sent_message = RPL::getMessage(::cmdToCode(package.cmd), package.client->getNick())
	+ "\r\n";

	std::cout << "REPLY : " << sent_message;
	send(package.client->getFd(), sent_message.c_str(), sent_message.size(), 0);
}
