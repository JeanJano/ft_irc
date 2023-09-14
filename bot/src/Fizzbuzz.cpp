#include "Fizzbuzz.hpp"

bool	isNum(char c) {
	if ((c >= '0' && c <= '9') || c == '-')
		return (true);
	return (false);
}

Fizzbuzz::Fizzbuzz(std::string input, int sd) : sd(sd) {
	parsing(input);
}

Fizzbuzz::Fizzbuzz(const Fizzbuzz& p) {
	this->num = p.num;
	this->sd = p.sd;
}

Fizzbuzz::~Fizzbuzz() {}

void	Fizzbuzz::parsing(std::string input) {
	input = input.substr(input.find(':') + 1);
	if (input.find("JOIN") != std::string::npos) {
		welcome();
		return ;
	}
	input = input.substr(input.find(':') + 1);
	if (isNum(input[0]) == false)
		return ;

	std::string	sub;
	int j;

	for (int i = 0; i < input.size(); i++) {
		if (isNum(input[i]) == true) {
			for (j = i; j < input.size() && isNum(input[j]) == true; j++) ;
			sub = input.substr(i, j);
			num.push_back(atoi(sub.c_str()));
			i = j;
		}
	}
}

void	Fizzbuzz::display() {
	for (auto it = num.begin(); it != num.end(); ++it) {
		std::cout << *it << " " << std::endl;
	}
}

void	Fizzbuzz::sendF() {
	for (auto it = num.begin(); it != num.end(); ++it) {
		std::stringstream ss;
		std::string msg = "";
		ss << *it;
		if (*it % 5 == 0 && *it % 7 == 0)
			msg = "FIZZ BUZZ";
		else if (*it % 5 == 0)
			msg = "FIZZ";
		else if (*it % 7 == 0)
			msg = "BUZZ";
		else
			ss >> msg;
		msg = "PRIVMSG #bot :" + msg + "\r\n";
		send(sd, msg.c_str(), msg.size(), 0);
	}
}

void	Fizzbuzz::welcome() {
	std::string	msg;

	msg = "PRIVMSG #bot :Welcome\r\n";
	send(sd, msg.c_str(), msg.size(), 0);
	msg = "PRIVMSG #bot :To communicate with me just enter a list of number\r\n";
	send(sd, msg.c_str(), msg.size(), 0);
	msg = "PRIVMSG #bot :Enjoy the experience\r\n";
	send(sd, msg.c_str(), msg.size(), 0);
	return ;
}
