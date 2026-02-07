/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:50 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 21:31:03 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <vector>
# include "Client.hpp"

typedef struct	s_package
{
	std::string					cmd;
	std::vector<std::string>	data;
	Client						oldClient;
	Client						*client;
	std::string					quit_message;
	bool						quit;
	int							error;
}	t_package;

extern t_package package;
