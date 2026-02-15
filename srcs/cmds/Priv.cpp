# include "Priv.hpp"

bool Priv::Check(Server &server)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string target = package.cmd_data[PRIV_TARGET];
	if (target[0] == '#' && !server.isChannel(target))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	if (target[0] != '#' && !server.isClient(target))
		return (package.setError(ERR_NOSUCHNICK), false);
	return (true);
}

void Priv::Send(Server &server)
{
	package.rpl_data = package.cmd_data[PRIV_TARGET];
	Channel	*channel = server.getChannel(package.rpl_data);

	if (package.rpl_data[0] == '#' && channel)
		package.channel = channel;
}
