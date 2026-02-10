/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/10 05:40:28 by hkeromne         ###   ########.fr       */
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
# define VERSION "1.0"
# define CMODES "itkol"
# define SERVER_NAME "ft.irc"
# define NETWORK_NAME "CACA"

# define HEADER_STR(code, nick, op, channel)	":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + std::string(op) + std::string(channel) + " :"

//Welcome
# define RPL_WELCOME_STR(nick)				"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR					"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR					"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)				std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR					""

//Join
# define RPL_TOPIC(topic)	topic
# define RPL_NOTOPIC		"No topic is set"
# define RPL_ENDOFNAMES		"End of /NAMES list"

//Errors
# define ERR_PASSWDMISMATCH_STR				"Password incorrect"

# define RPL_STR(nick, user, cmd, data)	":" + std::string(nick) + "!" + std::string(user) + "@" SERVER_NAME " " + std::string(cmd) + " " + std::string(data)

# define RPL_TOP(topic)					RPL_PRIV(topic)
# define RPL_PRIV(msg)					" :" + std::string(msg)
# define RPL_KICK(user, data)			" " + std::string(user) + " :" + std::string(data)

class Server;

enum ERR_ID
{
	ERR_PASSWDMISMATCH = 464
};

class RPL
{
	public:
		static void	reply(Server &server);
		static void Welcome(const int &fd, std::string const &nick);
		static void	Topic(Server &server);
		static void	Join(Server &server);
		static void	Kick(Server &server);
		static void	Priv(Server &server);
		static void	Default(Server &server);
		static void	Error(Server &server);
};
