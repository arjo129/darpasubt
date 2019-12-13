// Mesh network implementation using the Grove LoRa RFM95 module.

#include <RH_RF95.h>
#include <RHMesh.h>
#include <SoftwareSerial.h>
#include <serial_reader.h>
#include <common.h>
#include <Transport.h>
#include <main.h>

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



#ifdef USE_SOFTWARE_SERIAL
/**
 * @brief Configures network with defined values. For Software Serial.
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

SoftwareSerial soft_serial (RX_SOFTSERIAL_PIN, TX_SOFTSERIAL_PIN);
RH_RF95<SoftwareSerial> driver(soft_serial);
#endif



///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



#ifdef USE_HARDWARE_SERIAL
/**
 * @brief Configures network with defined values. For Hardware Serial.
 * 
 * @param driver radio driver used.
 * @param mesh_manager mesh network manager used.
 */
void config_network(RH_RF95<HardwareSerial>* driver, RHMesh* mesh_manager)
{
    driver->setFrequency(434.0);
    driver->setTxPower(TX_POWER);
    mesh_manager->setRetries(RE_TX_RETRIES);
    mesh_manager->setTimeout(RE_TX_TIMEOUT);
    mesh_manager->setArpTimeout(MESH_ARP_TIMEOUT);
}

RH_RF95<HardwareSerial> driver(HARD_SERIAL);
#endif



///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////



// Class to manage message delivery and receipt, using the driver declared above
RHMesh mesh_manager(driver, MESH_ADDRESS);
// Messages data
uint8_t data[] = "AAAAAAAAAB";
// Dont put this on the stack:
uint8_t dest = 2;

Transport::Transport transporter(MESH_ADDRESS, &mesh_manager);

void setup() 
{
    Serial.begin(SERIAL_BAUD_RATE);

    while (!mesh_manager.init()) {}
    config_network(&driver, &mesh_manager);

    #ifdef DEBUG_PRINT
    Serial.println(F(MSG_INIT_SUCCESS));
    Serial.println(F(MSG_WHOAMI));
    #endif
}


void loop()
{
    serial_reader::read();
    if (serial_reader::terminate)
    {
        Chunk::Chunk chunk(serial_reader::buffer, serial_reader::current_len, dest);
        transporter.queue_chunk(chunk);
        serial_reader::clear();
    }
    
    transporter.process_send_queue();

    // if (MESH_ADDRESS == 1)
    // {
    //     Chunk::Chunk chunk(data, sizeof(data)-1, dest);
    //     transporter.queue_chunk(chunk);
    // }
    // else if (MESH_ADDRESS == 2)
    // {
    //     bool complete = false;
    //     Chunk::Chunk recv = transporter.receive(complete);
    //     if (complete)
    //     {
    //         Serial.println((char*)recv.get_data());
    //     }
    // }
}
