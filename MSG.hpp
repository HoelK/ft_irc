/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MSG.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:29:12 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 16:29:13 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <vector>
# include <sstream>
# include "Ft.hpp"
# include "Client.hpp"
# include "Package.hpp"

class MSG
{
	public:
		static void	sendData(Client *client, std::string const &line);
};
