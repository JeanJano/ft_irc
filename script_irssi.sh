#!/bin/bash

# Ensure there are exactly 3 parameters
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <host> <port> <password>"
    exit 1
fi

HOST=$1
PORT=$2
PASSWORD=$3

# Launch the first client with nickname "ping"
gnome-terminal -- bash -c "irssi -c $HOST -p $PORT -w $PASSWORD -n ping"

# Give a little time between launches
sleep 1

# Launch the second client with nickname "pong"
gnome-terminal -- bash -c "irssi -c $HOST -p $PORT -w $PASSWORD -n pong"

exit 0
