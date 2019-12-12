// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RH_RF95.h>
#include <RHMesh.h>
#include <SoftwareSerial.h>
#include <common.h>
#include <Transport.h>
#include <main.h>

#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial soft_serial (RX_SOFTSERIAL_PIN, TX_SOFTSERIAL_PIN);
RH_RF95<SoftwareSerial> driver(soft_serial);
#endif

#ifdef USE_HARDWARE_SERIAL
RH_RF95<HardwareSerial> driver(HARD_SERIAL);
#endif

// Class to manage message delivery and receipt, using the driver declared above
RHMesh mesh_manager(driver, MESH_ADDRESS);
// Messages data
uint8_t data[] = "AAAAAAAAAB";
// Dont put this on the stack:
uint8_t buf[MAX_DATA_SIZE];
uint8_t buf_len = sizeof(buf);
uint8_t dest = 2;

Transport::Transport transporter(MESH_ADDRESS, &mesh_manager);

void setup() 
{
    Serial.begin(115200);

    while (!mesh_manager.init()) {}
    config_network(&driver, &mesh_manager);

    #ifdef DEBUG_PRINT
    Serial.println(F(MSG_INIT_SUCCESS));
    Serial.println(F(MSG_WHOAMI));
    #endif
}


void loop()
{
    transporter.process_send_queue();

    if (MESH_ADDRESS == 1)
    {
        Chunk::Chunk chunk(data, sizeof(data)-1, dest);
        transporter.queue_chunk(chunk);
    }
    else if (MESH_ADDRESS == 2)
    {
        bool complete = false;
        Chunk::Chunk recv = transporter.receive(complete);
        if (complete)
        {
            Serial.println((char*)recv.get_data());
        }
    }
}

/**
 * @brief Configures network with defined values.
 * 
 * @param driver radio driver used.
 * @param mesh_manager mesh network manager used.
 */
void config_network(RH_RF95<SoftwareSerial>* driver, RHMesh* mesh_manager)
{
    driver->setFrequency(434.0);
    driver->setTxPower(TX_POWER);
    mesh_manager->setRetries(RE_TX_RETRIES);
    mesh_manager->setTimeout(RE_TX_TIMEOUT);
    mesh_manager->setArpTimeout(MESH_ARP_TIMEOUT);
}