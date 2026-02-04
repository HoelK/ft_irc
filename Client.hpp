/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:30:32 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/04 01:10:02 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

#define MAX_BUFFER_SIZE 8192

class Client
{
    private:
        int			fd;
        std::string	nick;
        std::string	realname;
        std::string	buffer;

    public:
		Client(void);
        Client(int fd);
		Client(Client const &copy);
		Client &operator=(Client const &copy);
        ~Client(void);

        const int			&getFd(void) const;
        const std::string&	getNick(void) const;
		void				setNick(std::string const &nick);
        const std::string&	getName(void) const;
		void				setName(std::string const &name);
};
