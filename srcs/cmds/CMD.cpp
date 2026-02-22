/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/22 23:50:45 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD.hpp"
# include "RPL.hpp"
# include "Errors.hpp"

static std::map<std::string, void (*)(Server &)> initCmds()
{
    std::map<std::string, void (*)(Server &)> m;

	m[CMD_PING] = &CMD::Ping;
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

    return (m);
}

static std::map<std::string, void (*)(Server &server)> cmds = initCmds();

void	CMD::apply(Server &server)
{
	std::map<std::string, void (*)(Server &server)>::iterator it;

	it = cmds.find(package.cmd);
	if (it == cmds.end())
		return (package.setError(ERR_UNKNOWNCOMMAND));
	it->second(server);
}

void	CMD::Ping(Server &server)
{
	(void) server;

	if (package.cmdData.size() < 1)
		return (package.setError(ERR_NEEDMOREPARAMS));
	
	return ;
}

void	CMD::Pass(Server &server)
{
	(void) server;

	if (package.cmdData.size() < 1)
		return (package.setError(ERR_NEEDMOREPARAMS));
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED));
	package.client->setPass(package.cmdData[PASS_PASS]);
}

void	CMD::Nick(Server &server)
{
	if (!Nick::Check(server))
		return ;
	Nick::Change();
}

void	CMD::Quit(Server &server)
{
	(void) server;
	std::string	msg;

	package.quit = true;
	package.quitMessage = (package.cmdData.size() < 1)
		? DEFAULT_QUIT_MSG
		: package.cmdData[QUIT_REASON];
}

void	CMD::User(Server &server)
{
	if (!User::Check(server))
		return ;
	User::Set();
}

void	CMD::Join(Server &server)
{
	if (!Join::Check())
		return ;
	(server.isChannel(package.cmdData[JOIN_CHANNEL]))
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
	Kick::Kicking(server);
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

	if (!Mode::Check(server))
		return ;
	std::string	&modes		= package.cmdData[MODE_MODES]; 
	for (int i = 0; i < (int)modes.length(); i++)
	{
		char m = modes[i];
		if (!Mode::singleCheck(modes, m, argCount))
		{
			RPL::reply(server);
			package.errClear();
			continue ;
		}
		if (modes[i] == '+')
			add = true;
		else if (modes[i] == '-')
			add = false;
		else
			Mode::mode[modes[i]](server, add, argCount);
	}
	Mode::join();
}
