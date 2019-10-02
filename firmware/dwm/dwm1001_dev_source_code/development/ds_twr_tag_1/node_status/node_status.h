#include "deca_device_api.h"

typedef enum {
  MODE_TAG,
  MODE_ANCHOR,
  MODE_TAG_GATEWAY,
  MODE_ANCHOR_GATEWAY
} OperationMode;

typedef enum {
  STATE_STANDBY,
  STATE_RECEIVE_HOST_CMD,
  STATE_DISTRB_SYS_CMD,
  STATE_EXEC_SYS_CMD,
  STATE_RECEIVE_SYS_CMD
} DeviceState;

typedef enum {
  uint8 nodeId,
  uint8 modeId,
  OperationMode mode,
  DeviceState state
} NodeStatus;