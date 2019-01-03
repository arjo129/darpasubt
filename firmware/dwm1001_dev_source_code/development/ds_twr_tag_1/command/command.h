/*!
* @brief Component name:	command
*
* Defines public interface to command component.
*
* @file command.h
*/

#define DELIMITER ';'

enum CommandKey {
    UNKNOWN_KEY,
    TAG_KEY,
    ANCHOR_KEY,
    START_KEY,
    STOP_KEY
};

struct Command {
    enum CommandKey key;
    char *param;
};

struct Command constructCommand(char *inputString);