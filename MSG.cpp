# include "MSG.hpp"

bool	MSG::getToCmd(std::stringstream &stream, std::string const cmd)
{
	std::string	token;

	while (token != cmd && !stream.eof())
		stream >> token;
	return (token == cmd);
}
