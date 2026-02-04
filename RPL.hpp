/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 20:28:57 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/04 01:53:33 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <sstream>
# include <sys/socket.h>

// SERVER
# define VERSION "1.0"
# define CMODES "itkol"
# define SERVER_NAME "ft.irc"
# define NETWORK_NAME "CACA"

//RPL MESSAGES
# define RPL_WELCOME	1
# define RPL_YOURHOST	2
# define RPL_CREATED	3
# define RPL_MYINFO		4
# define RPL_ISUPPORT	5

# define RPL_HEADER(code, nick)	":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " :"

# define RPL_WELCOME_STR(nick)	"Welcome to " NETWORK_NAME ", " + std::string(nick)
# define RPL_YOURHOST_STR		"Your host is " SERVER_NAME ", running version " VERSION
# define RPL_CREATED_STR		"This server was created <timestamp>"
# define RPL_MYINFO_STR(nick)	std::string(nick) + " " + SERVER_NAME + " " + VERSION " 0 " CMODES
# define RPL_ISUPPORT_STR		""

class RPL
{
	public:
		static std::string	code_to_string(short code);
		static std::string	createMessage(short code, std::string nick);
		static std::string	getMessage(short code, std::string nick);

		static void	connection(int fd, std::string const &nick);
};
