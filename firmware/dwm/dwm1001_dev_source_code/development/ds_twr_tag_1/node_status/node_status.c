#include "deca_device_api.h"
#include "node_status.h"

NodeStatus nodeStatus(
    uint8 nodeId,
    uint8 modeId,
    OperationMode mode,
    DeviceState state) {
  NodeStatus newNode;
  newNode.nodeId = nodeId;
  newNode.modeId = modeId;
  newNode.mode = mode;
  newNode.state = state;
  return newNode;
}