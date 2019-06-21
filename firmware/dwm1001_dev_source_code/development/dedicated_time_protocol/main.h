#include "deca_types.h"

#ifndef MESSAGE_TEMPLATE_H
#define MESSAGE_TEMPLATE_H
#include "message_template.h"
#endif

#ifndef MESSAGE_TRANSCEIVER_H
#define MESSAGE_TRANSCEIVER_H
#include "message_transceiver.h"
#endif

#ifndef INT_HANDLER_H
#define INT_HANDLER_H
#include "int_handler.h"
#endif

#include "UART.h"
#include "low_timer.h"
#include "timestamper.h"
#include "common.h"
#include "printer.h"

#define OTP_ANT_DLY 0x01C

void syncCycle(void);
void rxHandler(msg_template *msg);
void updateTx1Ts(uint64 ts);
void setRxTimeout2(void);
void txUpdate(uint64 ts);
