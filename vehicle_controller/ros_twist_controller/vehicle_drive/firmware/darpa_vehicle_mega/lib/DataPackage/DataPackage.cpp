#include <DataPackage.h>
#include <Arduino.h>

void DataPackage::serialiseData(DataPackage::Packet packet, uint8_t serial[DataPackage::DATA_LEN])
{
  memcpy(&serial[X_1_IDX], &packet.js1X, JS_XY_SIZE);
  memcpy(&serial[Y_1_IDX], &packet.js1Y, JS_XY_SIZE);
  memcpy(&serial[X_2_IDX], &packet.js2X, JS_XY_SIZE);
  memcpy(&serial[Y_2_IDX], &packet.js2Y, JS_XY_SIZE);
  memcpy(&serial[SW_1_IDX], &packet.js1SW, JS_SW_SIZE);
  memcpy(&serial[SW_2_IDX], &packet.js2SW, JS_SW_SIZE);
}

DataPackage::Packet DataPackage::deserialiseData(uint8_t serial[DataPackage::DATA_LEN])
{
  DataPackage::Packet packet;

  memcpy(&packet.js1X, &serial[X_1_IDX], JS_XY_SIZE);
  memcpy(&packet.js1Y, &serial[Y_1_IDX], JS_XY_SIZE);
  memcpy(&packet.js2X, &serial[X_2_IDX], JS_XY_SIZE);
  memcpy(&packet.js2Y, &serial[Y_2_IDX], JS_XY_SIZE);
  memcpy(&packet.js1SW, &serial[SW_1_IDX], JS_SW_SIZE);
  memcpy(&packet.js2SW, &serial[SW_2_IDX], JS_SW_SIZE);

  return packet;
}
