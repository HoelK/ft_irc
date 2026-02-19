/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:30:32 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/18 01:37:28 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <map>
# include "poll.h"
# include <iostream>
# define MAX_BUFFER_SIZE 8192

class Server;
class Channel;

class Client
{
    private:
		bool							auth;
		struct pollfd					*pfd;
        std::string						nick;
		std::string						username;
        std::string						realname;
		std::string						password;
        std::string						recvBuffer;
		std::string						sendBuffer;
		std::map<std::string, Channel *> channels;

    public:
		Client(void);
        Client(struct pollfd *pfd);
		Client(Client const &copy);
		Client &operator=(Client const &copy);
        ~Client(void);

        const int			&getFd(void)		const;
		const bool			&getAuth(void)		const;
        const std::string	&getNick(void)		const;
        const std::string	&getName(void)		const;
		const std::string	&getUser(void)		const;
		const std::string	&getPass(void)		const;
		const std::string	&getSendBuffer(void)const;
		const std::string	&getRecvBuffer(void)const;
		Channel				*getChannel(std::string const &topic) const;
		void				setAuth(bool auth);
		void				setNick(std::string const &nick);
		void				setName(std::string const &name);
		void				setUser(std::string const &name);
		void				setPass(std::string const &pass);
		void				appendSendBuffer(std::string const &buff);
		void				appendRecvBuffer(std::string const &buff);
		void				clearSendBuffer(void);
		void				clearRecvBuffer(void);
		const std::map<std::string, Channel *> &getChannels(void) const;
		void				sendMsg(void);

		bool				isChannel(std::string const &topic);
		bool				isAuth(std::string const &password);
		void				addChannel(Channel *channel);
		void				delChannel(std::string const &topic);
		bool				inChannel(void) const;
		void				updateChannel(std::string const &oldTopic);
		void				updateInChannel(std::string const &oldNick);
		void				disconnection(Server &server);
		void				broadcastMsg(std::string const &msg);
};

std::ostream &operator<<(std::ostream &stream, Client const &client);
std::ostream &operator<<(std::ostream &stream, std::map<std::string, Channel *> const &channels);
