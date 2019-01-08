/*!
* @brief Component name:	command
*
* Defines public interface to command component.
*
* @file command.h
*/

#include "deca_types.h"

#define MSG_END_CHAR ';'

#define TAG_CHAR 't'
#define ANCHOR_CHAR 'a'
#define STOP_CHAR 's'
#define START_CHAR 'b'
#define SWITCH_CHAR 'w'
#define ADDRESS_CHAR 'd'

#define KEY_INDEX 0
#define ID_INDEX 1
#define ANCHORS_COUNT_INDEX 2
#define NODE_SWITCH_INDEX 1
#define ADDRESS_INDEX 1
#define SWITCH_DATA_INDEX 2

#define MAX_NODE_SWITCHES 6
#define MAX_CMD_SERIAL_LEN 20
#define SWITCH_SET_SIZE 3

enum CommandKey {
  UNKNOWN_KEY,
  TAG_KEY,
  ANCHOR_KEY,
  START_KEY,
  STOP_KEY,
  SWITCH_KEY,
  ADDRESS_KEY
};

struct NodeSwitch {
  uint8 currentId;
  uint8 newId;
  char newRole;
};

struct Command {
  enum CommandKey key;
  uint8 thisId;
  uint8 anchorsTotalCount;
  uint8 numberOfSwitches;
  struct NodeSwitch nodeSwitches[MAX_NODE_SWITCHES];
};

struct Command constructCommand(char *inputString);
void serializeCommand(struct Command command, uint8 *serialData);
