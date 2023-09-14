#include "irc.hpp"

const std::string server_address = "irc.example.com";
const int server_port = 8080;
const std::string bot_nickname = "FIZZBUZZ";
const std::string channel = "#bot";

class Fizzbuzz
{
	public:
		Fizzbuzz(std::string input);
		Fizzbuzz(const Fizzbuzz& p);
		~Fizzbuzz();

		void	parsing(std::string input);
		void	display();
		void	sendF(int sd);

	private:
		std::vector<int>	num;
};
