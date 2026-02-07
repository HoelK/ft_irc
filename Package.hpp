# pragma once
# include <string>
# include <vector>
# include "Client.hpp"

typedef struct	s_package
{
	std::string					cmd;
	std::vector<std::string>	data;
	Client						oldClient;
	Client						*client;
	int							error;
}	t_package;

extern t_package package;
