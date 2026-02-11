/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ft.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:06 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/11 04:12:29 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
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
}

