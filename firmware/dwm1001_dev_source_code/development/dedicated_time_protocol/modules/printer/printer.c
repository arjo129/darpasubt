#include "printer.h"

/**
 * @brief Prints all distances to other nodes as serial output.
 * 
 * @param dists array of distances to be printed.
 * @param thisId identifier of the calling node.
 */
static void printDists(double dists[N], uint8 thisId)
{
  printf("D: ");
  for (int i = 0; i < N; i++)
  {
    if (i == thisId)
    {
      continue;
    }

    printf("%0.4lf", dists[i]);
    if ((i + 1 != N - 1 || thisId != N - 1) && i != N - 1)
    {
      printf(",");
    }
  }
}

/**
 * @brief Reads and prints the temperature of the intergrated chip (DWM1000). 
 * 
 * @param temp temperature value to print.
 */
static void printTemp(double temp)
{
  printf("T: %lf", temp);
}

/**
 * @brief Prints the required output to terminal.
 * 
 * @param dists array of distances to print.
 * @param temp temperature value to print.
 * @param thisId identifier of the calling node.
 */
void printData(double dists[N], double temp, uint8 thisId)
{
  if (P_DIST == 1)
  {
    printDists(dists, thisId);
  }

  if (P_DIST == 1 && P_TEMP == 1)
  {
    printf(" ");
  }

  if (P_TEMP == 1)
  {
    printTemp(temp);
  }

  printf(";\r\n");
}

/**
 * @brief Prints all configuration information.
 * 
 * @param nodeCount number of nodes in network.
 * @param thisId identifier of the calling node.
 * @param rngFreq range frequency.
 * @param txInterval time period of TX interval.
 * @param rxToTxBuffer RX to TX turnover period.
 * @param cyclePeriod time period of each cycle.
 * @param activePeriod time period of the active duration of each cycle.
 * @param sleepPeriod time period of the sleep duration of each cycle.
 * @param wakeBuffer time difference factor between hardware waking and beginning of cycle.
 */
void printConfig(
  uint8 nodeCount, 
  uint8 thisId, 
  uint16 rngFreq, 
  uint32 txInterval,
  uint32 rxToTxBuffer,
  uint32 cyclePeriod,
  uint32 activePeriod,
  uint32 sleepPeriod,
  uint32 wakeBuffer
  )
{
  printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\r\n");
  printf(" NODES_COUNT : %d\r\n", nodeCount);
  printf("     NODE_ID : %d\r\n", thisId);
  printf("    RNG_FREQ : %d (Hz)\r\n", rngFreq);
  printf("TRX_INTERVAL : %d (us)\r\n", txInterval);
  printf("RX_TO_TX_BUF : %d (us)\r\n", rxToTxBuffer);
  printf("CYCLE_PERIOD : %u (us)\r\n", cyclePeriod);
  printf("ACTIV_PERIOD : %u (us)\r\n", activePeriod);
  printf("SLEEP_PERIOD : %u (us)\r\n", sleepPeriod);
  printf(" WAKE_BUFFER : %u (us)\r\n", wakeBuffer);
  printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\r\n\r\n");
}