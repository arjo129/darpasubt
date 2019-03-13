#include "string.h"
#include "message_template.h"

/**
 * @brief Initialises the timestamp tables with zeros.
 * 
 */
void initTsTable(uint32 table[NUM_STAMPS_PER_CYCLE][N])
{
  int i, j;

  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    for (j = 0; j < N; j++)
    {
      table[i][j] = 0;
    }
  }
}

/**
 * @brief Converts a given uint8 array to msg_template struct.
 * 
 * @param array the array to convert from.
 * @param msg pointer to the struct to store the conversion.
 */
void convertToStruct(uint8 *array, msg_template *msg)
{
  // Copy over the header bytes
  memcpy(&(msg->header), &array[0], sizeof(msg->header));
  // Copy over the id
  memcpy(&(msg->id), &array[IDX_ID], sizeof(msg->id));
  // Copy over the isFirst flag
  memcpy(&(msg->isFirst), &array[IDX_ISFIRST], sizeof(msg->isFirst));
  // Copy over the timestamps payload
  memcpy(msg->data, &array[IDX_DATA], sizeof(msg->id));
  // Copy over the CRC bytes
  memcpy(&(msg->id), &array[IDX_CRC], sizeof(msg->id));
}

/**
 * @brief Converts a given msg_template struct to a uin8t array.
 * 
 * @param msg the struct to convert from.
 * @param array pointer to the array to store the conversion.
 */
void convertToArr(msg_template msg, uint8 *array)
{
  // Copy over the header bytes
  memcpy(&array[0], &(msg.header), sizeof(msg.header));
  // Copy over the id
  memcpy(&array[IDX_ID], &(msg.id), sizeof(msg.id));
  // Copy over the isFirst flag
  memcpy(&array[IDX_ISFIRST], &(msg.isFirst), sizeof(msg.isFirst));
  // Copy over the timestamps payload
  memcpy(&array[IDX_DATA], msg.data, sizeof(msg.id));
  // Copy over the CRC bytes
  memcpy(&array[IDX_CRC], &(msg.id), sizeof(msg.id));
}

/**
 * @brief Updates the timestamps table given the structure containing the timestamps.
 * 
 * @param table 2D array representing the timestamps table.
 * @param msg structure containing the timestamps.
 */
void updateTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg)
{
  // TODO: Logic to determine where in the table to copy the timestamps to
  //       given the ID of the msg.
}

/**
 * @brief Retrieves all the timestamp values stamped with one particular node.
 * 
 * @param table 2D array representing the timestamps table.
 * @param ts array to contain the retrieved values.
 * @param id identifier of target node of retrieved values.
 */
void getFullTs(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint32 ts[NUM_STAMPS_PER_CYCLE], uint8 id)
{
  int i;
  
  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    ts[i] = table[i][id];
  }
}

/**
 * @brief Retrieves all non zero timestamp values stamped with one particular node so far.
 * 
 * @param table 2D array representing the timestamps table.
 * @param ts array to contain the retrieved values.
 * @param id identifier of target node of retrieved values.
 */
void getHalfTs(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint32 ts[NUM_STAMPS_PER_CYCLE/2], uint8 id)
{
  int i;
  int j = 0;

  for (i = 0; i < NUM_STAMPS_PER_CYCLE; i++)
  {
    if (table[i][id] != 0)
    {
      ts[j] = table[i][id];
      j++;
    }
  }
}