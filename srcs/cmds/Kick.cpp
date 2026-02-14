# include "Kick.hpp"

bool Kick::Check(Server &server)
{

	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	
	std::string	kickNick =	package.cmd_data[KICK_USER];
	std::string kickChan =	package.cmd_data[KICK_CHANNEL];

	if (!server.isChannel(kickChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel		*channel =	server.getChannel(kickChan);

	if (!server.isClient(kickNick))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	if (!channel->isClient(kickNick))
		return (package.setError(ERR_NOTONCHANNEL), false);
	return (true);
}

void Kick::Kicking(void)
{
	std::string	kickNick =		package.cmd_data[KICK_USER];
	std::string	kickChannel	=	package.cmd_data[KICK_CHANNEL];
	package.channel =			package.client->getChannel(kickChannel);
	Client	*client	=			package.channel->getClient(kickNick);

	package.rpl_data = kickChannel;
	package.channel->removeClient(client->getNick());
}
