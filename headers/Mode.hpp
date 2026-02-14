# pragma once
# include "Ft.hpp"
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Mode
{
	public :
		static std::map<char, int (*)(Server &, bool, int)> mode;
		static int	Check(Server &server, std::string const &modes);
		static int	i(Server &server, bool add, int argCount);
		static int	t(Server &server, bool add, int argCount);
		static int	l(Server &server, bool add, int argCount);
		static int	k(Server &server, bool add, int argCount);
		static int	o(Server &server, bool add, int argCount);
};
