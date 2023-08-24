#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class User;
class Role;

class Channel
{
	public:
		Channel();
		Channel(std::string n, std::string p);
		Channel(const Channel &cpy);
		~Channel();
		Channel& operator=(const Channel &cpy);

		void	addUser(User &user);
		void	removeUser(User user);
		void	addMessage(std::string msg);

		std::string						&getName();
		std::string						&getPass();
		std::vector<User>				&getMembers();
		std::map<std::string, Role*>	&getMode();

		void						setMember(User member);

	private:
		std::string						name;
		std::string						pass;
		std::string						topic;
		std::vector<User>				members;
		std::vector<User>				banList;
		std::map<std::string, Role*>	mode;
};

#endif