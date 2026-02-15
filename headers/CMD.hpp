/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/14 19:47:05 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Nick.hpp"
# include "Priv.hpp"
# include "Join.hpp"
# include "Mode.hpp"
# include "Kick.hpp"
# include "Topic.hpp"
# include "Invite.hpp"
# include "Server.hpp"
# include "Package.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define CMD_PASS	"PASS"
# define CMD_NICK	"NICK"
# define CMD_USER	"USER"
# define CMD_QUIT	"QUIT"
# define CMD_JOIN	"JOIN"
# define CMD_PRIV	"PRIVMSG"
# define CMD_KICK	"KICK"
# define CMD_TOPIC	"TOPIC"
# define CMD_INVITE "INVITE"
# define CMD_MODE	"MODE"

enum	cmdDataIds
{
	NICK_NICK		= 0,
	PASS_PASS		= 0,
	USER_USERNAME	= 0,
	USER_REALNAME	= 3,
	QUIT_REASON		= 0,
	JOIN_CHANNEL	= 0,
	JOIN_KEY		= 1,
	PRIV_TARGET		= 0,
	PRIV_MSG		= 1,
	KICK_CHANNEL	= 0,
	KICK_USER		= 1,
	KICK_MSG		= 2,
	TOPIC_CHANNEL	= 0,
	TOPIC_NEW		= 1,
	INVITE_NICK		= 0,
	INVITE_CHANNEL	= 1,
	MODE_CHANNEL	= 0,
	MODE_MODES		= 1
};

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
		static void Topic(Server &server);
		static void	Invite(Server &server);
		static void	Mode(Server &server);
};
