#include "deca_types.h"
#include "message_transceiver.h"
#include "message_template.h"
#include "UART.h"
#include "int_handler.h"
#include "low_timer.h"
#include "timestamper.h"
#include "common.h"
#include "printer.h"

#define OTP_ANT_DLY 0x01C

void syncCycle(void);
void rxHandler(msg_template *msg);
void configTx2(void);
void updateTx1Ts(uint64 ts);
void setRxTimeout2(void);
