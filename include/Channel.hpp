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
		void	removeUser(User &user);
		void	setTopic(std::string newTopic);
		void	setTimeStamp(std::time_t newTimeStamp);

		std::string						&getName();
		std::string						&getTopic();
		std::time_t						&getTimeStamp();
		std::string						&getPass();
		std::vector<User>				&getMembers();
		std::map<std::string, Role*>	&getMode();


	private:
		std::string						name;
		std::string						pass;
		std::string						topic;
		std::time_t						timestamp;
		std::vector<User>				members;
		std::vector<User>				banList;
		std::map<std::string, Role*>	mode;
};

#endif