/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:26:31 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/11 03:19:44 by hkeromne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void handleSignal(int) { sigShutdown = 1; };

bool	argCheck(int ac, char **av)
{
	int					port;
	std::stringstream	stream;

	if (ac < 3)
	{
		std::cout << "Not enough arguments" << std::endl;
		return (false);
	}
	else if (ac > 3)
	{
		std::cout << "Too much arguments" << std::endl;
		return (false);
	}
	stream << std::string(av[2]);
	stream >> port;
	if (stream.fail() || port < 1 || port > 65535)
	{
		std::cout << "Invalid Port" << std::endl;
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	if (!argCheck(ac, av))
		return (EXIT_FAILURE);

	std::signal(SIGINT, handleSignal);
	Server server(std::string(av[1]), std::atoi(av[2]));
	server.init();
	server.launch();
}
