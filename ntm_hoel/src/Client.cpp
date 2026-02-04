/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:32:49 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/03 17:31:00 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(int fd): _fd(fd) {
}

Client::~Client(void)
{
}

int Client::getFd(void) const
{
    return (this->_fd);
}

const std::string& Client::getNick(void) const
{
    return (this->_nickname);
}

const std::string& Client::getUser(void) const
{
    return (this->_username);
}

bool Client::isRegistered(void) const
{
    return (this->_registered);
}

bool Client::isOperator(void) const
{
    return (this->_isOperator);
}

bool Client::isPassed(void) const
{
    return (this->_passed);
}

void Client::setNick(const std::string& n)
{
    this->_nickname = n;
}

void Client::setUser(const std::string& u)
{
    this->_username = u;
}

void Client::setReal(const std::string& r)
{
    this->_realname = r;
}

void Client::setRegistered(bool v)
{
    this->_registered = v;
}

void Client::setOperator(bool v)
{
    this->_isOperator = v;
}

void Client::setPassed(bool v)
{
    this->_passed = v;
}

void Client::appendBuffer(const std::string& data)
{
    if (_buffer.size() + data.size() > MAX_BUFFER_SIZE)
    {
        size_t space_left = MAX_BUFFER_SIZE - _buffer.size();
        
        if (space_left > 0)
            _buffer.append(data, 0, space_left);
            
        return;
    }
    
    _buffer.append(data);
}

bool Client::hasLine(void) const
{
    // Chercher soit \r\n soit \n
    return (this->_buffer.find("\r\n") != std::string::npos || 
            this->_buffer.find("\n") != std::string::npos);
}

std::string Client::popLine(void)
{
    if (!this->hasLine())
        return ("");
    
    size_t pos_crlf = this->_buffer.find("\r\n");
    size_t pos_lf = this->_buffer.find("\n");
    size_t pos = std::string::npos;
    size_t erase_len = 1;
    
    // Prendre le plus petit position
    if (pos_crlf != std::string::npos && pos_lf != std::string::npos)
    {
        if (pos_crlf < pos_lf)
        {
            pos = pos_crlf;
            erase_len = 2;
        }
        else
        {
            pos = pos_lf;
            erase_len = 1;
        }
    }
    else if (pos_crlf != std::string::npos)
    {
        pos = pos_crlf;
        erase_len = 2;
    }
    else
    {
        pos = pos_lf;
        erase_len = 1;
    }
    
    std::string line = this->_buffer.substr(0, pos);
    this->_buffer.erase(0, pos + erase_len);
    
    return (line);
}
