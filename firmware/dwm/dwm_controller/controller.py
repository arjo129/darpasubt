import serial
import keyboard
import sys, termios
import re
import time
from threading import Event

# Variables to be used
devicePort = serial.Serial('/dev/ttyACM1', 115200, timeout=0.1)
selection = 0
exit = Event()

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
SELECTION_START_DEVICE = 2
SELECTION_START_NETWORK = 3
SELECTION_STOP_DEVICE = 4
SELECTION_STOP_NETWORK = 5
SELECTION_TAG = 6
SELECTION_ANCHOR = 7
SELECTION_ADDRESS = 8
SELECTION_SWITCH = 9
SELECTION_CONFIG = 0
SELECTION_BACK = 0
CHAR_START_DEVICE = 'b'
CHAR_START_NETWORK = 'n'
CHAR_STOP_DEVICE = 's'
CHAR_STOP_NETWORK = 'm'
CHAR_TAG = 't'
CHAR_ANCHOR = 'a'
CHAR_SWITCH = 'w'
CHAR_ADDRESS = 'd'
CHAR_TERM = ';'
MAX_CONFIG_SELECTIONS = 4
CONFIG_ID = 1
CONFIG_ANCHORS = 2
CONFIG_SWITCHES = 3


def serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList):
    serialOutput = ""

    serialOutput += commandKey
    if commandKey == CHAR_START_DEVICE or commandKey == CHAR_STOP_DEVICE:
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
    line = ""
    while True:
        if exit.isSet():
            return

        byte = devicePort.read()
        ch = byte.decode()
        if (ch == '\n'):
            line = ""
            continue
        else:
            line += ch
        if (ch == CHAR_TERM):
            break

    line = line.rstrip('\r\n')
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
    print("%d. Send START DEVICE command" % SELECTION_START_DEVICE)
    print("%d. Send START NETWORK command" % SELECTION_START_NETWORK)
    print("%d. Send STOP DEVICE command" % SELECTION_STOP_DEVICE)
    print("%d. Send STOP NETWORK command" % SELECTION_STOP_NETWORK)
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
    print("%d. Back to main menu" % SELECTION_BACK)


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

    commandKey = CHAR_START_DEVICE
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Started device.")
    pass


def sendStartNetwork():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_START_NETWORK
    print("Started network.")
    exit.clear()
    while (True):
        if exit.isSet():
            return
        devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
        time.sleep(3)


def sendStop():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_STOP_DEVICE
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Stopped device.")
    pass


def sendStopNetwork():
    global commandKey
    global newDeviceId
    global anchorsCount
    global switchesNum
    global switchList

    commandKey = CHAR_STOP_NETWORK
    devicePort.write(bytearray(serialDataBuilder(commandKey, newDeviceId, anchorsCount, switchesNum, switchList), 'utf-8'))
    print("Stopped network.")


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
    elif selection == SELECTION_BACK:
        return
    else:
        print(MSG_INVALID)


def parseSelection(selection):
    print(MSG_RESULT)

    if selection == SELECTION_OUTPUT:
        exit.clear()
    elif selection == SELECTION_START_DEVICE:
        sendStart()
    elif selection == SELECTION_START_NETWORK:
        sendStartNetwork()
    elif selection == SELECTION_STOP_DEVICE:
        sendStop()
    elif selection == SELECTION_STOP_NETWORK:
        sendStopNetwork()
    elif selection == SELECTION_TAG:
        sendTag()
    elif selection == SELECTION_ANCHOR:
        sendAnchor()
    elif selection == SELECTION_ADDRESS:
        sendAddress()
    elif selection == SELECTION_SWITCH:
        sendSwitch()
    elif selection == SELECTION_CONFIG:
        configure()
    else:
        print(MSG_INVALID)

    clearControlVars()


def switchOnMenu(e):
    # scan_code of '50' is 'm'
    if e.scan_code == 50:
        exit.set()
    pass


# Setup the keyboard interrupt
keyboard.hook(switchOnMenu)
exit.set()

while (True):
    if exit.isSet():
        selection = showMenu()
        parseSelection(selection)
    else:
        readDistance()


devicePort.close()
