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
	std::vector<std::string>	cmd_data;
	std::string					rpl_data;
	Client						oldClient;
	Client						*client;
	bool						quit;
	int							error;

	void						clear();
}	t_package;

extern t_package package;
