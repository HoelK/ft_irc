/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:45:20 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/16 20:25:14 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <set>
# include <map>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include "Ft.hpp"
# include "Client.hpp"

# define MODE_OP		'o'
# define MODE_KEY		'k'
# define MODE_TOPIC		't'
# define MODE_LIMIT		'l'
# define MODE_INVITE	'i'

class Client;

class Channel
{
    private:
		bool							opKey;
		bool							opTopic;
		int								opLimit;
		bool							opInvite;
        std::string						name;
		std::string						topic;
		std::string						password;
		std::vector<Client *>			invited;
		std::vector<Client *>			operators;
		std::map<std::string, Client *>	clients;

    public:
		Channel(void);
		Channel(Channel const &copy);
		Channel &operator=(Channel const &copy);
        Channel(const std::string& name);
        ~Channel(void);

		const bool			&getOpKey(void) const;
		const bool			&getOpTopic(void) const;
		const bool			&getOpInvite(void) const;
		const int			&getOpLimit(void) const;
		const std::string	&getPassword(void) const;
        const std::string	&getName(void) const;
		const std::string	&getTopic(void) const;
		void				setTopic(std::string const &topic);
		std::string			getNameList(void) const;
		void				setOpKey(bool const &key, std::string const &password);
		void				setOpTopic(bool const &topic);
		void				setOpInvite(bool const &invite);
		void				setOpLimit(int const &limit);
		void				setName(std::string const &name);

		bool				Auth(std::string const &password);
		bool				isFull(void);

		bool				isClient(std::string const &nick);
		Client				*getClient(std::string const &nick);
        void				addClient(Client *client);
        bool				removeClient(std::string const &name);
		void				updateClient(std::string const &oldNick);

		void				addInvited(Client *client);
		void				removeInvited(std::string const &nick);
		bool				isInvited(std::string const &nick);

		void				addOperator(Client *client);
		void				removeOperator(std::string const &nick);
		bool				isOperator(std::string const &nick) const;

		std::string			getModes(void);

		void				broadcastMessage(Client *sender, std::string const &msg);
};

std::ostream &operator<<(std::ostream &stream, Channel const &channel);
