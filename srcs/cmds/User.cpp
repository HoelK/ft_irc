/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:43:49 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/20 00:08:55 by dedavid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include "Errors.hpp"

bool User::Check(Server &server)
{
	(void) server;

	if (package.cmdData.size() < 4)
		return (package.setError(ERR_NEEDMOREPARAMS), false);
	if (package.client->getAuth())
		return (package.setError(ERR_ALREADYREGISTRED), false);
	return (true);
}

void User::Set(void)
{
	package.client->setUser(package.cmdData[USER_USERNAME]);
	package.client->setName(package.cmdData[USER_REALNAME]);
}
