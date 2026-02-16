# include "Topic.hpp"

bool Topic::Check(Server &server)
{
	if (package.cmdData.size() < 2)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	topicChan = package.cmdData[TOPIC_CHANNEL];
	if (!server.isChannel(topicChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	return (true);
}

void Topic::Set(Server &server)
{
	std::string	topicChan	= package.cmdData[TOPIC_CHANNEL];
	std::string newTopic	= package.cmdData[TOPIC_NEW];
	Channel		*channel	= server.getChannel(topicChan);

	channel->setTopic(newTopic);
	package.channel		= channel;
	package.rplData	= newTopic;
}
