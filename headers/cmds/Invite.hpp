/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:42:43 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 20:42:44 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Invite
{
	public:
		static bool	Check(Server &server);
		static void	Send(Server &server);
};
