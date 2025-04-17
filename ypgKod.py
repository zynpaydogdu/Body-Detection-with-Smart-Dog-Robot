import cv2
import numpy as np
from ultralytics import YOLO
import socket  # Wi-Fi üzerinden bağlantı için

# ESP8266'nın IP adresi ve portu
ESP8266_IP = "192.168.4.1"  # NodeMCU'nun seri portta gösterdiği IP adresi
ESP8266_PORT = 5000

# TCP bağlantısı başlat
try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((ESP8266_IP, ESP8266_PORT))
    print("ESP8266'ya bağlanıldı!")
except Exception as e:
    print("Bağlantı hatası:", e)
    client_socket = None

model = YOLO("yolov8n.pt")

cap = cv2.VideoCapture(0)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)

    results = model(frame)

    bedenvarmi = False

    for result in results:
        for box in result.boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            label = model.names[int(box.cls[0])]

            if label == "person":
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, label, (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                if client_socket:
                    try:
                        client_socket.sendall(b'c\n')  # ileri
                        cv2.waitKey(500)
                        client_socket.sendall(b'j\n')  # şınav
                    except Exception as e:
                        print("Gönderim hatası:", e)

                bedenvarmi = True

    if not bedenvarmi and client_socket:
        try:
            client_socket.sendall(b'h\n')  # idle
        except Exception as e:
            print("Gönderim hatası:", e)

    cv2.imshow("Enkaz Altında Beden Tespiti", frame)

    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()

# Bağlantıyı kapat
if client_socket:
    client_socket.close()
