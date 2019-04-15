#include <RH_RF95.h>

/**
 * @brief Client class that allows requests to be made a server.
 * 
 */
class Client
{
  private:
    SoftwareSerial softSerial;
    RH_RF95 client; // Instance of the radio driver in operation.
    uint8_t address; // Unique identifier for this server.
    float frequency; // Operation frequency.

  public:
    Client (uint8_t address, uint8_t receivePin, uint8_t transmitPin, float frequency);
    bool init(void);
    uint8_t getAddress(void);
    void loop(void);
};