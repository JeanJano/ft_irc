#include "Fizzbuzz.hpp"

bool	isNum(char c) {
	if ((c >= '0' && c <= '9') || c == '-')
		return (true);
	return (false);
}

Fizzbuzz::Fizzbuzz(std::string input) {
	parsing(input);
}

Fizzbuzz::Fizzbuzz(const Fizzbuzz& p) {
	this->num = p.num;
}

Fizzbuzz::~Fizzbuzz() {}

void	Fizzbuzz::parsing(std::string input) {
	input = input.substr(input.find(':') + 1);
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

void	Fizzbuzz::sendF(int sd) {
	std::stringstream ss;
	std::string s;
	for (auto it = num.begin(); it != num.end(); ++it) {
		ss << *it;
		if (*it % 5 == 0 && *it % 7 == 0)
			s = "FIZZ BUZZ";
		else if (*it % 5 == 0)
			s = "FIZZ";
		else if (*it % 7 == 0)
			s = "BUZZ";
		else
			ss >> s;
		s = "PRIVMSG #bot :" + s + "\r\n";
		send(sd, s.c_str(), s.size(), 0);
	}
}
