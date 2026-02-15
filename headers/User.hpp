# pragma once
# include "Server.hpp"

class User
{
	public:
		static bool Check(Server &server);
		static void Set(void);
};
