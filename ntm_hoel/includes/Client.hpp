/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:30:32 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/02 05:24:33 by sbonneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

#define MAX_BUFFER_SIZE 8192

class Client
{
    private:
        int _fd;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;
        bool _registered;
        bool _isOperator;
        bool _passed;
        std::string _buffer;
    public:
        Client(int fd);
        ~Client(void);

        int getFd(void) const;
        const std::string& getNick(void) const;
        const std::string& getUser(void) const;
        bool isRegistered(void) const;
        bool isOperator(void) const;
        bool isPassed(void) const;

        void setNick(const std::string& n);
        void setUser(const std::string& u);
        void setReal(const std::string& r);
        void setRegistered(bool v);
        void setOperator(bool v);
        void setPassed(bool v);

        void appendBuffer(const std::string& data);
        bool hasLine(void) const;
        std::string popLine(void);
};