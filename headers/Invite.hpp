# pragma once
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Invite
{
	public:
		static bool	Check(Server &server);
		static void	Send(Server &server);
};
