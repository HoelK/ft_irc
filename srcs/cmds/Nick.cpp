# include "Nick.hpp"

bool Nick::Check(Server &server)
{
	if (package.cmdData.size() < 1 || package.cmdData.size() == std::string::npos)
		return (package.setError(ERR_NONICKNAMEGIVEN), false);

	std::string	nick =	package.cmdData[NICK_NICK];
	package.errNick	=	nick;
	if (!Ft::isValidNickname(nick))
		return (package.setError(ERR_ONEUSNICKNAME), false);
	if (server.isClient(nick))
		return (package.setError(ERR_NICKNAMEINUSE), false);

	return (true);
}

void Nick::Change(void)
{
	std::string oldNick =	package.client->getNick();
	std::string	nick =		package.cmdData[NICK_NICK];

	package.client->setNick(nick);
	package.client->updateInChannel(oldNick);
}
