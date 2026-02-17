/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:43:39 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 00:44:39 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Mode.hpp"
# include "RPL.hpp"

static std::map<char, void (*)(Server &, bool, int &)> initMode()
{
    std::map<char, void (*)(Server &, bool, int &)> m;

	m[MODE_OP] =		&Mode::o;
	m[MODE_INVITE] =	&Mode::i;
	m[MODE_LIMIT] =		&Mode::l;
	m[MODE_KEY] =		&Mode::k;
	m[MODE_TOPIC] =		&Mode::t;

    return m;
}

std::map<char, void (*)(Server &, bool, int &)> Mode::mode = initMode();

static bool checkModes(std::string const &modes)
{
	int			args	= 0;
	std::string	set		= CMODES;

	package.errMode = modes;
	if (modes[0] != '+' && modes[0] != '-')
		return (package.setError(ERR_UNKNOWNMODE), false);
	for (int i = 0;	i < (int)modes.length(); i++)
	{
		char m = modes[i];
		if (m == '+' || m == '-'
		|| set.find(m) == std::string::npos)
			continue ;
		if (m == 'o' || m == 'l' || m == 'k')
			args++;
	}
	if (((int)package.cmdData.size() - 2) < args)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	return (true);
}

bool	Mode::Check(Server &server, std::string const &modes)
{
	std::string	set	= CMODES;
	std::string nick = package.client->getNick();

	if (package.cmdData.size() < 1)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	std::string chanName = package.cmdData[MODE_CHANNEL];
	package.errChanName = chanName;
	package.errNick = nick;
	if (chanName[0] != '#')
		return (package.setError(-1), false);
	if (!server.isChannel(chanName))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	package.channel = server.getChannel(chanName);
	if (package.cmdData.size() == 1)
		return (false);
	if (!package.channel->isClient(nick))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (!package.channel->isOperator(nick) && package.cmdData.size() >= 2)
		return (package.setError(ERR_CHANOPRIVSNEEDED), false);
	return (checkModes(modes));
}

bool	Mode::singleCheck(std::string &modes, char m, int &argCount)
{
	std::string	set = CMODES;

	if (m == '+' || m == '-' || set.find(m) != std::string::npos)
		return (true);
	package.errMode = m;
	if (set.find(m) == std::string::npos)
	{
		modes.erase(modes.find(m));
		if (m == 'l' || m == 'o' || m == 'k')
		{
			package.cmdData.erase(package.cmdData.begin() + argCount);
			argCount++;
		}
		return (package.setError(ERR_UNKNOWNMODE), false);
	}
	return (true);
}

void	Mode::o(Server &server, bool add, int &argCount)
{
	std::string nick = package.cmdData[argCount];
	package.errNick = nick;

	if (!server.isClient(nick))
		return (package.setError(ERR_NOSUCHNICK));
	if (!package.channel->isClient(nick))
		return (package.setError(ERR_NOTONCHANNEL));
	Client		*client = server.getClient(nick);

	if (add && package.channel->isOperator(client->getNick()))
		return ;

	(add) ? package.channel->addOperator(client) : (void)package.channel->removeOperator(client->getNick());
	argCount++;
}

void	Mode::l(Server &server, bool add, int &argCount)
{
	(void)				server;
	int					limit;
	std::stringstream	sNum;

	if (!add)
		package.channel->setOpLimit(0);
	if (!Ft::isInt(package.cmdData[argCount]))
		return (package.setError(ERR_NEEDMOREPARAMS));
	sNum << package.cmdData[argCount];
	sNum >> limit;

	if (!sNum.eof() || sNum.fail())
		return (package.setError(ERR_NEEDMOREPARAMS));

	package.channel->setOpLimit(limit);
	argCount++;
}

void	Mode::i(Server &server, bool add, int &argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpInvite(true) : package.channel->setOpInvite(false);
}

void	Mode::t(Server &server, bool add, int &argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpTopic(true) : package.channel->setOpTopic(false);
}

void	Mode::k(Server &server, bool add, int &argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpKey(true, package.cmdData[argCount]) : package.channel->setOpKey(false, "");
	argCount++;
}

void	Mode::join(void)
{
	std::string &modes = package.cmdData[1];

	for (std::vector<std::string>::iterator it = package.cmdData.begin() + 2; it != package.cmdData.end(); it++)
		modes = modes + " " + *it;
}
