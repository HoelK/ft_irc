/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:04:31 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <sstream>
# include <sys/socket.h>
# include "CMD.hpp"
# include "Package.hpp"
# include "Server.hpp"

// SERVER
# define VERSION									"1.0"
# define CMODES										"itkol"
# define SERVER_NAME								"ft.irc"
# define NETWORK_NAME								"42irc"

# define HEADER_STR(code, nick, op, channel)		":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + std::string(op) + std::string(channel) + " :"

//Welcome
# define RPL_WELCOME_STR(nick)						"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR							"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR(time)						"This server was created " + std::string(time)
# define RPL_MYINFO_STR(nick)						std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 +" CMODES
# define RPL_ISUPPORT_STR							""

//Join
# define RPL_TOPIC(topic)							topic
# define RPL_NOTOPIC								"No topic is set"
# define RPL_ENDOFNAMES								"End of /NAMES list"

//Numeric replies
# define RPL_STR(nick, user, cmd, data)				":" + std::string(nick) + "!" + std::string(user) + "@" SERVER_NAME " " + std::string(cmd) + " " + std::string(data)

# define RPL_TOP(topic)								" :" + std::string(topic)
# define RPL_NICK(nick)								nick
# define RPL_PRIV(msg)								" :" + std::string(msg)
# define RPL_KICK(user, data)						" " + std::string(user) + " :" + std::string(data)
# define RPL_JOIN(channel)							" :" + std::string(channel)
# define RPL_INVITE(channel)						" :" + std::string(channel)
# define RPL_MODE(modes)							" " + std::string(modes)
# define RPL_QUIT(message)							" " + std::string(message)

class Server;

class RPL
{
	public:
		static void	reply(Server &server);
		static void Welcome(Client *client, std::string const &nick);
		static void	Nick(Server &server);
		static void	Topic(Server &server);
		static void	Join(Server &server);
		static void	Kick(Server &server);
		static void	Priv(Server &server);
		static void	Default(Server &server);
		static void	Error(Server &server);
		static void	Invite(Server &server);
		static void Mode(Server &server);
		static void Quit(Server &server);
};
