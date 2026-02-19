/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkeromne <student@42lehavre.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:26:31 by hkeromne          #+#    #+#             */
/*   Updated: 2026/02/15 17:56:08 by hkeromne         ###   ########.fr       */
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
		std::cerr << "Not enough arguments" << std::endl;
		return (false);
	}
	else if (ac > 3)
	{
		std::cerr << "Too much arguments" << std::endl;
		return (false);
	}
	if (std::string(av[PASS_ID]).empty())
	{
		std::cerr << "Empty pass unauthorized" << std::endl;
		return (false);
	}
	stream << std::string(av[PORT_ID]);
	stream >> port;
	if (!stream.eof() || stream.fail()
	|| port < 1 || port > 65535)
	{
		std::cerr << "Invalid Port" << std::endl;
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	if (!argCheck(ac, av))
		return (EXIT_FAILURE);

	std::signal(SIGINT, handleSignal);
	Server server(std::string(av[PASS_ID]), Ft::strToInt(std::string(av[PORT_ID])));
	if (!server.init())
		server.launch();
}
