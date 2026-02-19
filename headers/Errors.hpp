# include "RPL.hpp"

# pragma once
# define HEADER_ERROR(code, nick)					":" SERVER_NAME " " + std::string(code) + " " + std::string(nick) + " "

# define ERR_NOSUCHNICK_STR(nick)					std::string(nick) + " :No such nick"
# define ERR_PASSWDMISMATCH_STR						":Password incorrect"
# define ERR_NONICKNAMEGIVEN_STR					":No nickname given"
# define ERR_ONEUSNICKNAME_STR(nick)				std::string(nick) +	" :Erroneous nickname"
# define ERR_NICKNAMEINUSE_STR(nick)				std::string(nick) +	" :Nickname is already in use"
# define ERR_NEEDMOREPARAMS_STR(cmd)				std::string(cmd) +	" :Not enough parameters"
# define ERR_USERNOTINCHANNEL_STR(nick, channel)	std::string(nick) + " " + std::string(channel) + " :They aren't on that channel"
# define ERR_USERONCHANNEL_STR(user, channel)		std::string(user) + " " + std::string(channel) + " :is already on channel"
# define ERR_INVITEONLYCHAN_STR(channel)			std::string(channel) + " :Cannot join channel (+i)" 
# define ERR_CHANNELISFULL_STR(channel)				std::string(channel) + " :Cannot join channel (+l)"
# define ERR_BADCHANNELKEY_STR(channel)				std::string(channel) + " :Cannot join channel (+k)"
# define ERR_NOSUCHCHANNEL_STR(channel)				std::string(channel) + " :No such channel"
# define ERR_CHANOPRIVSNEEDED_STR(channel)			std::string(channel) + " :You're not channel operator"
# define ERR_NOTONCHANNEL_STR(channel)				std::string(channel) + " :You're not on that channel"
# define ERR_UNKNOWNMODE_STR(mode)					std::string(mode) + " :is unknown mode char to me"
# define ERR_ALREADYREGISTRED_STR					":Unauthorized command (already registered)"

enum ERR_ID
{
	ERR_NOSUCHNICK =		401,
	ERR_NOSUCHCHANNEL =		403,
	ERR_NONICKNAMEGIVEN =	431,
	ERR_ONEUSNICKNAME =		432,
	ERR_NICKNAMEINUSE =		433,
	ERR_USERNOTINCHANNEL =	441,
	ERR_NOTONCHANNEL =		442,
	ERR_USERONCHANNEL =		443,
	ERR_NEEDMOREPARAMS =	461,
	ERR_ALREADYREGISTRED =	462,
	ERR_PASSWDMISMATCH =	464,
	ERR_CHANNELISFULL =		471,
	ERR_UNKNOWNMODE =		472,
	ERR_INVITEONLYCHAN =	473,
	ERR_BADCHANNELKEY =		475,
	ERR_CHANOPRIVSNEEDED =	482
};

std::string	codeStr(short code);
std::string	codeToErr(short code);
std::string	getError(short code, std::string const &nick);
