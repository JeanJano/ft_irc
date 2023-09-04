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

		// setters

		void	addUser(User user);
		void	removeUser(User &user);
		void	setTopic(std::string newTopic);
		void	setTimeStamp(std::time_t newTimeStamp);

		// getters

		std::string						&getName();
		std::string						&getTopic();
		std::time_t						&getTimeStamp();
		std::string						&getPass();
		std::vector<User>				&getMembers();
		User							*getTopicSetter();
		void							setTopicSetter(User topicSet);
		std::map<std::string, Role*>	&getRole();
		std::map<char, bool>			&getMode();
		int								&getLimit();

		// mode
		void	changeStatus(char mod, char operand);
		void	modeManager(std::string, User&);
		void	inviteOnly(char, char, std::string, std::string, User&);
		void	channelKey(char, char, std::string, std::string, User&);
		void	userLimit(char, char, std::string, std::string, User&);
		void	operatorPriv(char, char, std::string, std::string, User&);
		void	topicRestr(char, char, std::string, std::string, User&);

		bool	isOperator(Role* role);
		void	changeRole(User& user, char operand, std::string channelName);

	private:
		std::string						name;
		std::string						pass;
		std::string						topic;
		User							*topicSetter;
		std::time_t						timestamp;
		std::vector<User>				members;
		std::vector<User>				banList;
		std::map<std::string, Role*>	role;
		std::map<char, bool>			mode;
		int								limit;
};

#endif