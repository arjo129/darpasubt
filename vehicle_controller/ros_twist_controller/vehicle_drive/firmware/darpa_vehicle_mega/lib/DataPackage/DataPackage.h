#include <string.h>
#include <stdint.h>

namespace DataPackage
{
  
  namespace
  {
    const uint16_t JS_XY_SIZE = 2;
    const uint16_t JS_SW_SIZE = 1;
    const uint16_t DATA_LEN = 10; // Total of 10 bytes.

    // Position index of various values.
    const uint16_t X_1_IDX = 0;
    const uint16_t Y_1_IDX = 2;
    const uint16_t X_2_IDX = 4;
    const uint16_t Y_2_IDX = 6;
    const uint16_t SW_1_IDX = 8;
    const uint16_t SW_2_IDX = 9;
  }

  struct Packet
  {
    uint16_t js1X = 0;
    uint16_t js1Y = 0;
    uint16_t js1SW = 0;
    uint16_t js2X = 0; 
    uint16_t js2Y = 0;
    uint16_t js2SW = 0;
  };

  void serialiseData(Packet packet, uint8_t serial[DATA_LEN]);
  Packet deserialiseData(uint8_t serial[DATA_LEN]);
}