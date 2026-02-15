/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:06 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/15 17:46:17 by hkeromne         ###   ########.fr       */
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
	std::string	getFdContent(int const fd);
	std::string extractLine(std::string &buffer);
	bool		endsWithCRLF(std::string const &s);
	bool		isValidNickname(std::string const &nick);
	bool		isValidChannelName(std::string const &name);
	bool		isInt(const std::string &sNum);
	std::string	intToStr(int const &num);
}
