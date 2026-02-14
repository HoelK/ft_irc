# include "Kick.hpp"

bool Kick::Check(void)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS));
}

void Kick::tuTej(void)
{
	std::string	kickUser =		package.cmd_data[KICK_USER];
	std::string	kickChannel	=	package.cmd_data[KICK_CHANNEL];

	if (!package.client->isChannel(kickChannel))
		return (package.setError(ERR_NOSUCHCHANNEL));

	Channel *channel	= package.client->getChannel(kickChannel);
	package.channel		= channel;

	if (!channel->isClient(kickUser))
	{
		package.rpl_data = kickUser;
		return (package.setError(ERR_USERNOTINCHANNEL));
	}
	Client	*client		= channel->getClient(kickUser);
	package.channel		= channel;
	package.rpl_data	= kickChannel;
	channel->removeClient(client->getNick());
}
