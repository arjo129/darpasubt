#include "message_template.h"
#include "deca_types.h"

void syncCycle(void);
void rxHandler(msg_template *msg);
void configTx2(void);
void updateTx1Ts(uint64 ts);
void setRxTimeout2(void);
void resetRxTimeout(uint8 id);
