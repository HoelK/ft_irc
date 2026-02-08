/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:27:59 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 22:00:11 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD.hpp"

static std::map<std::string, void (*)()> cmds = {
	{CMD_NICK, &CMD::Nick},
	{CMD_USER, &CMD::User},
	{CMD_PASS, &CMD::Pass},
	{CMD_QUIT, &CMD::Quit}
};

void	CMD::apply(void)
{
	std::map<std::string, void (*)()>::iterator it;

	it = cmds.find(package.cmd);
	if (it == cmds.end())
		return ;
	it->second();
}

void	CMD::Pass(void) { package.client->setPass(package.cmd_data[PASS_PASS]); };
void	CMD::Nick(void) { package.client->setNick(package.cmd_data[NICK_NICK]); package.rpl_data = package.client->getNick(); };
void	CMD::Quit(void) { package.quit = true; package.rpl_data = package.cmd_data[QUIT_REASON]; };
void	CMD::User(void)
{
	package.client->setUser(package.cmd_data[USER_USERNAME]);
	package.client->setName(package.cmd_data[USER_REALNAME]);
}
