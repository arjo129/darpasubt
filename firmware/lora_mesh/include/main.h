#ifndef main_h
#define main_h

#include <RH_RF95.h>
#include <RHMesh.h>
#include <SoftwareSerial.h>

void config_network(RH_RF95<SoftwareSerial>* driver, RHMesh* mesh_manager);

#endif