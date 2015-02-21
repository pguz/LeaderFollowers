#!/bin/bash
REQ_PROC=3
REQ_NUM=10000
HOST_ADDRESS=127.0.0.1
PORT=5555

trap ctrl_c INT

function ctrl_c() {
    echo "** Trapped CTRL-C"
    for i in "${pidArr[@]}" ; do
        echo "Killing PID: $i"
        kill $i # nie sprawdza czy istnieje
    done
    exit 0
}

pidArr=()
for ((i = 1; i <= $REQ_PROC; i++))
do
    ./example/ZmqClient/client.py $i $HOST_ADDRESS $PORT $REQ_NUM &
    pidArr+=($!)
done
wait
