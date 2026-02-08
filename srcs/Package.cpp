/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:40 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/07 16:28:41 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Package.hpp"
# include <strings.h>

t_package package;

void	t_package::clear(void)
{
	cmd.clear();
	cmd_data.clear();
	rpl_data.clear();
	client = NULL;
	quit = 0;
	error = 0;

}
