# include "Errors.hpp"

std::string	codeStr(short code)
{
	std::stringstream	s;
	std::string			res;

	s << code;
	s >> res;
	if (code < 10)
		res = "00" + s.str();
	else if (code < 100)
		res = "0" + s.str();

	return (res);
}

std::string	codeToErr(short code)
{
	if (code == ERR_PASSWDMISMATCH)
		return (ERR_PASSWDMISMATCH_STR);
	else if (code == ERR_NEEDMOREPARAMS)
		return (ERR_NEEDMOREPARAMS_STR(package.cmd));
	else if (code == ERR_USERNOTINCHANNEL)
		return (ERR_USERNOTINCHANNEL_STR(package.errNick, package.errChanName));
	else if (code == ERR_NOSUCHCHANNEL)
		return (ERR_NOSUCHCHANNEL_STR(package.errChanName));
	else if (code == ERR_NOSUCHNICK)
		return (ERR_NOSUCHNICK_STR(package.errNick));
	else if (code == ERR_USERONCHANNEL)
		return (ERR_USERONCHANNEL_STR(package.errNick, package.errChanName));
	else if (code == ERR_NONICKNAMEGIVEN)
		return (ERR_NONICKNAMEGIVEN_STR);
	else if (code == ERR_ONEUSNICKNAME)
		return (ERR_ONEUSNICKNAME_STR(package.errNick));
	else if (code == ERR_NICKNAMEINUSE)
		return (ERR_NICKNAMEINUSE_STR(package.errNick));
	else if (code == ERR_NOTONCHANNEL)
		return (ERR_NOTONCHANNEL_STR(package.errChanName));
	else if (code == ERR_ALREADYREGISTRED)
		return (ERR_ALREADYREGISTRED_STR);
	else if (code == ERR_CHANNELISFULL)
		return (ERR_CHANNELISFULL_STR(package.channel->getName()));
	else if (code == ERR_INVITEONLYCHAN)
		return (ERR_INVITEONLYCHAN_STR(package.channel->getName()));
	else if (code == ERR_BADCHANNELKEY)
		return (ERR_BADCHANNELKEY_STR(package.channel->getName()));
	else if (code == ERR_CHANOPRIVSNEEDED)
		return (ERR_CHANOPRIVSNEEDED_STR(package.errChanName));
	else if (code == ERR_UNKNOWNMODE)
		return (ERR_UNKNOWNMODE_STR(package.errMode));
	
	return ("");
}

std::string	getError(short code, std::string const &nick) { return (HEADER_ERROR(codeStr(code), nick) + codeToErr(code)); };
