# pragma once
# include <string>
# include <vector>
# include <sstream>
# include "Ft.hpp"
# include "Client.hpp"
# include "Package.hpp"

class MSG
{
	public:
		static void	sendData(Client *client, std::string const &line);
};
