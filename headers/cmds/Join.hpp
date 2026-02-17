/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:44:11 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 20:44:12 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once 
# include "RPL.hpp"
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Join
{
	public:
		static bool Check(void);
		static void	Create(Server &server);
		static void	Joining(Server &server);
};
