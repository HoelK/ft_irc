/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 16:37:11 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <sstream>
# include <sys/socket.h>
# include "CMD.hpp"
# include "Package.hpp"

// SERVER
# define VERSION "1.0"
# define CMODES "itkol"
# define SERVER_NAME "ft.irc"
# define NETWORK_NAME "CACA"

# define RPL_HEADER(code, nick)	":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " :"

# define RPL_WELCOME_STR(nick)				"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR					"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR					"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)				std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR					""

# define RPL_NICK_STR(oldnick, nick, user)	":" + std::string(oldnick) + "!" + std::string(user) + "@" SERVER_NAME " NICK :" + std::string(nick)


//RPL MESSAGES
enum RPL_ID
{
	RPL_WELCOME = 1,
	RPL_YOURHOST,
	RPL_CREATED,
	RPL_MYINFO,
	RPL_ISUPPORT,
	RPL_PONG,
	RPL_NICK,
	RPL_USER
};

class RPL
{
	public:
		static short		cmdToCode();
		static std::string	codeToStr(short code);
		static std::string	createMessage(short code, std::string const &nick);
		static std::string	getMessage(short code, std::string nick);

		static void	reply(void);
		static void	connection(int fd, std::string const &nick);
};
