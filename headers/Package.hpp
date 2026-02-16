/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:50 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 04:53:38 by hkeromne         ###   ########.fr       */
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
	std::string					errNick;
	std::string					errChanName;
	std::string					rpl_data;
	Client						oldClient;
	Client						*client;
	Channel						*channel;
	bool						quit;
	int							error;

	void						clear(void);
	void						setError(int error);
}	t_package;

extern t_package package;
