# include "RPL.hpp"

std::string		RPL::getMessage(short code, std::string nick)
{
	if (code == RPL_WELCOME)
		return (RPL_WELCOME_STR(nick));
	else if (code == RPL_YOURHOST)
		return (RPL_YOURHOST_STR);
	else if (code == RPL_CREATED)
		return (RPL_CREATED_STR);
	else if (code == RPL_MYINFO)
		return (RPL_MYINFO_STR(nick));
	else if (code == RPL_ISUPPORT)
		return (RPL_ISUPPORT_STR);

	return ("");
}

std::string		RPL::code_to_string(short code)
{
	std::stringstream	stream;

	if (code < 10)
		stream << "00";
	else if (code >= 10 && code < 100)
		stream << "0";
	stream << code;
	return (stream.str());
}

std::string		RPL::createMessage(short code, std::string nick)
{
	std::stringstream	stream;

	stream << RPL_HEADER(code_to_string(code), nick);
	stream << RPL::getMessage(code, nick);
	stream << "\r\n";

	return (stream.str());
}


void RPL::connection(int fd, std::string const &nick)
{
	std::string sent_message = RPL::createMessage(RPL_WELCOME,	nick);
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_YOURHOST, nick);
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_CREATED,	nick);
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_MYINFO,	nick);
	send(fd, sent_message.c_str(), sent_message.size(), 0);
	sent_message = RPL::createMessage(RPL_ISUPPORT, nick);
	send(fd, sent_message.c_str(), sent_message.size(), 0);
}
