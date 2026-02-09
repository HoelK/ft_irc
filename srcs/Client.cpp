/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:10 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/09 20:39:11 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Client.hpp"

Client::Client(void): fd(0), auth(false), op(false), channel(NULL) {};
Client::~Client(void) {};
Client::Client(int fd): fd(fd), auth(false), op(false), channel(NULL) {};
Client::Client(Client const &copy) { (*this) = copy; };

Client &Client::operator=(Client const &copy)
{
	this->fd = copy.fd;
	this->op = copy.op;
	this->nick = copy.nick;
	this->auth = copy.auth;
	this->username = copy.username;
	this->realname = copy.realname;
	this->password = copy.password;
	this->buffer = copy.buffer;
	this->channel = copy.channel;

	return (*this);
}

const bool			&Client::getOp(void) const { return (this->op); };
const int			&Client::getFd(void) const { return (this->fd); };
const bool			&Client::getAuth(void) const { return (this->auth); };
const std::string	&Client::getNick(void) const { return (this->nick); };
const std::string	&Client::getName(void) const { return (this->realname); };
const std::string	&Client::getUser(void) const { return (this->username); };
const std::string	&Client::getPass(void) const { return (this->password); };
const std::string	&Client::getBuffer(void) const { return (this->buffer); };
Channel				*Client::getChannel(void) const { return (this->channel); };
void				Client::setAuth(bool auth) { this->auth = auth; };
void				Client::setNick(std::string const &nick) { this->nick = nick; };
void				Client::setName(std::string const &name) { this->realname = name; };
void				Client::setUser(std::string const &user) { this->username = user; };
void				Client::setPass(std::string const &pass) { this->password = pass; };
void				Client::setBuffer(std::string const &buff) { this->buffer = buff; };
void				Client::setOp(bool op) { this->op = op; };
void				Client::setChannel(Channel *channel) { this->channel = channel; };

bool				Client::isAuth(std::string const &password)
{
	return (!this->nick.empty()
			&& !this->username.empty()
			&& !this->realname.empty()
			&& this->password == password);
}
