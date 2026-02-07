# pragma once
# include <string>
# include <sys/socket.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

namespace Ft
{
	std::string extractLine(std::string &buffer);
	std::string	getFdContent(const int fd);
}

