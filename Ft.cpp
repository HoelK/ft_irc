# include "Ft.hpp"
# include <iostream>

std::string Ft::extractLine(std::string &buffer)
{
	std::string	line;
	char		sep = '\n';

	if (!buffer.find(sep) || buffer.empty())
		sep = 0;
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
