/*!
* @brief Component name:	command
*
* Defines public interface to command component.
*
* @file command.h
*/

#define DELIMITER ';'

enum CommandKey {
    TAG_KEY,
    ANCHOR_KEY,
    START_KEY,
    STOP_KEY,
    UNKNOWN_KEY
};

struct Command {
    enum CommandKey key;
    char *param;
};

struct Command constructCommand(char *inputString);