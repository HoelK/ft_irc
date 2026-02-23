/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MSG.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:29 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 20:48:24 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "MSG.hpp"

std::string MSG::getCmd(std::string const &line)
{ 
	char	sep = ' ';
	
	if (line.find(sep) == std::string::npos)
		return (line.substr(0, line.length() - 2));
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
		if (stream.fail())
			break ;
		if (token[0] == ':')
		{
			token.erase(0, 1);
			std::string	rest;
			std::getline(stream, rest);
			token = token + rest;
			if (token[token.size() - 1] == '\r')
				token.erase(token.size() - 1);
		}
		if (!token.empty())
			data.push_back(token);
	}
	return (data);
}

void	MSG::sendData(Client *client, std::string const &line)
{
	package.clear();
	package.oldClient = *client;
	package.cmd = getCmd(line);
	package.cmdData = getCmdData(line);
	package.client = client;
	package.error = 0;
	package.quit = false;
}
