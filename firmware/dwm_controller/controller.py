import serial
import keyboard
import sys, termios

# Variables to be used
devicePort = serial.Serial('/dev/ttyACM1', 115200, timeout=1)
needMenu = False
selection = 0
MAX_SELECTIONS = 4

# Data bytes to be sent over the port
startData = b"b;"
stopData = b"s;"
tagData = b"t03;"
anchorData = b"a13;"

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
    print("1. Display PORT output")
    print("2. Send START command")
    print("3. Send STOP command")
    print("4. Send TAG command")
    print("5. Send ANCHOR command")
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


def parseSelection():
    if selection == 1:
        global needMenu
        needMenu = False
    elif selection == 2:
        sendStart()
    elif selection == 3:
        sendStop()
    elif selection == 4:
        sendTag()
    elif selection == 5:
        sendAnchor()
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
