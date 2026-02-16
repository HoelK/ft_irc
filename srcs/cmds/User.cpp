# include "User.hpp"

bool User::Check(Server &server)
{
	(void) server;

	if (package.cmdData.size() < 4)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED), false);
	return (true);
}

void User::Set(void)
{
	package.client->setUser(package.cmdData[USER_USERNAME]);
	package.client->setName(package.cmdData[USER_REALNAME]);
}
