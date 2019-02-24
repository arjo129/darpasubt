//! Timer module that uses the low frequency clock.
/*!
  This module uses the low frequency clock (LFCLK) to create, start and stop timer instances.

  NOTE:
    -# APP_TIMER_DEF(timerIdentifier) must be called to create a timer identifier and thus statically allocate memory for it.
    -# Ensure APP_TIMER_ENABLED is set to '1' and respective configurations are set in sdk_config.h before using this module.
*/

#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "app_error.h"

/* Local function prototypes */
static void lfclkRequest(void);

/**
 * @brief Initialises the low frequency timer (LFCLK).
 * 
 */
void lowTimerInit(void)
{
  lfclkRequest();
  app_timer_init();
}

/**
 * @brief Creates a repeated timer instance of the low frequency timer.
 * 
 * @details Only one instance is allowed when using FreeRTOS.
 * 
 * @param timerId Pointer to timer identifier.
 * @param handler Function to be executed when timer runs out.
 */
void lowTimerRepeatCreate(const app_timer_id_t *timerId, app_timer_timeout_handler_t handler)
{
  ret_code_t errCode;
  errCode = app_timer_create(timerId, APP_TIMER_MODE_REPEATED, handler);
  APP_ERROR_CHECK(errCode);
}

/**
 * @brief Starts the repeated timer instance.
 * 
 * @param timerId Identifier of timer to start.
 * @param timeout Number of milliseconds to time-out event (minimum 5ms).
 */
void lowTimerRepeatStart(app_timer_id_t timerId, uint32_t timeout)
{
  ret_code_t errCode = app_timer_start(timerId, timeout, NULL);
  APP_ERROR_CHECK(errCode);
}

/**
 * @brief Stops the timer instance.
 * 
 * @param timerId Identifier of the timer to stop.
 */
void lowTimerStop(app_timer_id_t timerId)
{
  ret_code_t errCode = app_timer_stop(timerId);
  APP_ERROR_CHECK(errCode);
}

/* Local functions */

/**
 * @brief Function starting the internal LFCLK oscillator.
 * 
 * @details This is needed by RTC1 which is used by the Application Timer
 *          (When SoftDevice is enabled the LFCLK is always running and this is not needed).
 */
static void lfclkRequest(void)
{
  ret_code_t errCode = nrf_drv_clock_init();
  APP_ERROR_CHECK(errCode);
  nrf_drv_clock_lfclk_request(NULL);
}