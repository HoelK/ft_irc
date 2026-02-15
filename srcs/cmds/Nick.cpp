# include "Nick.hpp"

bool Nick::Check(Server &server)
{
	if (package.cmd_data.size() < 2)
		return (package.setError(ERR_NONICKNAMEGIVEN), false);

	std::string	nick =		package.cmd_data[NICK_NICK];
	if (!Ft::isValidNickname(nick))
		return (package.setError(ERR_ONEUSNICKNAME), false);
	if (server.isClient(nick))
		return (package.setError(ERR_NICKNAMEINUSE), false);

	return (true);
}

void Nick::Change(void)
{
	std::string oldNick =	package.client->getNick();
	std::string	nick =		package.cmd_data[NICK_NICK];

	package.rpl_data = nick;
	package.client->setNick(nick);
	package.client->updateInChannel(oldNick);
	package.rpl_data = package.client->getNick();
}
