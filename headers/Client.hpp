/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:30:32 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/10 21:23:55 by hkeromne         ###   ########.fr       */
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
        int								fd;
		bool							auth;
		bool							op;
        std::string						nick;
		std::string						username;
        std::string						realname;
		std::string						password;
        std::string						buffer;
		std::map<std::string, Channel *> channels;

    public:
		Client(void);
        Client(int fd);
		Client(Client const &copy);
		Client &operator=(Client const &copy);
        ~Client(void);

		const bool			&getOp(void)		const;
        const int			&getFd(void)		const;
		const bool			&getAuth(void)		const;
        const std::string	&getNick(void)		const;
        const std::string	&getName(void)		const;
		const std::string	&getUser(void)		const;
		const std::string	&getPass(void)		const;
		const std::string	&getBuffer(void)	const;
		Channel				*getChannel(std::string const &topic) const;
		void				setOp(bool op);
		void				setAuth(bool auth);
		void				setNick(std::string const &nick);
		void				setName(std::string const &name);
		void				setUser(std::string const &name);
		void				setPass(std::string const &pass);
		void				setBuffer(std::string const &buff);
		const std::map<std::string, Channel *> &getChannels(void) const;

		bool				isChannel(std::string const &topic);
		bool				isAuth(std::string const &password);
		void				addChannel(Channel *channel);
		void				delChannel(std::string const &topic);
		bool				inChannel(void) const;
		void				updateChannel(std::string const &oldTopic);
};

std::ostream &operator<<(std::ostream &stream, Client const &client);
std::ostream &operator<<(std::ostream &stream, std::map<std::string, Channel *> const &channels);
