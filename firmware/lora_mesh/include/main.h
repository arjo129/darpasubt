#ifndef main_h
#define main_h

#include <RH_RF95.h>
#include <RHMesh.h>
#include <SoftwareSerial.h>

#ifdef USE_SOFTWARE_SERIAL
void config_network(RH_RF95<SoftwareSerial>* driver, RHMesh* mesh_manager);
#endif

#ifdef USE_HARDWARE_SERIAL
void config_network(RH_RF95<HardwareSerial>* driver, RHMesh* mesh_manager);
#endif

#endif