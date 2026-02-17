/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Priv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:45:15 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 20:45:16 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Server.hpp"

class Server;

class Priv
{
	public:
		static bool Check(Server &server);
		static void Send(Server &server);
};
