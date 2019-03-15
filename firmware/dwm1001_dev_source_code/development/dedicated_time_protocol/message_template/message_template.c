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
 * @brief Initialise indexes to table.
 * These indexes point to where in table message data should be copied to.
 *
 * There are only two types of initial values:
 *   0: for all other node ids > NODE_ID
 *   1: for all other node ids < NODE_ID
 * Value of index of NODE_ID is undefined.
 */
void initTableIndexes(uint8 tableIndexes[N])
{
  int i;
  for (i = 0; i < NODE_ID; i++)
  {
    tableIndexes[i] = 1;
  }
  for (i = NODE_ID+1; i < N-1; i++)
  {
    tableIndexes[i] = 0;
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
 * Two behaviours depending on node ids.
 *
 * @param table 2D array representing the timestamps table.
 * @param msg structure containing the timestamps.
 * @param ts the reception timestamp of the incoming second TX.
 */
void updateTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg, uint32 ts)
{
  // Only copy @param ts to table if tableIndex < 6
  if (tableIndexes[NODE_ID] < 6) {
    // Copy @param ts to table
    memcpy(table[tableIndexes[NODE_ID]][NODE_ID], ts, sizeof(uint32));

    // Change to tableIndex depends on whether
    // the node with NODE_ID is currently rx or tx
    // This is determined by comparing NODE_ID with msg.id
    // TODO find better way to change index, maybe preprocessor?
    if (NODE_ID < msg.id) {
      // NODE_ID is tx
      switch (tableIndexes[NODE_ID]) {
        case 0:
          tableIndexes[NODE_ID] = tableIndexes[NODE_ID] + 3;
          break;
        case 3:
          tableIndexes[NODE_ID]++;
          break;
        case 4:
          tableIndexes[NODE_ID] = tableIndexes[NODE_ID] + NUM_STAMPS_PER_CYCLE;
          break;
        default:
          break;
      }

    } else if (NODE_ID > msg.id) {
      // NODE_ID is rx
      switch (tableIndexes[NODE_ID]) {
        case 1:
          tableIndexes[NODE_ID]++;
          break;
        case 2:
          tableIndexes[NODE_ID] = tableIndexes[NODE_ID] + 3;
          break;
        case 5:
          tableIndexes[NODE_ID] = tableIndexes[NODE_ID] + NUM_STAMPS_PER_CYCLE;
          break;
        default:
          break;
      }

    } // else {}

  }

  // Copy msg.data to table
  // msg.data is populated only if msg.isFirst is false
  if (msg.isFirst == 0) {

    // Where in table to copy msg.data
    // TODO remove hardcoded indexes
    if (NODE_ID < msg.id) {
      memcpy(table[1][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*NODE_ID, sizeof(uint32));
      memcpy(table[2][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*(N-1), sizeof(uint32));
      memcpy(table[5][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*NODE_ID+1, sizeof(uint32));

    } else if (NODE_ID > msg.id) {
      memcpy(table[0][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*(N-1), sizeof(uint32));
      memcpy(table[3][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*NODE_ID, sizeof(uint32));
      memcpy(table[4][NODE_ID], msg.data + NUM_STAMPS_PER_NODE*(N-1)+1, sizeof(uint32));

    } // else {}
  }

}

/**
 * @brief Updates the timestamps table with a single value depending on this node's and
 *        incoming node's id.
 *
 * @param table 2D array representing the timestamps table.
 * @param ts timestamp value to update with.
 * @param thisId current node's id.
 * @param otherId incoming node's id.
 */
void updateTs(uint32 table[NUM_STAMPS_PER_CYCLE][N], uint32 ts, uint8 thisId, uint8 otherId)
{
  // TODO: Logic to determine where in the table to update a single value in the table.
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
