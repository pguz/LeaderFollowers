#!/usr/bin/python
import argparse
import zmq
import base64

parser = argparse.ArgumentParser()
parser.add_argument("client_id", help = "client identificator", type = str)
parser.add_argument("host", help = "address ip of server", type = str)
parser.add_argument("port", help = "port number of server", type = int)
parser.add_argument("num_requests", help = "number of requestes to send", type = int)
args = parser.parse_args()

client_id = args.client_id
host = args.host
port = args.port
num_requests = args.num_requests
server_address = "tcp://" + host + ":" + str(port)
templateRequestData = "[PyZMQ " + str(client_id) + "] "

context = zmq.Context()

#  Socket to talk to server
socket = context.socket(zmq.REQ)
socket.connect(server_address)

#  Do requests, waiting each time for a response
for iter in range(num_requests):
    requestToSend = templateRequestData + str(iter)
    print(("REQ nr %s: \t" % iter)+ requestToSend)
    socket.send(str.encode(requestToSend))

    #  Get the reply.
    message = (socket.recv()).decode("utf-8")
    print("RES nr %s: \t%s" % (iter, message))
