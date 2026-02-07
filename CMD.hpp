# pragma once
# include "Package.hpp"

# define CMD_NICK			"NICK"
# define CMD_USER			"USER"
# define NICK_NICK			0
# define USER_USERNAME		0
# define USER_REALNAME		3
# define PONG_DAEMON

class CMD
{
	public:
		static void	apply(void);
		static void	Nick(void);
		static void	User(void);
};
