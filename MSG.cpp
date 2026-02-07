# include "MSG.hpp"

static std::string getCmd(std::string const &line)
{ 
	char	sep = ' ';
	
	if (!line.find(sep))
		sep = 0;
	return (line.substr(0, line.find(sep)));
}

static std::vector<std::string>	getCmdData(std::string const &line)
{
	std::vector<std::string>	data;
	std::string					token;
	std::stringstream			stream;

	stream << line;
	stream >> token;
	while (!stream.eof())
	{
		stream >> token;
		if (token[0] == ':')
		{
			token.erase(0, 1);
			std::string	rest;
			std::getline(stream, rest);
			token = token + rest;
		}
		data.push_back(token);
	}
	return (data);
}

void	MSG::sendData(Client *client, std::string const &line)
{
	package.oldClient = *client;
	package.cmd = getCmd(line);
	package.data = getCmdData(line);
	package.client = client;
	package.error = 0;
}
