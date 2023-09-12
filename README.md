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

# file transfer
dcc send nick_receiver file_path
dcc get nick_sender
Files are downloaded on the root directory (~)