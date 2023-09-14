#include "Fizzbuzz.hpp"

bool	run = true;

void	signalHandlerBot(int num) {
	(void)num;
	run = false;
}

std::string ReceiveIRCMessage(int irc_socket, bool isWelcome) {
    char buffer[1024];

    struct timeval tv;
    if (isWelcome == true)
        tv.tv_sec = 5;
    else
        tv.tv_sec = 1;

	tv.tv_usec = 0;
	setsockopt(irc_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
    ssize_t bytes_received = recv(irc_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        return "";
    }
    buffer[bytes_received] = '\0';
    return (std::string(buffer));
}

void	reply(int sd, std::string msg) {
	msg += "\r\n";
	int val = send(sd, msg.c_str(), msg.size(), 0);
	if (val < 0)
		std::cout << "Message not complete" << std::endl;
	std::cout << "Sent: " << msg;
}

int main() {
    int irc_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    connect(irc_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char            host[256];
    std::string     ip;
    struct hostent  *host_entry;
    int             hostname;

    hostname = gethostname(host, sizeof(host));
    host_entry = gethostbyname(host);
    ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    reply(irc_socket, "CAP LS");
    reply(irc_socket, "PASS p");
    reply(irc_socket, "NICK " + bot_nickname);
    reply(irc_socket, "USER jsauvage jsauvage " + ip + " :Jean SAUVAGE");
    std::string irc_message = ReceiveIRCMessage(irc_socket, true);
    reply(irc_socket, "JOIN " + channel);

    signal(SIGINT, signalHandlerBot);
    while (run) {
        std::string irc_message = ReceiveIRCMessage(irc_socket, false);
    
        if (!irc_message.empty()) {
            std::cout << "message: " << irc_message << std::endl;
            Fizzbuzz fizzbuzz(irc_message, irc_socket);
            fizzbuzz.display();
            fizzbuzz.sendF();
        }
    }

    return 0;
}