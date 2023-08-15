#include "irc.hpp"

bool	checkpassword(int sd, t_server *server, User client)
{
	std::string msg;
	std::cout << "Compare = " << client.getPassWord().compare(server->password) << std::endl;
	if (client.getPassWord().compare(server->password) != 0)
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		msg = ":server 464 " + client.getNickName() + " :Password incorrect\r\n";
		send(sd, (char*)msg.c_str(), msg.size(), 0);
		return (false);
	}
	return (true);
}

void	welcomeMsg(int sd, t_server *server)
{
	std::cout << "new client connected" << std::endl;
	std::string input(getCompleteMsg(sd));
	User usr;
	usr.parseInput(input);
	usr.printInfos();
	if (checkpassword(sd, server, usr) == false)
		return ;
	server->users.push_back(usr);

	std::string msg001;
	msg001 = ":server 001 " + usr.getNickName() + " :Welcome to the Internet Relay Network, " + usr.getNickName() + "!\r\n";
	send(sd, (char*)msg001.c_str(), msg001.size(), 0);
}