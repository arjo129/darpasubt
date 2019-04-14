#include <RH_RF95.h>

#define LED_PIN 13

/**
 * @brief Server class that can service multiple clients.
 * 
 */
class Server
{
  private:
    SoftwareSerial *softSerial;
    RH_RF95 *server; // Instance of the radio driver in operation.
    uint8_t address; // Unique identifier for this server.
    float frequency; // Operation frequency.

  public:
    Server(uint8_t address, uint8_t receivePin, uint8_t transmitPin, float frequency);
    bool init(void);
    uint8_t getAddress(void);
    void receive(void);
};
