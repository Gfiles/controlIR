import serial #pip install pyserial
from time import sleep
import json
import os
import sys
import serial.tools.list_ports

def readConfig():
    settingsFile = os.path.join(cwd, "config.json")
    if os.path.isfile(settingsFile):
        with open(settingsFile) as json_file:
            data = json.load(json_file)
    else:
        data = {
	        "uart" : "auto",
            "baudrate" : 9600,
            "numTV" : 6,
            "commands" : {
                "on" : [
				    "S02707099",
                    "N020040c4",
                    "S02707099",
                    "S02707099",
                    "S02707099",
                    "S02707099"
		        ],
                "off" : [
                    "S02707098",
                    "N020040c5",
                    "S02707098",
                    "S02707098",
                    "S02707098",
                    "S02707098"
                ]
	    	}
        }
        # Serializing json
        json_object = json.dumps(data, indent=4)
 
        # Writing to config.json
        with open(settingsFile, "w") as outfile:
            outfile.write(json_object)
    return data

# Get the current working 
# directory (CWD)
try:
    this_file = __file__
except NameError:
    this_file = sys.argv[0]
this_file = os.path.abspath(this_file)
if getattr(sys, 'frozen', False):
    cwd = os.path.dirname(sys.executable)
else:
    cwd = os.path.dirname(this_file)
    
print("Current working directory:", cwd)

# Read Config File
config = readConfig()
numTV = config["numTV"]
options = config["commands"]
baudrate = config["baudrate"]
uart = config["uart"]

if len(sys.argv) == 1:
    print(f"Possible comands are:")
    for key in options:
        print (f"  {key} [IR Emitter number]")
else:
    command = sys.argv[1]
    commands = options[command]
    
    # setup Seiral
    if uart == "auto":
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            if "USB" in p.description:
                uart = p.device
    
    ser = serial.Serial(
        # Serial Port to read the data from
        port = uart,
        #Rate at which the information is shared to the communication channel
        baudrate = baudrate,
        # Number of serial commands to accept before timing out
        timeout=1
    )
    running = True
    
    i = 0 #Counter for all commands
    while running:
        serialReady = False
        try:
            readSerial = ser.readline().strip().decode()
            print(readSerial)
            if ("Ready" in readSerial) or ("COMMAND" in readSerial):
                serialReady = True
        except:
            serialReady = False
    
        if serialReady:
            #print(commands)
            if len(sys.argv) == 3:
                pinNum = int(sys.argv[2])
                if readSerial == commands[pinNum-1]:
                    running = False
                else:
                    print(f"Sent: {commands[pinNum-1]}")
                    ser.write(bytearray(commands[pinNum-1],'ascii'))
            else:
                if readSerial == commands[i]:
                    running = False
                else:
                    if i < len(commands):
                        print(f"Sent {i+1}: {commands[i]}")
                        ser.write(bytearray(commands[i],'ascii'))
                        #print(ser.readline().strip().decode())
                        i = i + 1
                    if i == len(commands):
                        running = False
    if not running:
        ser.close()
        print("Finished")
