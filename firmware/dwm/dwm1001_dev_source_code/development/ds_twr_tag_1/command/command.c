/*!
* @brief Component name:	command
*
* Helper component that converts a string received from Serial communications 
* into a Command structure that is then passed to the main application to be
* executed as a command.
*
* @file command.c
*/

#include <string.h>
#include "command.h"

static void serializeSwitches(uint8 *field, struct NodeSwitch *switches, int numberOfSwitches);
static void getSwitches(struct NodeSwitch *switches, char *paramString);

/* Construct a Command structure based on a given string. */
struct Command constructCommand(char *inputString) {
  struct Command result;
  char inputKey;

  inputKey = inputString[KEY_INDEX];

  switch(inputKey) {
    case TAG_CHAR:
      result.key = TAG_KEY;
      result.thisId = (uint8) inputString[ID_INDEX] - '0';
      result.anchorsTotalCount = inputString[ANCHORS_COUNT_INDEX] - '0';
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case ANCHOR_CHAR:
      result.key = ANCHOR_KEY;
      result.thisId = inputString[ID_INDEX] - '0';
      result.anchorsTotalCount = inputString[ANCHORS_COUNT_INDEX] - '0';
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case STOP_CHAR:
      result.key = STOP_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case STOP_NETWORK_CHAR:
      result.key = STOP_NETWORK_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case START_CHAR:
      result.key = START_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case START_NETWORK_CHAR:
      result.key = START_NETWORK_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case SWITCH_CHAR:
      result.key = SWITCH_KEY;
      result.numberOfSwitches = inputString[NODE_SWITCH_INDEX] - '0';
      getSwitches(&result.nodeSwitches, &inputString[NODE_SWITCH_INDEX]);
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      break;
    case ADDRESS_CHAR:
      result.key = ADDRESS_KEY;
      result.thisId = inputString[ID_INDEX] - '0';
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    default:
      result.key = UNKNOWN_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
  }
  return result;
}

void serializeCommand(struct Command command, uint8 *serialData) {
  memset(serialData, 0, MAX_CMD_SERIAL_LEN); // Clear to null first

  switch(command.key) {
    case TAG_KEY:
      serialData[KEY_INDEX] = TAG_CHAR;
      serialData[ID_INDEX] = command.thisId + '0';
      serialData[ANCHORS_COUNT_INDEX] = command.anchorsTotalCount + '0';
      break;
    case ANCHOR_KEY:
      serialData[KEY_INDEX] = ANCHOR_CHAR;
      serialData[ID_INDEX] = command.thisId + '0';
      serialData[ANCHORS_COUNT_INDEX] = command.anchorsTotalCount + '0';
      break;
    case STOP_KEY:
      serialData[KEY_INDEX] = STOP_CHAR;
      break;
    case STOP_NETWORK_KEY:
      serialData[KEY_INDEX] = STOP_NETWORK_CHAR;
      break;
    case START_KEY:
      serialData[KEY_INDEX] = START_CHAR;
      break;
    case START_NETWORK_KEY:
      serialData[KEY_INDEX] = START_NETWORK_CHAR;
      break;
    case SWITCH_KEY:
      serialData[KEY_INDEX] = SWITCH_CHAR;
      serialData[NODE_SWITCH_INDEX] = command.numberOfSwitches + '0';
      serializeSwitches(&serialData[SWITCH_DATA_INDEX], command.nodeSwitches, command.numberOfSwitches);
      break;
    case ADDRESS_KEY:
      serialData[KEY_INDEX] = ADDRESS_CHAR;
      serialData[ID_INDEX] = command.thisId + '0';
      break;
    default:
      break; // Unknown key so we return empty serialData
  }
}

static void serializeSwitches(uint8 *field, struct NodeSwitch *switches, int numberOfSwitches) {
  int i, j = 0;
  for (i = 0; i < numberOfSwitches; i++) {
    field[j++] = switches[i].currentId + '0';
    field[j++] = switches[i].newId + '0';
    field[j++] = switches[i].newRole;
  }
}

static void getSwitches(struct NodeSwitch *switches, char *paramString) {
  int i, j;
  int numberOfSwitches = paramString[0] - '0'; // Number of switches is at the first position
  memset(switches, 0, (sizeof *switches) * MAX_NODE_SWITCHES); // Clear to null first

  j = 1;
  for (i = 0; i < numberOfSwitches; i++) {
    switches[i].currentId = paramString[j++] - '0';
    switches[i].newId = paramString[j++] - '0';
    switches[i].newRole = paramString[j++];
  }
}