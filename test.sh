#!/usr/bin/env python3

import socket
import time
import sys

PORT = int(sys.argv[1])
HOST = 'localhost'

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

def send_command(command):
    print(f">>> {command}")
    sock.send(f"{command}\r\n".encode('utf-8'))
    time.sleep(0.2)

# Send initial commands
send_command("PASS pass")
send_command("NICK nick")
send_command("USER hk hk localhost :hk")
time.sleep(1)
send_command("JOIN #a")
send_command("MODE #a +l 10")

print("\n=== Connection established and commands sent ===")
print("=== Connection will stay open. Press Ctrl+C to exit ===\n")

# Keep connection alive and show responses
try:
    sock.settimeout(1)
    while True:
        try:
            data = sock.recv(4096).decode('utf-8', errors='ignore')
            if data:
                print(data, end='')
            else:
                print("Server closed connection")
                break
        except socket.timeout:
            pass  # No data, keep waiting
except KeyboardInterrupt:
    print("\n\nClosing connection...")
finally:
    sock.close()
