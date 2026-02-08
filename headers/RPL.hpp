/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 21:36:44 by hkeromne         ###   ########.fr       */
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

# define HEADER_STR(code, nick)	":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " :"

# define RPL_WELCOME_STR(nick)				"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR					"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR					"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)				std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR					""

# define ERR_PASSWDMISMATCH_STR				"Password incorrect"

# define RPL_STR(nick, user, cmd, data)	":" + std::string(nick) + "!" + std::string(user) + "@" SERVER_NAME " " + std::string(cmd) + " " + std::string(data)

enum ERR_ID
{
	ERR_PASSWDMISMATCH = 464
};

class RPL
{
	public:
		static void	reply(void);
		static void Welcome(const int &fd, std::string const &nick);
};
