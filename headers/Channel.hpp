/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:45:20 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:49 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <set>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include "Client.hpp"

class Client;

class Channel
{
    private:
        std::string						name;
		std::map<std::string, Client *>	clients;

    public:
		Channel(void);
		Channel(Channel const &copy);
		Channel &operator=(Channel const &copy);
        Channel(const std::string& name);
        ~Channel(void);

        const std::string&	getName(void) const;
		void				setName(std::string const &name);
		std::string			getNameList(void) const;
		bool				isClient(std::string const &nick);
		Client				*getClient(std::string const &nick);
        void				addClient(Client *client);
        bool				removeClient(std::string const &name);

		void				broadcastMessage(Client *sender, std::string const &msg);
};

std::ostream &operator<<(std::ostream &stream, Channel const &channel);
