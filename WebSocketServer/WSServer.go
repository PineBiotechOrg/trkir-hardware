package main

import (
    "bufio"
    "fmt"
    "log"
    "net"
    "net/http"

    "github.com/gorilla/websocket"
)


func main() {
    upgrader := websocket.Upgrader{}

    clientSocketConn, err := net.Dial(tcp, socketUrl)
    if err != nil {
        log.Println(err)
        return
    }
    reader := bufio.NewReader(clientSocketConn)

    fmt.Printf("Waiting connection...\n")

    http.HandleFunc(wsStreamingPath, func(w http.ResponseWriter, r *http.Request) {
        // TODO: check and log error https://tracker.yandex.ru/VPAGROUPDEV-907
        conn, err := upgrader.Upgrade(w, r, nil)
        if err != nil {
            log.Println(err)
            return
        }
        fmt.Printf("Someone connected\n")

        frameBuffer := make([]byte, 0)

        for {
            readByte, err := reader.ReadByte()
            if err != nil {
                log.Println(err)
                return
            }
            frameBuffer = append(frameBuffer, readByte)

            if len(frameBuffer) >= len(imageEndPrefix) && (string(frameBuffer[len(frameBuffer) - len(imageEndPrefix):]) == imageEndPrefix) {
                if err := conn.WriteMessage(websocket.TextMessage, frameBuffer); err != nil {
                  return
                }
                frameBuffer = make([]byte, 0)
            }
        }
    })

    _ = http.ListenAndServe(fmt.Sprintf("%s:%s", hostsListen, wsPort), nil)
}
