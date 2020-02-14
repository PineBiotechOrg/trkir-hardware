import socket
import picamera
import picamera.array
import cv2


TCP_IP = '0.0.0.0'
TCP_PORT = 8080


def start_streaming():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((TCP_IP, TCP_PORT))
    sock.listen(1)
    with picamera.PiCamera() as camera:
        camera.resolution = (160, 120)
        with picamera.array.PiRGBArray(camera) as stream:
            while True:
                print('Camera waiting for connection')
                conn, _ = sock.accept()
                print('Someone connected')
                while True:
                    try:
                        camera.capture(stream, format='bgr', use_video_port=True)
                        image = stream.array
                        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

                        ret, buffer = cv2.imencode('.jpg', gray)

                        stream.seek(0)
                        stream.truncate()

                        try:
                            conn.sendall(buffer.tobytes() + b'\r\n')
                        except Exception:
                            # client left
                            conn.close()
                            break
                        # reset the stream before the next capture
                    except KeyboardInterrupt:
                        break


if __name__ == '__main__':
    start_streaming()
