# --------------- #
#  I M P O R T S  #
# --------------- #

from subprocess import PIPE, Popen
from threading import Thread
from queue import Queue, Empty
import sys
import time

import websocket

# ----------- #
#  L U M E N  #
# ----------- #

# Non blocking read setup.
ON_POSIX = 'posix' in sys.builtin_module_names
def enqueue_output(out, queue):
    for line in iter(out.readline, b''):
        queue.put(line)
    out.close()

# Start Lumen & thread.
process = Popen("..\\Binaries\\Executables\\x64\\Release\\Lumen.exe", stdout = PIPE, text = True, close_fds=ON_POSIX)
q = Queue()
t = Thread(target=enqueue_output, args=(process.stdout, q))
t.start()

# Find socket output from Lumen.
lumenSocketIDString = "[LUMEN] [WEBSOCKET]"
line = ""
while(True):
    try:    line = q.get()
    except  Empty: pass
    else:
        # Check if Socket ID is found.
        if(line.find(lumenSocketIDString) != -1):
            break

print(line)
# t.join()
# q.join()
# For some reason these wont close...

# ------------- #
#  S O C K E T  #
# ------------- #

# Find the port Lumen connected to in the string.
# Hard coded for now, but this will be contained in 'line'.
localhost = "127.0.0.1"
port = 8083
url = "ws://127.0.0.1:8083"

# Connect to socket.
wsConnection = websocket.create_connection(url)
wsConnection.send("Terminate")

# ------- #
#  E O F  #
# ------- #