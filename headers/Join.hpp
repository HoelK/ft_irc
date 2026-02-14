# pragma once 
# include "RPL.hpp"
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Join
{
	public:
		static bool Check(void);
		static void	Create(Server &server);
		static void	Joining(Server &server);
};
