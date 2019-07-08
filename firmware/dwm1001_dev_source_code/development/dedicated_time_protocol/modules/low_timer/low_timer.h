#include "app_timer.h"

/* Public function prototypes */
void lowTimerInit(void);
void lowTimerRepeatCreate(const app_timer_id_t *timerId, app_timer_timeout_handler_t handler);
void lowTimerSingleCreate(const app_timer_id_t *timerId, app_timer_timeout_handler_t handler);
void lowTimerStart(app_timer_id_t timerId, uint32_t timeout);
void lowTimerStop(app_timer_id_t timerId);
void lowTimerStopAll(void);
