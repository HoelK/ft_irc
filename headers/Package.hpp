/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:50 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 22:57:35 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <vector>
# include "Client.hpp"

class Channel;

typedef struct	s_package
{
	std::string					cmd;
	std::vector<std::string>	cmdData;
	std::string					quitMessage;
	std::string					errNick;
	std::string					errChanName;
	std::string					errMode;
	Client						oldClient;
	Client						*client;
	Channel						*channel;
	bool						quit;
	int							error;

	void						clear(void);
	void						errClear(void);
	void						setError(int error);
}	t_package;

extern t_package package;
