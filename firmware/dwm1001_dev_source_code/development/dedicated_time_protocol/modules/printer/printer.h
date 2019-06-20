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
  uint16 txInterval,
  uint16 rxToTxBuffer,
  uint16 cyclePeriod,
  uint16 activePeriod,
  uint16 sleepPeriod,
  double wakeInitFact
  );
