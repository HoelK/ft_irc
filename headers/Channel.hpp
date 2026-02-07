/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:45:20 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/06 19:12:37 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <set>
# include <map>
# include "Client.hpp"

class Client;

class Channel
{
    private:
        std::string						name;
		std::map<std::string, Client>	clients;

    public:
		Channel(void);
		Channel(Channel const &copy);
		Channel &operator=(Channel const &copy);
        Channel(const std::string& name);
        ~Channel(void);

        const std::string& getName(void) const;

        void addClient(std::string const &name, Client &client);
        bool removeClient(std::string const &name);
};
