# include "Invite.hpp"

bool	Invite::Check(Server &server)
{
	if (package.cmdData.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	invited =	package.cmdData[INVITE_NICK];
	std::string	chanName =	package.cmdData[INVITE_CHANNEL];
	if (!server.isClient(invited))
		return (package.setError(ERR_NOSUCHNICK), false);
	if (!server.isChannel(chanName))
		return (package.setError(ERR_NOSUCHCHANNEL), false);

	Channel *channel = server.getChannel(chanName);
	if (!channel->isClient(package.client->getNick()))
		return (package.setError(ERR_NOTONCHANNEL), false);
	if (channel->isClient(invited))
		return (package.setError(ERR_USERONCHANNEL), false);
	return (true);
}

void	Invite::Send(Server &server)
{
	std::string	invited =	package.cmdData[INVITE_NICK];
	package.channel	=		server.getChannel(package.cmdData[INVITE_CHANNEL]);

	package.channel->addInvited(server.getClient(invited));
}
