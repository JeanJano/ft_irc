#include "IRCBot.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

void SendIRCMessage(const std::string& message, int irc_socket) {
    std::string irc_message = message + "\r\n";
    send(irc_socket, irc_message.c_str(), irc_message.length(), 0);
}

std::string ReceiveIRCMessage(int irc_socket) {
    char buffer[1024];
    ssize_t bytes_received = recv(irc_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        return "";
    }
    buffer[bytes_received] = '\0';
    return std::string(buffer);
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

    reply(irc_socket, "CAP LS");
    reply(irc_socket, "PASS p");
    reply(irc_socket, "NICK " + bot_nickname);
    reply(irc_socket, "USER jsauvage jsauvage localhost :Jean SAUVAGE");
    std::string irc_message = ReceiveIRCMessage(irc_socket);
    reply(irc_socket, "JOIN #bot");

    while (true) {
        std::string irc_message = ReceiveIRCMessage(irc_socket);
    
        if (!irc_message.empty()) {
            std::cout << "message: " << irc_message << std::endl;
            // Process the IRC message and send user messages to ChatGPT
            // Send ChatGPT responses back to the IRC channel
            std::string chatgpt_response = SendToChatGPT(irc_message);
            reply(irc_socket, "PRIVMSG " + channel + " :" + chatgpt_response);
        }
    }

    return 0;
}