/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:20 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 21:38:13 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "Package.hpp"

# define CMD_PASS			"PASS"
# define CMD_NICK			"NICK"
# define CMD_USER			"USER"
# define CMD_QUIT			"QUIT"
# define NICK_NICK			0
# define PASS_PASS			0
# define USER_USERNAME		0
# define USER_REALNAME		3
# define QUIT_REASON		0

class CMD
{
	public:
		static void	apply(void);
		static void	Nick(void);
		static void	User(void);
		static void	Pass(void);
		static void	Quit(void);
};
