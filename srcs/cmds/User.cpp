# include "User.hpp"

bool User::Check(Server &server)
{
	(void) server;

	if (package.cmd_data.size() < 5)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED), false);
	return (true);
}

void User::Set(void)
{
	package.client->setUser(package.cmd_data[USER_USERNAME]);
	package.client->setName(package.cmd_data[USER_REALNAME]);
}
