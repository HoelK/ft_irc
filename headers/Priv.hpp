# pragma once
# include "Server.hpp"

class Priv
{
	public:
		static bool Check(Server &server);
		static void Send(Server &server);
};
