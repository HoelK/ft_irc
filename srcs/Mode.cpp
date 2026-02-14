# include "Mode.hpp"

std::map<char, int (*)(Server &, bool, int)>	Mode::mode = {
	{MODE_OP, &Mode::o},
	{MODE_INVITE, &Mode::i},
	{MODE_LIMIT, &Mode::l},
	{MODE_KEY, &Mode::k},
	{MODE_TOPIC, &Mode::t}
};

static int checkModes(std::string const &modes)
{
	int			args	= 0;
	std::string	set		= CMODES;

	if (modes[0] != '+' && modes[0] != '-')
		return (ERR_UNKNOWNMODE);
	for (int i = 0;	i < (int)modes.length(); i++)
	{
		if (modes[i] == '+' || modes[i] == '-')
			continue ;
		if (set.find(modes[i]) == std::string::npos)
			return (ERR_UNKNOWNMODE);
		if (modes[i] == 'o' || modes[i] == 'l' || modes[i] == 'k')
			args++;
	}
	if ((int)package.cmd_data.size() < (args + 1))
		return (ERR_NEEDMOREPARAMS);
	return (0);
}

int	Mode::Check(Server &server, std::string const &modes)
{
	(void)		server;
	int			error	= 0;
	std::string	set		= CMODES;

	if (package.cmd_data.size() < 2)
		return (0);
	if (package.cmd_data[MODE_CHANNEL][0] != '#')
		return (-1);
	if (!server.isChannel(package.cmd_data[MODE_CHANNEL]))
		return (ERR_NOSUCHNICK);
	package.channel = server.getChannel(package.cmd_data[MODE_CHANNEL]);
	if (!package.client->getOp())
		return (ERR_CHANOPRIVSNEEDED);
	if (!package.channel->isClient(package.client->getNick()))
		return (ERR_NOTONCHANNEL);
	if (package.cmd_data.size() < 3)
		return (0);
	error = checkModes(modes);
	if (error)
		return (error);
	return (0);
}

int		Mode::o(Server &server, bool add, int argCount)
{
	std::string nick = package.cmd_data[argCount];

	if (!server.isClient(nick))
		return (ERR_NOSUCHNICK);
	if (!package.channel->isClient(nick))
		return (ERR_NOTONCHANNEL);
	Client		*client = server.getClient(nick);

	(add) ? client->setOp(true) : client->setOp(false);

	return (0);
}

int		Mode::l(Server &server, bool add, int argCount)
{
	(void)				server;
	int					limit;
	std::stringstream	sNum;

	if (!add)
		package.channel->setOpLimit(0);
	if (!Ft::isInt(package.cmd_data[argCount]))
		return (ERR_NEEDMOREPARAMS);
	sNum << package.cmd_data[argCount];
	sNum >> limit;

	if (!sNum.eof() || sNum.fail())
		return (ERR_NEEDMOREPARAMS);

	package.channel->setOpLimit(limit);

	return (0);
}

int		Mode::i(Server &server, bool add, int argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpInvite(true) : package.channel->setOpInvite(false);

	return (0);
}

int		Mode::t(Server &server, bool add, int argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpTopic(true) : package.channel->setOpTopic(false);
	return (0);
}

int		Mode::k(Server &server, bool add, int argCount)
{
	(void) server;
	(void) argCount;

	(add) ? package.channel->setOpKey(true, package.cmd_data[argCount]) : package.channel->setOpKey(false, "");

	return (0);
}
