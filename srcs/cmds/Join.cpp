# include "Join.hpp"

static bool checkInvite(Channel *channel)
{
	if (!channel->getOpInvite())
		return (true);
	if (!channel->isInvited(package.client->getNick()))
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	return (true);
}

static bool	checkKey(Channel *channel)
{
	if (!channel->getOpKey())
		return (true);
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	if (!channel->Auth(package.cmdData[JOIN_KEY]))
		return (package.setError(ERR_BADCHANNELKEY), false);
	return (true);
}

bool	Join::Check()
{
	if (package.cmdData.size() < 1)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	return (true);
}

void	Join::Joining(Server &server)
{
	Channel		*channel;
	std::string	joinChannel;

	joinChannel	= package.cmdData[JOIN_CHANNEL];
	channel		= server.getChannel(joinChannel);

	if (channel->isFull())
		return (package.setError(ERR_CHANNELISFULL));
	if (!checkInvite(channel) || !checkKey(channel))
		return ;

	channel->addClient(package.client);
	package.client->addChannel(channel);
	package.channel = channel;
}

void	Join::Create(Server &server)
{
	Channel		channel;
	std::string	joinChannel = package.cmdData[JOIN_CHANNEL];

	channel.addOperator(package.client);
	channel.setName(joinChannel);
	channel.addClient(package.client);
	server.createChannel(channel);
	package.client->addChannel(server.getChannel(joinChannel));
	package.channel = package.client->getChannel(joinChannel);
}
