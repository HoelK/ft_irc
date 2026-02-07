/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:27 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 18:42:31 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Ft.hpp"
# include <iostream>

std::string Ft::extractLine(std::string &buffer)
{
	std::string	line;
	std::string	sep = "\n";

	if (buffer.empty())
		return ("");
	if (buffer.find(sep) == std::string::npos)
		sep = "\0";
	line = buffer.substr(0, buffer.find(sep));
	buffer.erase(0, buffer.find(sep) + 1);

	return (line);
}

std::string	Ft::getFdContent(const int fd)
{
	std::string	file;
	int			bytes;

	file.resize(BUFFER_SIZE);
	bytes = recv(fd, &file[0], BUFFER_SIZE, 0);
	if (bytes < 0)
		return ("");
	file.resize(bytes);
	return (file);
}
