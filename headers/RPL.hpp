/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/22 23:57:06 by hkeromne         ###   ########.fr       */
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
# define RPL_STR(nick, user, cmd)					":" + std::string(nick) + "!" + std::string(user) + "@" SERVER_NAME " " + std::string(cmd) + " "

# define RPL_PING									"PONG " SERVER_NAME
# define RPL_NICK(nick)								nick
# define RPL_JOIN(channel)							":" + std::string(channel)
# define RPL_QUIT(message)							std::string(message)
# define RPL_TOP(channel, topic)					std::string(channel) + " :" + std::string(topic)
# define RPL_PRIV(target, msg)						std::string(target) + " :" + std::string(msg)
# define RPL_KICK(channel, user, data)				std::string(channel) + " " + std::string(user) + " :" + std::string(data)
# define RPL_INVITE(nick, channel)					std::string(nick) + " :" + std::string(channel)
# define RPL_MODE(channel, modes)					std::string(channel) + " " + std::string(modes)

class Server;

class RPL
{
	public:
		static void	reply(Server &server);
		static void Welcome(Server &server, Client *client, std::string const &nick);
		static void Ping(Server &server);
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
