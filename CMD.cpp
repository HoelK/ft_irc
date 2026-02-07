# include "CMD.hpp"

static std::map<std::string, void (*)()> cmds = {
	{CMD_NICK, &CMD::Nick},
	{CMD_USER, &CMD::User}
};

void	CMD::apply(void)
{
	std::map<std::string, void (*)()>::iterator it;

	it = cmds.find(package.cmd);
	if (it == cmds.end())
		return ;
	it->second();
}

void	CMD::Nick(void) { package.client->setNick(package.data[NICK_NICK]); };
void	CMD::User(void)
{
	package.client->setUser(package.data[USER_USERNAME]);
	package.client->setName(package.data[USER_REALNAME]);
}
