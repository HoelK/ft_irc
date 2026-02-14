# include "Topic.hpp"

bool Topic::Check(Server &server)
{
	if (package.cmd_data.size() < 3)
		return (package.setError(ERR_NEEDMOREPARAMS), false);

	std::string	topicChan = package.cmd_data[TOPIC_CHANNEL];
	if (!server.isChannel(topicChan))
		return (package.setError(ERR_NOSUCHCHANNEL), false);
	return (true);
}

void Topic::Set(Server &server)
{
	std::string	topicChan	= package.cmd_data[TOPIC_CHANNEL];
	std::string newTopic	= package.cmd_data[TOPIC_NEW];
	Channel		*channel	= server.getChannel(topicChan);

	channel->setTopic(newTopic);
	package.channel		= channel;
	package.rpl_data	= newTopic;
}
