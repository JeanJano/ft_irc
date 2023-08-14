# ft_irc

# compilation
server: g++ server.cpp -o server
client: g++ client.cpp -o client

./server 8080 passw
./client 8080 passw

# irssi
/connect localhost 8080 passw

# command useful
create log file: /rawlog open logs