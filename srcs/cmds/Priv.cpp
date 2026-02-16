# include "Priv.hpp"

bool Priv::Check(Server &server)
{
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string target = package.cmdData[PRIV_TARGET];
	if (target[0] == '#' && !server.isChannel(target))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	if (target[0] != '#' && !server.isClient(target))
		return (package.setError(ERR_NOSUCHNICK), false);
	return (true);
}

void Priv::Send(Server &server)
{
	package.rplData = package.cmdData[PRIV_TARGET];
	Channel	*channel = server.getChannel(package.rplData);

	if (package.rplData[0] == '#' && channel)
		package.channel = channel;
}
