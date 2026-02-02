/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbonneau <sbonneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 04:50:20 by sbonneau          #+#    #+#             */
/*   Updated: 2026/02/02 05:10:09 by sbonneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <csignal>
#include <cstdlib>

bool g_running = true;

void signalHandler(int signum)
{
	(void)signum;
	std::cout << "\nSignal received, shutting down server..." << std::endl;
	g_running = false;
}

bool isValidPort(const std::string& portStr)
{
	if (portStr.empty())
		return false;
	
	for (size_t i = 0; i < portStr.length(); i++)
	{
		if (!isdigit(portStr[i]))
			return false;
	}
	
	int port = atoi(portStr.c_str());
	return (port > 0 && port <= 65535);
}

void printUsage(const std::string& programName)
{
	std::cerr << "Usage: " << programName << " <port> <password>" << std::endl;
	std::cerr << "  port     : Port number (1-65535)" << std::endl;
	std::cerr << "  password : Server password" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printUsage(argv[0]);
		return 1;
	}

	std::string portStr = argv[1];
	std::string password = argv[2];

	if (!isValidPort(portStr))
	{
		std::cerr << "Error: Invalid port number" << std::endl;
		printUsage(argv[0]);
		return 1;
	}

	if (password.empty())
	{
		std::cerr << "Error: Password cannot be empty" << std::endl;
		printUsage(argv[0]);
		return 1;
	}

	int port = atoi(portStr.c_str());

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);

	try
	{
		Server server(port, password);
		server.run();
		std::cout << "Server stopped successfully" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}