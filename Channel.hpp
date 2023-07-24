#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "User.hpp"

class Channel
{
	public:
		Channel();
		Channel(const Channel& p);
		~Channel();
		Channel& operator=(const Channel& p);

	private:
		std::deque<User> user;
		std::deque<std::string> message;
};

#endif