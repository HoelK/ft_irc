/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:30:32 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/07 04:16:31 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include "Channel.hpp"
# define MAX_BUFFER_SIZE 8192

class Channel;

class Client
{
    private:
		unsigned int	id;
        int				fd;
        std::string		nick;
		std::string		username;
        std::string		realname;
        std::string		buffer;
		Channel			*channel;

    public:
		Client(void);
        Client(int fd);
		Client(Client const &copy);
		Client &operator=(Client const &copy);
        ~Client(void);

        const int			&getFd(void) const;
		const unsigned int	&getId(void) const;
        const std::string&	getNick(void) const;
        const std::string&	getName(void) const;
		const std::string&	getUser(void) const;
		const std::string&	getBuffer(void) const;
		void				setId(const int id);
		void				setNick(std::string const &nick);
		void				setName(std::string const &name);
		void				setUser(std::string const &name);
		void				setBuffer(std::string const &buff);
};
