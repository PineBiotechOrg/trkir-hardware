package main

import "fmt"

const (
    wsPort = "8081"
    hostsListen = "0.0.0.0"
    wsStreamingPath = "/streaming"
    socketPort = "8080"
    socketHost = "localhost"
    tcp = "tcp"
    imageEndPrefix = "\r\n"
)

var socketUrl = fmt.Sprintf("%s:%s", socketHost, socketPort)
