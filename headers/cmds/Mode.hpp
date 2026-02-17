/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 20:44:56 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/18 00:17:26 by hkeromne         ###   ########.fr       */
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
		static std::map<char, void (*)(Server &, bool, int &)> mode;
		static bool	Check(Server &server, std::string const &modes);
		static bool singleCheck(std::string &modes, char m, int &argCount);
		static void	i(Server &server, bool add, int &argCount);
		static void	t(Server &server, bool add, int &argCount);
		static void	l(Server &server, bool add, int &argCount);
		static void	k(Server &server, bool add, int &argCount);
		static void	o(Server &server, bool add, int &argCount);
		static void	join(void);
};
