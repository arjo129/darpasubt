#include "string.h"
#include "message_template.h"

/* Public functions */

/**
 * @brief Initialises timestamp table.
 *
 * @param table pointer to the 2D array representing the timestamp table.
 */
void initTable(uint64 table[NUM_STAMPS_PER_CYCLE][N])
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
  memcpy(msg->header, &array[0], sizeof(msg->header));
  // Copy over the id
  memcpy(&(msg->id), &array[IDX_ID], sizeof(msg->id));
  // Copy over the isFirst flag
  memcpy(&(msg->isFirst), &array[IDX_ISFIRST], sizeof(msg->isFirst));
  // Copy over the timestamps payload
  memcpy(msg->data, &array[IDX_DATA], sizeof(msg->data));
  // Copy over the CRC bytes
  memcpy(msg->crc, &array[IDX_CRC], sizeof(msg->crc));
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
  memcpy(&array[0], msg.header, sizeof(msg.header));
  // Copy over the id
  memcpy(&array[IDX_ID], &(msg.id), sizeof(msg.id));
  // Copy over the isFirst flag
  memcpy(&array[IDX_ISFIRST], &(msg.isFirst), sizeof(msg.isFirst));
  // Copy over the timestamps payload
  memcpy(&array[IDX_DATA], msg.data, sizeof(msg.data));
  // Copy over the CRC bytes
  memcpy(&array[IDX_CRC], msg.crc, sizeof(msg.crc));
}

/**
 * @brief Updates the timestamps table given the TX/RX message.
 *
 * @param table 2D array representing the timestamps table.
 * @param msg structure containing the timestamps.
 * @param ts the reception timestamp of the incoming second TX.
 * @param thisId identifier of calling node.
 */
void updateTable(uint64 table[NUM_STAMPS_PER_CYCLE][N], msg_template msg, uint64 ts, uint8 thisId)
{
  // This message is from own node.
  if (msg.id == thisId)
  {
    // Own TX timestamps are store in the first two row of its own column.
    // For perfornance and simplicity sake.
    if (msg.isFirst == 1)
    {
      table[IDX_TS_1][thisId] = ts;
    }
    else
    {
      table[IDX_TS_2][thisId] = ts;
    }
    return;
  }

  // This message is the first TX from another node.
  // We want to store the reception timestamp of this message in that node's dedicated column.
  if (msg.isFirst == 1)
  {
    /*
      Two behaviours here:

      All the columns with thisId < msg.id will DEFINITELY have TX1 and TX2
      timestamps (both of which from thisId) in IDX_TS_1 and IDX_TS_5
      (though we store at IDX_TS_1 and IDX_TS_2 instead for performance and simplicity sake)
      respectively.

      In this case, we will only need RX timestamp (stamped when receive from another
      node) at IDX_TS_4.

      All columns with thisId > msg.id will DEFINITELY have TX1 timestamp
      (from thisId) in IDX_TS_3 (though we store at IDX_TS_1 instead for performance and
      simplicity sake). And TX2 timestamp is not required for calculation.

      In this case, we will only need RX timestamp (stamped when receive from another
      node) at IDX_TS_2 and IDX_TS_6.

      Since all states are easily predetermined, we can hardcode the location to
      store the individual timestamps.

      NOTE: these will only occur for the first 3 timestamps (All the first TXes).
      For the reception of other node's timestamps from their second TX, we will have
      still determine where to place their timestamps in this node's table. Which is the
      second portion in this function scope.
    */

    if (thisId < msg.id)
    {
      table[IDX_TS_4][msg.id] = ts;
    }
    else if (thisId > msg.id)
    {
      table[IDX_TS_2][msg.id] = ts;
    }
    else
    {
      // Will not reach here.
    }
  }
  else
  {
    // This message is the second TX from another node. Handle the timestamps in the payload.

    if (thisId < msg.id)
    {
      memcpy(&table[IDX_TS_2][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP), sizeof(uint64));
      memcpy(&table[IDX_TS_3][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP) + NUM_BYTES_PER_STAMP, sizeof(uint64));
      memcpy(&table[IDX_TS_6][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP) + 2*NUM_BYTES_PER_STAMP, sizeof(uint64));
    }
    else if (thisId > msg.id)
    {
      table[IDX_TS_6][msg.id] = ts; // Store the reception timestamp
      memcpy(&table[IDX_TS_1][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP), sizeof(uint64));
      memcpy(&table[IDX_TS_4][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP) + NUM_BYTES_PER_STAMP, sizeof(uint64));
      memcpy(&table[IDX_TS_5][msg.id], msg.data + (thisId * NUM_STAMPS_PER_NODE * NUM_BYTES_PER_STAMP) + 2*NUM_BYTES_PER_STAMP, sizeof(uint64));
    }
    else
    {
      // Will not reach here.
    }
  }
}

/**
 * @brief Retrieves all the timestamp values stamped with one particular node.
 *
 * @details When @param thisId is equal to @param targetId, zeroed @param ts is returned.
 *
 * @param table 2D array representing the timestamps table.
 * @param ts array to contain the retrieved values.
 * @param thisId identifier of calling node.
 * @param targetId identifier of target node.
 */
void getFullTs(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint64 ts[NUM_STAMPS_PER_CYCLE], uint8 thisId, uint8 targetId)
{
  // Invalid call, zero all field to indicate.
  if (thisId == targetId)
  {
    memset(ts, 0, sizeof(ts));
    return;
  }

  int i;

  for (i = IDX_TS_1; i <= IDX_TS_6; i++)
  {
    if (thisId < targetId && i == IDX_TS_1)
    {
      ts[IDX_TS_1] = table[IDX_TS_1][thisId];
    }
    else if (thisId < targetId && i == IDX_TS_5)
    {
      ts[IDX_TS_5] = table[IDX_TS_2][thisId];
    }
    else if (thisId > targetId && i == IDX_TS_3)
    {
      ts[IDX_TS_3] = table[IDX_TS_1][thisId];
    }
    else
    {
      ts[i] = table[i][targetId];
    }
  }
}

/**
 * @brief Retrieves all non zero timestamp values stamped with one particular node so far.
 *
 * @param table 2D array representing the timestamps table.
 * @param ts array to contain the retrieved values.
 * @param thisId identifier of calling node.
 * @param targetId identifier of target node.
 */
void getHalfTs(uint64 table[NUM_STAMPS_PER_CYCLE][N], uint64 ts[NUM_STAMPS_PER_CYCLE/2], uint8 thisId, uint8 targetId)
{
  if (thisId < targetId)
  {
    ts[IDX_TS_1] = table[IDX_TS_1][thisId];
    ts[IDX_TS_2] = table[IDX_TS_4][targetId];
    ts[IDX_TS_3] = table[IDX_TS_2][thisId];
  }
  else if (thisId > targetId)
  {
    ts[IDX_TS_1] = table[IDX_TS_2][targetId];
    ts[IDX_TS_2] = table[IDX_TS_1][thisId];
    ts[IDX_TS_3] = table[IDX_TS_6][targetId];
  }
  else
  {
    // Invalid call, zero all field to indicate.
    memset(ts, 0, sizeof(ts));
    return;
  }
}
