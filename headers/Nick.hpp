# pragma once
# include "Server.hpp"

class Server;

class Nick
{
	public:
		static bool Check(Server &server);
		static void Change(void);
};
