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
 * @brief Initialise indexes to data.
 * These indexes point to message data to be copied to table.
 *
 */
void initDataIndexes(uint8 dataIndexes[N])
{
  int i;
  for (i = 0; i < N-1; i++)
  {
    dataIndexes[i] = 2*N-2;
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
 * Only updates when receiving a message.
 *
 * @param table 2D array representing the timestamps table.
 * @param msg structure containing the timestamps.
 */
void updateTableRx(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg)
{
  if (msg.isFirst == 1) {
    setRxTimestamp(table[tableIndexes[msg.id]][msg.id]);
  } else {
    table[tableIndexes[msg.id]][msg.id] = msg.data[dataIndexes[msg.id]];
    dataIndexes[msg.id]++;
  }

  // Always increment table index after receive
  tableIndexes[msg.id]++;
}

/**
 * @brief Updates the timestamps table given the structure containing the timestamps.
 * Only updates when transmitting a message.
 *
 * @param table 2D array representing the timestamps table.
 * @param msg structure containing the timestamps.
 * @param ts the reception timestamp of the incoming second TX.
 */
<<<<<<< HEAD
void updateTableTx(uint32 table[NUM_STAMPS_PER_CYCLE][N])
||||||| merged common ancestors
void updateTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg)
=======
void updateTable(uint32 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg, uint32 ts)
>>>>>>> 11c54a935c1e152db9162fe8cbef93793482e175
{
<<<<<<< HEAD
  if (msg.isFirst == 1) {
    setTxTimestamp(table[tableIndexes[NODE_ID]][NODE_ID]);
  } else {
    setTxTimestampDelayed(table[tableIndexes[NODE_ID]][NODE_ID], 0); // TODO adjust delay
  }

  // Always increment table index after transmit
  tableIndexes[NODE_ID] = tableIndexes[NODE_ID] + 3;
||||||| merged common ancestors
  // TODO: Logic to determine where in the table to copy the timestamps to
  //       given the ID of the msg.
=======
  // TODO: Logic to determine where in the table to copy the timestamps to
  //       given the ID of the msg.
  //       Note: @param ts refers to the timestamp when receiving the second TX.
  //             For some nodes, the first 3 timestamps will only be complete after
  //             receiving the second TX. Eg: Node(U2) See the TX dots diagram.
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
  //       2 separate behaviours depnding on the this node's and incoming node's id.
>>>>>>> 11c54a935c1e152db9162fe8cbef93793482e175
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
