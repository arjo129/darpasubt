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
    case START_CHAR:
      result.key = START_KEY;
      result.thisId = 0; // Irrelevant so we clear
      result.anchorsTotalCount = 0; // Irrelevant so we clear
      memset(&result.nodeSwitches, 0, sizeof result.nodeSwitches); // Irrelevant so we clear
      break;
    case SWITCH_CHAR:
      result.key = SWITCH_KEY;
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

static void getSwitches(struct NodeSwitch *switches, char *paramString) {
  int i, j;
  int numberOfSwitches = paramString[0]; // Number of switches is at the first position

  j = 1;
  for (i = 0; i < numberOfSwitches; i++) {
    switches->currentId = paramString[j++] - '0';
    switches->newId = paramString[j++] - '0';
    switches->newRole = paramString[j++];
  }
}