/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:06 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/12 20:47:46 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <sstream>
# include <sys/socket.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

namespace Ft
{
	std::string	getFdContent(const int fd);
	std::string extractLine(std::string &buffer);
	bool		endsWithCRLF(const std::string& s);
	bool		isValidNickname(const std::string &nick);
	bool		isInt(const std::string &sNum);
	std::string	intToStr(const int &num);
}

