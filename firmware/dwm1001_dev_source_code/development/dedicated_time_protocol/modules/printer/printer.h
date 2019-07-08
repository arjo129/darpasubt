#include "deca_types.h"
#include "common.h"
#include "stdio.h"

static void printDists(double dists[N], uint8 thisId);
static void printTemp(double temp);
void printData(double dists[N], double temp, uint8 thisId);
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
  );
