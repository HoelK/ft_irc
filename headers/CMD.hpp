/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/10 00:41:47 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Server.hpp"
# include "Package.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define CMD_PASS			"PASS"
# define CMD_NICK			"NICK"
# define CMD_USER			"USER"
# define CMD_QUIT			"QUIT"
# define CMD_JOIN			"JOIN"
# define CMD_PRIV			"PRIVMSG"
# define CMD_KICK			"KICK"
# define NICK_NICK			0
# define PASS_PASS			0
# define USER_USERNAME		0
# define USER_REALNAME		3
# define QUIT_REASON		0
# define JOIN_CHANNEL		0
# define PRIV_TARGET		0
# define PRIV_MSG			1
# define KICK_CHANNEL		0
# define KICK_USER			1
# define KICK_MSG			2

class Server;

class CMD
{
	public:
		static void	apply(Server &server);
		static void	Nick(Server &server);
		static void	User(Server &server);
		static void	Pass(Server &server);
		static void	Quit(Server &server);
		static void Join(Server &server);
		static void	Priv(Server &server);
		static void	Kick(Server &server);
};
