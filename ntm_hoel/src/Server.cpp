/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:50:45 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/02 05:24:20 by sbonneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(int port, const std::string& pass): _port(port), _password(pass), _serverFd(-1)
{
    (void)_port;
    std::cout << "[Server] Initializing IRC Server on port " << port << "..." << std::endl;
    
    this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverFd < 0)
        throw std::runtime_error("Failed to create socket");
    
    int opt = 1;
    if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(this->_serverFd);
        throw std::runtime_error("Failed to set socket options");
    }
    
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(this->_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(this->_serverFd);
        throw std::runtime_error("Failed to bind socket");
    }
    
    if (listen(this->_serverFd, 10) < 0)
    {
        close(this->_serverFd);
        throw std::runtime_error("Failed to listen on socket");
    }
    
    std::cout << "[Server] Server ready and listening on port " << port << std::endl;
}

Server::~Server(void)
{
    std::cout << "[Server] Server destroyed" << std::endl;
    for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
        delete it->second;
    for (std::map<std::string, Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
        delete it->second;
    if (this->_serverFd >= 0)
        close(this->_serverFd);
}

void Server::run(void)
{
    extern bool g_running;
    
    std::cout << "[Server] Server running, waiting for connections..." << std::endl;
    
    while (g_running)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(this->_serverFd, &read_fds);
        
        int max_fd = this->_serverFd;
        
        for (std::map<int, Client*>::iterator it = this->_clients.begin();
             it != this->_clients.end(); ++it)
        {
            FD_SET(it->first, &read_fds);
            if (it->first > max_fd)
                max_fd = it->first;
        }
        
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (activity < 0 && g_running)
        {
            std::cerr << "[Server] Select error" << std::endl;
            continue;
        }
        
        if (!g_running)
            break;
        
        if (FD_ISSET(this->_serverFd, &read_fds))
        {
            this->acceptClient();
        }
        
        std::vector<int> to_remove;
        for (std::map<int, Client*>::iterator it = this->_clients.begin();
             it != this->_clients.end(); ++it)
        {
            if (FD_ISSET(it->first, &read_fds))
            {
                char buffer[MAX_BUFFER_SIZE];
                int bytes = recv(it->first, buffer, MAX_BUFFER_SIZE - 1, 0);
                
                if (bytes <= 0)
                {
                    to_remove.push_back(it->first);
                }
                else
                {
                    buffer[bytes] = '\0';
                    it->second->appendBuffer(std::string(buffer));
                    
                    while (it->second->hasLine())
                    {
                        std::string line = it->second->popLine();
                        this->handleCommand(it->second, line);
                    }
                }
            }
        }
        
        for (size_t i = 0; i < to_remove.size(); ++i)
            this->removeClient(to_remove[i]);
    }
    
    std::cout << "[Server] Shutting down..." << std::endl;
}

void Server::acceptClient(void)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(this->_serverFd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0)
    {
        std::cerr << "[Server] Failed to accept client" << std::endl;
        return;
    }
    
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    
    Client* new_client = new Client(client_fd);
    this->_clients[client_fd] = new_client;
    
    std::cout << "[\033[32m+\033[0m] Client fd=" << client_fd << " connected" << std::endl;
}

void Server::removeClient(int fd)
{
    Client* c = this->getClient(fd);
    
    if (!c)
        return;

    for (std::map<std::string, Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
        it->second->removeClient(c);
    
    this->_clients.erase(fd);
    close(fd);
    delete c;
    std::cout << "[\033[31m-\033[0m] Client fd=" << fd << " disconnected" << std::endl;
}

Client* Server::getClient(int fd)
{
    std::map<int, Client*>::iterator it = this->_clients.find(fd);
    if (it == this->_clients.end())
        return (NULL);
    return (it->second);
}

Client* Server::getClientByNick(const std::string& nick)
{
    for (std::map<int, Client*>::iterator it = this->_clients.begin(); 
         it != this->_clients.end(); ++it)
    {
        if (it->second->getNick() == nick)
            return (it->second);
    }
    return (NULL);
}

Channel* Server::getChannel(const std::string& name)
{
    std::map<std::string, Channel*>::iterator it = this->_channels.find(name);
    if (it == this->_channels.end())
        return (NULL);
    return (it->second);
}

Channel* Server::createChannel(const std::string& name)
{
    if (this->getChannel(name))
        return (this->getChannel(name));
    
    Channel* c = new Channel(name);
    this->_channels[name] = c;
    std::cout << "[\033[36m+\033[0m] New channel " << name << std::endl;
    return (c);
}

void Server::deleteChannel(const std::string& name)
{
    Channel* c = this->getChannel(name);
    if (!c)
        return;
    
    this->_channels.erase(name);
    delete c;
}

void Server::handleCommand(Client* c, const std::string& line)
{
    if (!c || line.empty())
        return;
    
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    
    std::vector<std::string> args;
    std::string arg;
    
    while (iss >> arg)
    {
        if (!arg.empty() && arg[0] == ':')
        {
            std::string trailing = arg.substr(1);
            std::string rest;
            if (std::getline(iss, rest))
                trailing += rest;
            args.push_back(trailing);
            break;
        }
        args.push_back(arg);
    }
    
    if (cmd == "PASS")
        this->cmdPASS(c, args);
    else if (cmd == "NICK")
        this->cmdNICK(c, args);
    else if (cmd == "USER")
        this->cmdUSER(c, args);
    else if (cmd == "JOIN")
        this->cmdJOIN(c, args);
    else if (cmd == "PART")
        this->cmdPART(c, args);
    else if (cmd == "PRIVMSG")
        this->cmdPRIVMSG(c, args);
    else if (cmd == "MODE")
        this->cmdMODE(c, args);
    else if (cmd == "TOPIC")
        this->cmdTOPIC(c, args);
    else if (cmd == "INVITE")
        this->cmdINVITE(c, args);
    else if (cmd == "KICK")
        this->cmdKICK(c, args);
}

void Server::cmdPASS(Client* c, std::vector<std::string>& args)
{
    if (args.empty())
        return;
    
    if (args[0] == this->_password)
    {
        c->setPassed(true);
        std::cout << "[\033[33m*\033[0m] PASS authentication successful" << std::endl;
    }
    else
    {
        std::cout << "[\033[33m*\033[0m] PASS authentication failed" << std::endl;
    }
}

void Server::cmdNICK(Client* c, std::vector<std::string>& args)
{
    if (args.empty())
        return;
    
    const std::string& nick = args[0];
    
    if (this->getClientByNick(nick) && this->getClientByNick(nick) != c)
    {
        return;
    }
    
    c->setNick(nick);
    std::cout << "[\033[35m~\033[0m] " << nick << " set as nickname" << std::endl;
}

void Server::cmdUSER(Client* c, std::vector<std::string>& args)
{
    if (args.size() < 4)
    {
        return;
    }
    
    c->setUser(args[0]);
    c->setReal(args[3]);
    
    if (!c->getNick().empty() && !c->getUser().empty())
        c->setRegistered(true);
    
    std::cout << "[\033[35m~\033[0m] " << args[0] << " registered" << std::endl;
}

void Server::cmdJOIN(Client* c, std::vector<std::string>& args)
{
    if (args.empty() || !c->isRegistered())
        return;
    
    const std::string& channel_name = args[0];
    std::string key = (args.size() > 1) ? args[1] : "";
    
    Channel* ch = this->createChannel(channel_name);
    
    if (!ch->canJoin(c, key))
        return;
    
    ch->addClient(c);
    std::cout << "[\033[36m#\033[0m] " << c->getNick() << " joined " << channel_name << std::endl;
}

void Server::cmdPART(Client* c, std::vector<std::string>& args)
{
    if (args.empty())
        return;
    
    const std::string& channel_name = args[0];
    Channel* ch = this->getChannel(channel_name);
    
    if (!ch)
        return;
    
    ch->removeClient(c);
    std::cout << "[\033[36m#\033[0m] " << c->getNick() << " left " << channel_name << std::endl;

    if (ch->getModes().find('+') == std::string::npos)
    {
        this->deleteChannel(channel_name);
        std::cout << "[\033[36m-\033[0m] Channel " << channel_name << " deleted" << std::endl;
    }
}

void Server::cmdPRIVMSG(Client* c, std::vector<std::string>& args)
{
    if (args.size() < 2 || !c->isRegistered())
        return;
    
    const std::string& target = args[0];
    (void)args[1]; // message - sera utilisé pour l'envoi réel
    
    // Vérifier si c'est un channel ou un client
    if (target[0] == '#')
    {
        Channel* ch = this->getChannel(target);
        if (ch && ch->hasClient(c))
        {
            // Message envoyé au channel
        }
    }
    else
    {
        Client* target_client = this->getClientByNick(target);
        if (target_client)
        {
            // Message envoyé au client
        }
    }
}

void Server::cmdMODE(Client* c, std::vector<std::string>& args)
{
    if (args.empty())
        return;
    
    const std::string& target = args[0];
    Channel* ch = this->getChannel(target);
    
    if (!ch || !ch->isOperator(c))
        return;
    
    std::cout << "[\033[33m*\033[0m] MODE changed on " << target << std::endl;
}

void Server::cmdTOPIC(Client* c, std::vector<std::string>& args)
{
    if (args.empty() || !c->isRegistered())
        return;
    
    const std::string& channel_name = args[0];
    Channel* ch = this->getChannel(channel_name);
    
    if (!ch || !ch->hasClient(c))
        return;
    
    if (args.size() > 1)
    {
        if (!ch->isOperator(c) && ch->isTopicRestricted())
            return;
        ch->setTopic(args[1]);
        std::cout << "[\033[33m*\033[0m] Topic set on " << channel_name << ": " << args[1] << std::endl;
    }
}

void Server::cmdINVITE(Client* c, std::vector<std::string>& args)
{
    if (args.size() < 2)
        return;
    
    const std::string& nick = args[0];
    const std::string& channel_name = args[1];
    
    Channel* ch = this->getChannel(channel_name);
    
    if (!ch || !ch->isOperator(c))
        return;
    
    ch->invite(nick);
    std::cout << "[\033[33m*\033[0m] " << nick << " invited to " << channel_name << std::endl;
}

void Server::cmdKICK(Client* c, std::vector<std::string>& args)
{
    if (args.size() < 2 || !c->isRegistered())
        return;
    
    const std::string& channel_name = args[0];
    const std::string& nick = args[1];
    std::string reason = (args.size() > 2) ? args[2] : "";
    
    Channel* ch = this->getChannel(channel_name);
    
    if (!ch || !ch->isOperator(c))
        return;
    
    Client* target = this->getClientByNick(nick);
    if (target && ch->hasClient(target))
        ch->removeClient(target);
    
    std::cout << "[\033[31m!\033[0m] " << nick << " kicked from " << channel_name << std::endl;
}

