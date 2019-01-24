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

/* Construct a Command structure based on a given string. */
struct Command constructCommand(char *inputString) {
  struct Command result;
  char inputKey;
  char *inputParam;

  inputKey = inputString[0];
  inputParam = strchr(inputString, DELIMITER);
  inputParam++; // We do not want the delimiter character

  switch(inputKey) {
    case 't':
      result.key = TAG_KEY;
      break;
    case 'a':
      result.key = ANCHOR_KEY;
      break;
    case 's':
      result.key = STOP_KEY;
      break;
    case 'b':
      result.key = START_KEY;
      break;
    default:
      result.key = UNKNOWN_KEY;
  }
  result.param = inputParam;
  
  return result;
}