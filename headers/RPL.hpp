/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 05:09:41 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <sstream>
# include <sys/socket.h>
# include "CMD.hpp"
# include "Server.hpp"
# include "Package.hpp"

// SERVER
# define VERSION									"1.0"
# define CMODES										"itkol"
# define SERVER_NAME								"ft.irc"
# define NETWORK_NAME								"CACA"

# define HEADER_STR(code, nick, op, channel)		":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + std::string(op) + std::string(channel) + " :"

//Welcome
# define RPL_WELCOME_STR(nick)						"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR							"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR							"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)						std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR							""

//Join
# define RPL_TOPIC(topic)							topic
# define RPL_NOTOPIC								"No topic is set"
# define RPL_ENDOFNAMES								"End of /NAMES list"

//Errors
# define HEADER_ERROR(code, nick)					":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " "

# define ERR_NOSUCHNICK_STR(nick)					std::string(nick) + " :No such nick"
# define ERR_PASSWDMISMATCH_STR						"Password incorrect"
# define ERR_NONICKNAMEGIVEN_STR					":No nickname given"
# define ERR_ONEUSNICKNAME_STR(nick)				std::string(nick) +	" :Erroneous nickname"
# define ERR_NICKNAMEINUSE_STR(nick)				std::string(nick) +	" :Nickname is already in use"
# define ERR_NEEDMOREPARAMS_STR(cmd)				std::string(cmd) +	" :Not enough parameters"
# define ERR_USERNOTINCHANNEL_STR(nick, channel)	std::string(nick) + " " + std::string(channel) + " :They aren't on that channel"
# define ERR_USERONCHANNEL_STR(user, channel)		std::string(user) + " " + std::string(channel) + " :is already on channel"
# define ERR_INVITEONLYCHAN_STR(channel)			std::string(channel) + " :Cannot join channel (+i)" 
# define ERR_CHANNELISFULL_STR(channel)				std::string(channel) + " :Cannot join channel (+l)"
# define ERR_BADCHANNELKEY_STR(channel)				std::string(channel) + " :Cannot join channel (+k)"
# define ERR_NOSUCHCHANNEL_STR(channel)				std::string(channel) + " :No such channel"
# define ERR_CHANOPRIVSNEEDED_STR(channel)			std::string(channel) + " :You're not channel operator"
# define ERR_NOTONCHANNEL_STR(channel)				std::string(channel) + " :You're not on that channel"
# define ERR_ALREADYREGISTRED_STR					":Unauthorized command (already registered)"

//Numeric replies
# define RPL_STR(nick, user, cmd, data)				":" + std::string(nick) + "!" + std::string(user) + "@" SERVER_NAME " " + std::string(cmd) + " " + std::string(data)

# define RPL_TOP(topic)								" :" + std::string(topic)
# define RPL_NICK(nick)								nick
# define RPL_PRIV(msg)								" :" + std::string(msg)
# define RPL_KICK(user, data)						" " + std::string(user) + " :" + std::string(data)
# define RPL_JOIN(channel)							" :" + std::string(channel)
# define RPL_INVITE(channel)						" :" + std::string(channel)
# define RPL_MODE(modes)							modes

class Server;

enum ERR_ID
{
	ERR_NOSUCHNICK =		401,
	ERR_NOSUCHCHANNEL =		403,
	ERR_NONICKNAMEGIVEN =	431,
	ERR_ONEUSNICKNAME =		432,
	ERR_NICKNAMEINUSE =		433,
	ERR_USERNOTINCHANNEL =	441,
	ERR_NOTONCHANNEL =		442,
	ERR_USERONCHANNEL =		443,
	ERR_NEEDMOREPARAMS =	461,
	ERR_ALREADYREGISTRED =	462,
	ERR_PASSWDMISMATCH =	464,
	ERR_CHANNELISFULL =		471,
	ERR_UNKNOWNMODE =		472,
	ERR_INVITEONLYCHAN =	473,
	ERR_BADCHANNELKEY =		475,
	ERR_CHANOPRIVSNEEDED =	482
};

class RPL
{
	public:
		static void	reply(Server &server);
		static void Welcome(const int &fd, std::string const &nick);
		static void	Nick(Server &server);
		static void	Topic(Server &server);
		static void	Join(Server &server);
		static void	Kick(Server &server);
		static void	Priv(Server &server);
		static void	Default(Server &server);
		static void	Error(Server &server);
		static void	Invite(Server &server);
		static void Mode(Server &server);
};
