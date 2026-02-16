/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:40 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/16 11:15:46 by dedavid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Package.hpp"
# include <strings.h>

t_package package;

void	t_package::clear(void)
{
	cmd.clear();
	cmdData.clear();
	rplData.clear();
	client = NULL;
	channel = NULL;
	quit = 0;
	error = 0;

}


void	t_package::setError(int error) { this->error = error; };
