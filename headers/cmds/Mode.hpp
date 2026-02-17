/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:44:56 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 20:44:57 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Ft.hpp"
# include "Server.hpp"
# include "Package.hpp"

class Server;

class Mode
{
	public :
		static std::map<char, int (*)(Server &, bool, int)> mode;
		static int	Check(Server &server, std::string const &modes);
		static int	i(Server &server, bool add, int argCount);
		static int	t(Server &server, bool add, int argCount);
		static int	l(Server &server, bool add, int argCount);
		static int	k(Server &server, bool add, int argCount);
		static int	o(Server &server, bool add, int argCount);
};
