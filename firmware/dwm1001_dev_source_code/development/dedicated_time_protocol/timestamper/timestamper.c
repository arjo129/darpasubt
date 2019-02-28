#include "deca_device_api.h"
#include "port_platform.h"

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn getTxTimestampU64()
 *
 * @brief Get the TX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
uint64 getTxTimestampU64(void) {
    uint8 tsTab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(tsTab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= tsTab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn getRxTimestampU64()
 *
 * @brief Get the RX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
uint64 getRxTimestampU64(void) {
    uint8 tsTab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(tsTab);
    for (i = 4; i >= 0; i--) {
        ts <<= 8;
        ts |= tsTab[i];
    }
    return ts;
}