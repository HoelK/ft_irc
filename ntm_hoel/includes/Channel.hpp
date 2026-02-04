/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:45:20 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/03 01:56:39 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include <set>

class Channel {
    private:
        int								_userLimit;
        bool							_inviteOnly;
        bool							_topicRestricted;
        std::string						_name;
        std::string						_topic;
        std::string						_key;
        std::set<Client*>				_clients;
        std::set<Client*>				_operators;
        std::set<std::string>			_invited;
		std::map<std::string, Client*>	clients;

    public:
        Channel(const std::string& name);
        ~Channel(void);

        const std::string& getName(void) const;

        bool isMember(Client* c) const;
        bool hasClient(Client* c) const;
        bool isOperator(Client* c) const;

        void addClient(Client* c);
        void removeClient(Client* c);

        void addOperator(Client* c);
        void removeOperator(Client* c);

        void setTopic(const std::string& t);
        const std::string& getTopic() const;

        void setKey(const std::string& k);
        const std::string& getKey() const;

        void setInviteOnly(bool v);
        bool isInviteOnly(void) const;
        void setTopicRestricted(bool v);
        bool isTopicRestricted(void) const;
        void setUserLimit(int limit);

        bool canJoin(Client* c, const std::string& key) const;
        void invite(const std::string& nick);

        std::string getModes() const;
};
