/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:44:11 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 02:07:49 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once 
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
