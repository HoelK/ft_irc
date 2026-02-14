# include "Invite.hpp"

bool	Invite::Check(Server &server)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	invited =	package.cmd_data[INVITE_NICK];
	std::string	chanName =	package.cmd_data[INVITE_CHANNEL];
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
	std::string	invited =	package.cmd_data[INVITE_NICK];
	package.channel	=		server.getChannel(package.cmd_data[INVITE_CHANNEL]);

	package.channel->addInvited(server.getClient(invited));
}
