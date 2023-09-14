#include "irc.hpp"

const std::string server_address = "irc.example.com";
const int server_port = 8080;
const std::string bot_nickname = "gogol";
const std::string channel = "#bot";

void	signalHandlerBot(int);

class Fizzbuzz
{
	public:
		Fizzbuzz(std::string input, int sd);
		Fizzbuzz(const Fizzbuzz& p);
		~Fizzbuzz();

		void	parsing(std::string input);
		void	display();
		void	sendF();
		void	welcome();

	private:
		std::vector<int>	num;
		int					sd;
};
