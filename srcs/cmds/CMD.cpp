/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/14 19:52:27 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD.hpp"

static std::map<std::string, void (*)(Server &)> initCmds()
{
    std::map<std::string, void (*)(Server &)> m;

	m[CMD_NICK] = &CMD::Nick;
	m[CMD_USER] = &CMD::User;
	m[CMD_PASS] = &CMD::Pass;
	m[CMD_QUIT] = &CMD::Quit;
	m[CMD_JOIN] = &CMD::Join;
	m[CMD_PRIV] = &CMD::Priv;
	m[CMD_KICK] = &CMD::Kick;
	m[CMD_TOPIC] = &CMD::Topic;
	m[CMD_INVITE] = &CMD::Invite;
	m[CMD_MODE] = &CMD::Mode;

    return m;
}

static std::map<std::string, void (*)(Server &server)> cmds = initCmds();

void	CMD::apply(Server &server)
{
	std::map<std::string, void (*)(Server &server)>::iterator it;

	it = cmds.find(package.cmd);
	if (it == cmds.end())
		return ;
	it->second(server);
}

void	CMD::Pass(Server &server)
{
	(void) server;
	if (package.cmd_data.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS));
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED));
	package.client->setPass(package.cmd_data[PASS_PASS]);
}

void	CMD::Nick(Server &server)
{
	std::string oldNick =	package.client->getNick();
	std::string	nick =		package.cmd_data[NICK_NICK];

	package.rpl_data = nick;
	if (package.cmd_data.size() < 2)
		return (package.setError(ERR_NONICKNAMEGIVEN));
	if (!Ft::isValidNickname(nick))
		return (package.setError(ERR_ONEUSNICKNAME));
	if (server.isClient(nick))
		return (package.setError(ERR_NICKNAMEINUSE));

	package.client->setNick(nick);
	package.client->updateInChannel(oldNick);
	package.rpl_data = package.client->getNick();
}

void	CMD::Quit(Server &server)
{
	(void) server;
	package.quit = true;
	package.rpl_data = package.cmd_data[QUIT_REASON];
}

void	CMD::User(Server &server)
{
	(void) server;
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED));
	if (package.cmd_data.size() < 5)
		return (package.setError(ERR_NEEDMOREPARAMS));
	package.client->setUser(package.cmd_data[USER_USERNAME]);
	package.client->setName(package.cmd_data[USER_REALNAME]);
}

void	CMD::Join(Server &server)
{
	if (!Join::Check())
		return ;
	(server.isChannel(package.cmd_data[JOIN_CHANNEL]))
		? Join::Joining(server)
		: Join::Create(server);
}

void	CMD::Priv(Server &server)
{
	if (!Priv::Check(server))
		return ;
	Priv::Send(server);
}

void	CMD::Kick(Server &server)
{
	if (!Kick::Check(server))
		return ;
	Kick::Kicking();
}

void	CMD::Topic(Server &server)
{
	if (!Topic::Check(server))
		return ;
	Topic::Set(server);
}

void	CMD::Invite(Server &server)
{
	if (!Invite::Check(server))
		return ;
	Invite::Send(server);
}

void	CMD::Mode(Server &server)
{
	int			argCount	= 2;
	bool		add			= false;
	std::string	&modes		= package.cmd_data[MODE_MODES];

	package.error = Mode::Check(server, modes);
	if (package.cmd_data.size() == 2 || package.error)
		return ;
	for (int i = 0; i < (int)modes.length(); i++)
	{
		if (modes[i] == '+')
			add = true;
		else if (modes[i] == '-')
			add = false;
		else
			package.error = Mode::mode[modes[i]](server, add, argCount);
		if (package.error)
			return ;
	}
}
