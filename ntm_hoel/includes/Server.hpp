/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:46:48 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/02 05:01:04 by sbonneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

class Server {
    private:
        int _port;
        std::string _password;
        int _serverFd;

        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
    public:
        Server(int port, const std::string& pass);
        ~Server(void);

        void run(void);

        void acceptClient(void);
        void removeClient(int fd);

        Client* getClient(int fd);
        Client* getClientByNick(const std::string& nick);

        Channel* getChannel(const std::string& name);
        Channel* createChannel(const std::string& name);
        void deleteChannel(const std::string& name);

        void handleCommand(Client* c, const std::string& line);

        void cmdPASS(Client* c, std::vector<std::string>& args);
        void cmdNICK(Client* c, std::vector<std::string>& args);
        void cmdUSER(Client* c, std::vector<std::string>& args);
        void cmdJOIN(Client* c, std::vector<std::string>& args);
        void cmdPART(Client* c, std::vector<std::string>& args);
        void cmdPRIVMSG(Client* c, std::vector<std::string>& args);
        void cmdMODE(Client* c, std::vector<std::string>& args);
        void cmdTOPIC(Client* c, std::vector<std::string>& args);
        void cmdINVITE(Client* c, std::vector<std::string>& args);
        void cmdKICK(Client* c, std::vector<std::string>& args);
};