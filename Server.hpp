# pragma once
# include <map>
# include <string>

class Server
{
	private:
		int									port;
		std::string							password;
		std::map<std::string, User>	users;
		std::map<std::string, Channel>	channels;
	
	public:
		Server(std::string password, int port);
		void	addUser(std::string const &name, User &user);
		User	*getUser(std::string const &name);
		bool	deleteUser(std::string const &name);

		void	addChannel(std::string const &name, User &user);
		Channel	*getChannel(std::string const &name);
		bool	deleteChannel(std::string const &name);


		~Server(void);
};
