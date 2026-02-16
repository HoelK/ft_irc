# include "Kick.hpp"

bool Kick::Check(Server &server)
{
	if (package.cmdData.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	
	std::string	kickNick =	package.cmdData[KICK_USER];
	std::string kickChan =	package.cmdData[KICK_CHANNEL];
	package.errNick =		package.client->getNick();
	package.errChanName =	kickChan;

	if (!server.isChannel(kickChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel		*channel =	server.getChannel(kickChan);

	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (!channel->isOperator(package.client->getNick()))
		return (package.setError(ERR_CHANOPRIVSNEEDED), false);
	package.errNick = kickNick;
	if (!server.isClient(kickNick))
		return (package.setError(ERR_NOSUCHNICK), false);
	package.channel = channel;
	if (!channel->isClient(kickNick))
		return (package.setError(ERR_USERNOTINCHANNEL), false);
	return (true);
}

void Kick::Kicking(void)
{
	std::string	kickNick =		package.cmdData[KICK_USER];
	std::string	kickChannel	=	package.cmdData[KICK_CHANNEL];
	package.channel =			package.client->getChannel(kickChannel);
	Client	*target	=			package.channel->getClient(kickNick);

	package.rplData = kickChannel;
	package.channel->removeClient(target->getNick());
}
