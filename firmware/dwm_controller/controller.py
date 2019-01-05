import serial
import keyboard
import sys, termios

# Variables to be used
devicePort = serial.Serial('/dev/ttyACM1', 115200, timeout=1)
needMenu = True
selection = 0

# Constants
MAX_SELECTIONS = 7
SELECTION_OUTPUT = 1
SELECTION_START = 2
SELECTION_STOP = 3
SELECTION_TAG = 4
SELECTION_ANCHOR = 5
SELECTION_ADDRESS = 6
SELECTION_SWITCH = 7

# Data bytes to be sent over the port
startData = b"b;"
stopData = b"s;"
tagData = b"t03;"
anchorData = b"a13;"
addressData = b"d0;"
switchData = b"w00t;"

def readDistance():
    line = devicePort.readline()
    line = line.rstrip(b'\r\n')
    line = line.decode()
    if line.endswith(';'):
        values = line.split(',')

        for idx, val in enumerate(values):
    	    print("ID:", idx + 1, " ",  val)
        print("")
    
    pass


def printMenu():
    print("")
    print("%d. Display PORT output" % SELECTION_OUTPUT)
    print("%d. Send START command" % SELECTION_START)
    print("%d. Send STOP command" % SELECTION_STOP)
    print("%d. Send TAG command" % SELECTION_TAG)
    print("%d. Send ANCHOR command" % SELECTION_ANCHOR)
    print("%d. Send ADDRESS command" % SELECTION_ADDRESS)
    print("%d. Send SWITCH command" % SELECTION_SWITCH)
    pass


def showMenu() -> int:
    printMenu()
    termios.tcflush(sys.stdin, termios.TCIOFLUSH)
    selection = int(input("Select no: "))
    print("")
    return selection


def sendStart():
    devicePort.write(startData)
    print("Started device.\n")
    pass

def sendStop():
    devicePort.write(stopData)
    print("Stopped device.\n")
    pass

def sendTag():
    devicePort.write(tagData)
    print("Switched device to Tag.\n")
    pass

def sendAnchor():
    devicePort.write(anchorData)
    print("Switched device to Anchor.\n")
    pass


def sendAddress():
    devicePort.write(addressData)
    print("Switched device to ID:\n")
    pass


def sendSwitch():
    devicePort.write(switchData)
    print("Executed switches:\n")
    pass


def parseSelection():
    if selection == SELECTION_OUTPUT:
        global needMenu
        needMenu = False
    elif selection == SELECTION_START:
        sendStart()
    elif selection == SELECTION_STOP:
        sendStop()
    elif selection == SELECTION_TAG:
        sendTag()
    elif selection == SELECTION_ANCHOR:
        sendAnchor()
    elif selection == SELECTION_ADDRESS:
        sendAddress()
    elif selection == SELECTION_SWITCH:
        sendSwitch()
    else:
        print("Invalid selection.\n")


def switchOnMenu(e):
    # scan_code of '50' is 'm'
    if e.scan_code == 50:
        global needMenu
        needMenu = True
    pass


# Setup the keyboard interrupt
keyboard.hook(switchOnMenu)

while (True):
    if needMenu:
        selection = showMenu()
        if selection > MAX_SELECTIONS or selection < 0:
            print("Invalid selection.\n")
            pass

        parseSelection()
    else:
        readDistance()

	
devicePort.close()
