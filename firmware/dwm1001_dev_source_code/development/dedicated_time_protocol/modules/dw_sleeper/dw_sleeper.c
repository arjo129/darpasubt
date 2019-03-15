//! Module that controls the sleeping function of DW1000 device.
#include "deca_device_api.h"

#define DUMMY_SIZE 130 // Byte length set for SPI 2MHz

/**
 * @brief Puts the device to SLEEP state.
 * 
 * @param rxOnWake set to 1 if receiver needs to be turned on upon wake
 */
void dwSleep(int rxOnWake)
{
  uint16 mode = DWT_PRESRV_SLEEP | DWT_CONFIG;

  if (rxOnWake)
  {
    mode |= DWT_RX_EN;
  }

  dwt_configuresleep(mode, DWT_WAKE_CS | DWT_SLP_EN);
  dwt_entersleep();
}

/**
 * @brief Wakes up the device from SLEEP state.
 * 
 * @return int DWT_SUCCESS for success, or DWT_ERROR for error 
 */
int dwWake(void)
{
  // We use a dummy buffer to read from SPI for at least 500us 
  // in order to wake up the device.
  // The bytes length is chosen based on SPI speed.
  uint8 dummyBuf[DUMMY_SIZE];
  return dwt_spicswakeup(dummyBuf, DUMMY_SIZE);
}
