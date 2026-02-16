/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:50 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 11:15:43 by dedavid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <vector>
# include "Client.hpp"

typedef struct	s_package
{
	std::string					cmd;
	std::vector<std::string>	cmdData;
	std::string					errNick;
	std::string					errChanName;
	std::string					rplData;
	Client						oldClient;
	Client						*client;
	Channel						*channel;
	bool						quit;
	int							error;

	void						clear(void);
	void						setError(int error);
}	t_package;

extern t_package package;
