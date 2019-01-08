import serial
import keyboard
import sys, termios
import re

# Variables to be used
devicePort = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
needMenu = True
selection = 0

# Devic control configuration variables
givenDeviceId = '0'
givenAnchorsCount = '3'
givenSwitchList = ["12a", "23a", "01a", "30t"]

# Device control variables
commandKey = ''
newDeviceId = ''
anchorsCount = ''
switchesNum = ''
switchList = []

# Message strings
MSG_INVALID = "Invalid selection"
MSG_SELECT_NO = "Select no: "
MSG_GIVE_VAL = "Give a new value: "
MSG_GIVE_SWITCHES = "Give switches separated by ' ' space (eg. 12a 23a 31a): "
MSG_INVALID_SWITCHES = "Invalid characters"
MSG_CONFIG_TITLE = "*** CONFIG VALUES ***"
MSG_SEPARATOR = "---------------------"
MSG_RESULT = "--- RESULT ---"
MSG_SERIAL_SENT = "Serial transmitted: "

# Constants
MAX_SELECTIONS = 8
SELECTION_OUTPUT = 1
SELECTION_START = 2
SELECTION_STOP = 3
SELECTION_TAG = 4
SELECTION_ANCHOR = 5
SELECTION_ADDRESS = 6
SELECTION_SWITCH = 7
SELECTION_CONFIG = 8
CHAR_START = 'b'
CHAR_STOP = 's'
CHAR_TAG = 't'
CHAR_ANCHOR = 'a'
CHAR_SWITCH = 'w'
CHAR_ADDRESS = 'd'
CHAR_TERM = ';'
MAX_CONFIG_SELECTIONS = 4
CONFIG_ID = 1
CONFIG_ANCHORS = 2
CONFIG_SWITCHES = 3

# Data bytes to be sent over the port
startData = b"b;"
stopData = b"s;"
tagData = b"t03;"
anchorData = b"a13;"
addressData = b"d1;"
switchData = b"w412a23a01a30t;"

def serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList):
    serialOutput = ""

    serialOutput += commandKey
    if commandKey == CHAR_START or commandKey == CHAR_STOP:
        pass
    elif commandKey == CHAR_TAG or commandKey == CHAR_ANCHOR:
        serialOutput += newDeviceId
        serialOutput += anchorsCount
    elif commandKey == CHAR_ADDRESS:
        serialOutput += newDeviceId
    elif commandKey == CHAR_SWITCH:
        serialOutput += switchesNum
        for switch in switchList:
            serialOutput += switch
    else:
        pass

    serialOutput += CHAR_TERM
    print(MSG_SERIAL_SENT + "\"" + serialOutput + "\"")
    return serialOutput


def clearControlVars():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = ''
    newDeviceId = ''
    anchorsCount = ''
    switchesNum =''
    switchList = []


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


def printConfigStatus():
    print(MSG_CONFIG_TITLE)
    print("* ID            : " + givenDeviceId)
    print("* Anchors Count : " + givenAnchorsCount)
    print("* Switches      : " + str(givenSwitchList))


def printMenu():
    print("")
    printConfigStatus()
    print("")
    print("%d. Display PORT output" % SELECTION_OUTPUT)
    print("%d. Send START command" % SELECTION_START)
    print("%d. Send STOP command" % SELECTION_STOP)
    print("%d. Send TAG command" % SELECTION_TAG)
    print("%d. Send ANCHOR command" % SELECTION_ANCHOR)
    print("%d. Send ADDRESS command" % SELECTION_ADDRESS)
    print("%d. Send SWITCH command" % SELECTION_SWITCH)
    print("%d. Configure switches" % SELECTION_CONFIG)
    pass


def printConfigMenu():
    print("")
    print("Choose to configure: ")
    print("%d. Device ID" % CONFIG_ID)
    print("%d. Anchors Count" % CONFIG_ANCHORS)
    print("%d. Switches" % CONFIG_SWITCHES)


def showMenu():
    printMenu()
    termios.tcflush(sys.stdin, termios.TCIOFLUSH)
    print(MSG_SEPARATOR)
    selection = int(input(MSG_SELECT_NO))
    print("")
    return selection


def sendStart():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_START
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Started device.")
    clearControlVars()
    pass

def sendStop():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_STOP
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Stopped device.")
    clearControlVars()
    pass

def sendTag():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_TAG
    newDeviceId = givenDeviceId
    anchorsCount = givenAnchorsCount
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Switched device to Tag.")
    clearControlVars()
    pass

def sendAnchor():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_ANCHOR
    newDeviceId = givenDeviceId
    anchorsCount = givenAnchorsCount
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Switched device to Anchor.")
    clearControlVars()
    pass


def sendAddress():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_ADDRESS
    newDeviceId = givenDeviceId
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Switched device to ID: %d" % int(newDeviceId))
    clearControlVars()
    pass


def sendSwitch():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_SWITCH
    switchList = givenSwitchList
    switchesNum = str(len(switchList))
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Executed switches: " + str(switchList))
    clearControlVars()
    pass


def configSwitches(raw):
    global givenSwitchList

    doesMatch = re.match(r'\d\d[a - z](\s\d\d[a - z])*', raw, 0)
    if not doesMatch:
        print(MSG_INVALID_SWITCHES)
        return

    givenSwitchList = re.split('\s', raw) # Split any whitespace
    givenSwitchList = list(filter(None, givenSwitchList)) # Remove all the empty elements


def configure():
    global givenDeviceId
    global givenAnchorsCount
    global givenSwitchList

    printConfigMenu()
    termios.tcflush(sys.stdin, termios.TCIOFLUSH)
    print(MSG_SEPARATOR)
    selection = int(input(MSG_SELECT_NO))
    print("")

    if selection == CONFIG_ID:
        value = input(MSG_GIVE_VAL)
        print("")
        givenDeviceId = value
    elif selection == CONFIG_ANCHORS:
        value = input(MSG_GIVE_VAL)
        print("")
        givenAnchorsCount = value
    elif selection == CONFIG_SWITCHES:
        raw = input(MSG_GIVE_SWITCHES)
        configSwitches(raw)
    else:
        print(MSG_INVALID)


def parseSelection():
    if selection == SELECTION_OUTPUT:
        global needMenu
        needMenu = False
    elif selection == SELECTION_START:
        print(MSG_RESULT)
        sendStart()
    elif selection == SELECTION_STOP:
        print(MSG_RESULT)
        sendStop()
    elif selection == SELECTION_TAG:
        print(MSG_RESULT)
        sendTag()
    elif selection == SELECTION_ANCHOR:
        print(MSG_RESULT)
        sendAnchor()
    elif selection == SELECTION_ADDRESS:
        print(MSG_RESULT)
        sendAddress()
    elif selection == SELECTION_SWITCH:
        print(MSG_RESULT)
        sendSwitch()
    elif selection == SELECTION_CONFIG:
        configure()
    else:
        print(MSG_INVALID)


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

        parseSelection()
    else:
        readDistance()

	
devicePort.close()
