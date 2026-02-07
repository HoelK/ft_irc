/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:27 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 20:04:57 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Ft.hpp"
# include <iostream>


bool Ft::endsWithCRLF(const std::string& s)
{
    if (s.size() < 2)
		return false;
    return (s.substr(s.size() - 2) == "\r\n");
}

std::string Ft::extractLine(std::string &buffer)
{
	std::size_t	pos;
	std::string	sep("\r\n");
	std::string line;

	if (buffer.empty())
		return ("");
	pos = buffer.find(sep);
	pos = (pos == std::string::npos) ? buffer.length() : pos + 2;
	line = buffer.substr(0, pos);
	buffer.erase(0, pos);

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
