#include "deca_types.h"

#ifndef MESSAGE_TEMPLATE_H
#define MESSAGE_TEMPLATE_H
#include "message_template.h"
#endif

#ifndef MESSAGE_TRANSCEIVER_H
#define MESSAGE_TRANSCEIVER_H
#include "message_transceiver.h"
#endif

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
