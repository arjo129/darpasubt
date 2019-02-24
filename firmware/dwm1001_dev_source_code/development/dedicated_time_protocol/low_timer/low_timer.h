#include "app_timer.h"

/* Public function prototypes */
void lowTimerInit(void);
void lowTimerRepeatCreate(const app_timer_id_t *timerId, app_timer_timeout_handler_t handler);
void lowTimerRepeatStart(app_timer_id_t timerId, uint32_t timeoutTicks);
void lowTimerStop(app_timer_id_t timerId);
