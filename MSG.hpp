# pragma once
# include <sstream>

class MSG
{
	public:
		static bool	getToCmd(std::stringstream &stream, std::string const cmd);
};
