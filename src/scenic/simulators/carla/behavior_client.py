import socket
import threading

ADDRESS= "127.0.0.1"
PORT = 5051
CONNECT = (ADDRESS,PORT)

_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
_socket.connect(CONNECT)

def send_openpilot_init( default_speed, steer_ratio, vehicle_id ):
    global _socket
    _socket.sendall(bytes("openpilot"+"-"*23,'ascii'))
    _socket.sendall(bytes(str(default_speed),'ascii'))
    _socket.sendall(bytes(str(steer_ratio),'ascii'))
    _socket.sendall(bytes(str(vehicle_id),'ascii'))
    msg = _socket.recv(32)
