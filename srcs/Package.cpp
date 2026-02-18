/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Package.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dedavid <dedavid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:28:40 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/17 23:09:23 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Package.hpp"
# include <strings.h>

t_package package;

void	t_package::clear(void)
{
	cmd.clear();
	cmdData.clear();
	quitMessage.clear();
	errNick.clear();
	errChanName.clear();
	errMode.clear();
	client = NULL;
	channel = NULL;
	quit = 0;
	error = 0;

}

void	t_package::errClear(void)
{
	errNick.clear();
	errChanName.clear();
	errMode.clear();
}

void	t_package::setError(int error) { this->error = error; };
