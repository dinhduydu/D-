xTaskNotifyWait / xTaskNotifyWaitIndexed
[RTOS Task Notification API]

task.h


 BaseType_t xTaskNotifyWait( uint32_t ulBitsToClearOnEntry,
                             uint32_t ulBitsToClearOnExit,
                             uint32_t *pulNotificationValue,
                             TickType_t xTicksToWait );

 BaseType_t xTaskNotifyWaitIndexed( UBaseType_t uxIndexToWaitOn, 
                                    uint32_t ulBitsToClearOnEntry, 
                                    uint32_t ulBitsToClearOnExit, 
                                    uint32_t *pulNotificationValue, 
                                    TickType_t xTicksToWait );
[If you are using RTOS task notifications to implement binary or counting semaphore type behaviour then use the simpler ulTaskNotifyTake() API function instead of xTaskNotifyWait()]

Each task has an array of 'task notifications' (or just 'notifications'), each of which has a state and a 32-bit value. A direct to task notification is an event sent directly to a task that can unblock the receiving task, and optionally update one of the receiving task’s notification values in a number of different ways. For example, a notification may overwrite one of the receiving task’s notification values, or just set one or more bits in one of the receiving task’s notification values.

xTaskNotifyWait() waits, with an optional timeout, for the calling task to receive a notification. If the receiving RTOS task was already Blocked waiting for a notification when the notification it is waiting for arrives the receiving RTOS task will be removed from the Blocked state and the notification cleared.

Note: Each notification within the array operates independently – a task can only block on one notification within the array at a time and will not be unblocked by a notification sent to any other array index.

xTaskNotifyWait() and xTaskNotifyWaitIndexed() are equivalent macros - the only difference being xTaskNotifyWaitIndexed() can operate on any task notification within the array and xTaskNotifyWait() always operates on the task notification at array index 0.

xTaskNotifyGive() must not be called from an interrupt service routine. Use vTaskNotifyGiveFromISR() instead.

configUSE_TASK_NOTIFICATIONS must set to 1 in FreeRTOSConfig.h (or be left undefined) for these macros to be available. The constant configTASK_NOTIFICATION_ARRAY_ENTRIES sets the number of indexes in each task's array of task notifications.

Backward compatibility information:
Prior to FreeRTOS V10.4.0 each task had a single "notification value", and all task notification API functions operated on that value. Replacing the single notification value with an array of notification values necessitated a new set of API functions that could address specific notifications within the array. xTaskNotifyWait() is the original API function, and remains backward compatible by always operating on the notification value at index 0 within the array. Calling xTaskNotifyWait() is equivalent to calling xTaskNotifyWaitIndexed() with the uxIndexToWaitOn parameter set to 0.

Parameters:
uxIndexToWaitOn  	The index within the calling task's array of notification values on which the calling task will wait for a notification to be received.
uxIndexToWaitOn must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES.

xTaskNotifyWait() does not have this parameter and always waits for notifications on index 0.

ulBitsToClearOnEntry  	Any bits set in ulBitsToClearOnEntry will be cleared in the calling RTOS task's notification value on entry to the xTaskNotifyWait() function (before the task waits for a new notification) provided a notification is not already pending when xTaskNotifyWait() is called.
For example, if ulBitsToClearOnEntry is 0x01, then bit 0 of the task's notification value will be cleared on entry to the function.

Setting ulBitsToClearOnEntry to 0xffffffff (ULONG_MAX) will clear all the bits in the task's notification value, effectively clearing the value to 0.

ulBitsToClearOnExit  	Any bits set in ulBitsToClearOnExit will be cleared in the calling RTOS task's notification value before xTaskNotifyWait() function exits if a notification was received.
The bits are cleared after the RTOS task's notification value has been saved in *pulNotificationValue (see the description of pulNotificationValue below).

For example, if ulBitsToClearOnExit is 0x03, then bit 0 and bit 1 of the task's notification value will be cleared before the function exits.

Setting ulBitsToClearOnExit to 0xffffffff (ULONG_MAX) will clear all the bits in the task's notification value, effectively clearing the value to 0.

pulNotificationValue  	Used to pass out the RTOS task's notification value. The value copied to *pulNotificationValue is the RTOS task's notification value as it was before any bits were cleared due to the ulBitsToClearOnExit setting.
If the notification value is not required then set pulNotificationValue to NULL.

xTicksToWait  	The maximum time to wait in the Blocked state for a notification to be received if a notification is not already pending when xTaskNotifyWait() is called.
The RTOS task does not consume any CPU time when it is in the Blocked state.

The time is specified in RTOS tick periods. The pdMS_TO_TICKS() macro can be used to convert a time specified in milliseconds into a time specified in ticks.

Returns:
pdTRUE if a notification was received, or a notification was already pending when xTaskNotifyWait() was called.
pdFALSE if the call to xTaskNotifyWait() timed out before a notification was received.


Example usage:

[More examples are referenced from the main RTOS task notifications page]



/* This task shows bits within the RTOS task notification value being used to pass
different events to the task in the same way that flags in an event group might
be used for the same purpose. */
void vAnEventProcessingTask( void *pvParameters )
{
uint32_t ulNotifiedValue;

    for( ;; )
    {
        /* Block indefinitely (without a timeout, so no need to check the function's
        return value) to wait for a notification.

        Bits in this RTOS task's notification value are set by the notifying
        tasks and interrupts to indicate which events have occurred. */
        xTaskNotifyWaitIndexed( 0,         /* Wait for 0th notification. */
                                0x00,      /* Don't clear any notification bits on entry. */
                                ULONG_MAX, /* Reset the notification value to 0 on exit. */
                                &ulNotifiedValue, /* Notified value pass out in
                                                     ulNotifiedValue. */
                                portMAX_DELAY );  /* Block indefinitely. */

        /* Process any events that have been latched in the notified value. */

        if( ( ulNotifiedValue & 0x01 ) != 0 )
        {
            /* Bit 0 was set - process whichever event is represented by bit 0. */
            prvProcessBit0Event();
        }

        if( ( ulNotifiedValue & 0x02 ) != 0 )
        {
            /* Bit 1 was set - process whichever event is represented by bit 1. */
            prvProcessBit1Event();
        }

        if( ( ulNotifiedValue & 0x04 ) != 0 )
        {
            /* Bit 2 was set - process whichever event is represented by bit 2. */
            prvProcessBit2Event();
        }

        /* Etc. */
    }
}xTaskNotifyWait / xTaskNotifyWaitIndexed
[RTOS Task Notification API]

task.h


 BaseType_t xTaskNotifyWait( uint32_t ulBitsToClearOnEntry,
                             uint32_t ulBitsToClearOnExit,
                             uint32_t *pulNotificationValue,
                             TickType_t xTicksToWait );

 BaseType_t xTaskNotifyWaitIndexed( UBaseType_t uxIndexToWaitOn, 
                                    uint32_t ulBitsToClearOnEntry, 
                                    uint32_t ulBitsToClearOnExit, 
                                    uint32_t *pulNotificationValue, 
                                    TickType_t xTicksToWait );
[If you are using RTOS task notifications to implement binary or counting semaphore type behaviour then use the simpler ulTaskNotifyTake() API function instead of xTaskNotifyWait()]

Each task has an array of 'task notifications' (or just 'notifications'), each of which has a state and a 32-bit value. A direct to task notification is an event sent directly to a task that can unblock the receiving task, and optionally update one of the receiving task’s notification values in a number of different ways. For example, a notification may overwrite one of the receiving task’s notification values, or just set one or more bits in one of the receiving task’s notification values.

xTaskNotifyWait() waits, with an optional timeout, for the calling task to receive a notification. If the receiving RTOS task was already Blocked waiting for a notification when the notification it is waiting for arrives the receiving RTOS task will be removed from the Blocked state and the notification cleared.

Note: Each notification within the array operates independently – a task can only block on one notification within the array at a time and will not be unblocked by a notification sent to any other array index.

xTaskNotifyWait() and xTaskNotifyWaitIndexed() are equivalent macros - the only difference being xTaskNotifyWaitIndexed() can operate on any task notification within the array and xTaskNotifyWait() always operates on the task notification at array index 0.

xTaskNotifyGive() must not be called from an interrupt service routine. Use vTaskNotifyGiveFromISR() instead.

configUSE_TASK_NOTIFICATIONS must set to 1 in FreeRTOSConfig.h (or be left undefined) for these macros to be available. The constant configTASK_NOTIFICATION_ARRAY_ENTRIES sets the number of indexes in each task's array of task notifications.

Backward compatibility information:
Prior to FreeRTOS V10.4.0 each task had a single "notification value", and all task notification API functions operated on that value. Replacing the single notification value with an array of notification values necessitated a new set of API functions that could address specific notifications within the array. xTaskNotifyWait() is the original API function, and remains backward compatible by always operating on the notification value at index 0 within the array. Calling xTaskNotifyWait() is equivalent to calling xTaskNotifyWaitIndexed() with the uxIndexToWaitOn parameter set to 0.

Parameters:
uxIndexToWaitOn  	The index within the calling task's array of notification values on which the calling task will wait for a notification to be received.
uxIndexToWaitOn must be less than configTASK_NOTIFICATION_ARRAY_ENTRIES.

xTaskNotifyWait() does not have this parameter and always waits for notifications on index 0.

ulBitsToClearOnEntry  	Any bits set in ulBitsToClearOnEntry will be cleared in the calling RTOS task's notification value on entry to the xTaskNotifyWait() function (before the task waits for a new notification) provided a notification is not already pending when xTaskNotifyWait() is called.
For example, if ulBitsToClearOnEntry is 0x01, then bit 0 of the task's notification value will be cleared on entry to the function.

Setting ulBitsToClearOnEntry to 0xffffffff (ULONG_MAX) will clear all the bits in the task's notification value, effectively clearing the value to 0.

ulBitsToClearOnExit  	Any bits set in ulBitsToClearOnExit will be cleared in the calling RTOS task's notification value before xTaskNotifyWait() function exits if a notification was received.
The bits are cleared after the RTOS task's notification value has been saved in *pulNotificationValue (see the description of pulNotificationValue below).

For example, if ulBitsToClearOnExit is 0x03, then bit 0 and bit 1 of the task's notification value will be cleared before the function exits.

Setting ulBitsToClearOnExit to 0xffffffff (ULONG_MAX) will clear all the bits in the task's notification value, effectively clearing the value to 0.

pulNotificationValue  	Used to pass out the RTOS task's notification value. The value copied to *pulNotificationValue is the RTOS task's notification value as it was before any bits were cleared due to the ulBitsToClearOnExit setting.
If the notification value is not required then set pulNotificationValue to NULL.

xTicksToWait  	The maximum time to wait in the Blocked state for a notification to be received if a notification is not already pending when xTaskNotifyWait() is called.
The RTOS task does not consume any CPU time when it is in the Blocked state.

The time is specified in RTOS tick periods. The pdMS_TO_TICKS() macro can be used to convert a time specified in milliseconds into a time specified in ticks.

Returns:
pdTRUE if a notification was received, or a notification was already pending when xTaskNotifyWait() was called.
pdFALSE if the call to xTaskNotifyWait() timed out before a notification was received.


Example usage:

[More examples are referenced from the main RTOS task notifications page]



/* This task shows bits within the RTOS task notification value being used to pass
different events to the task in the same way that flags in an event group might
be used for the same purpose. */

void vAnEventProcessingTask( void *pvParameters )
{
uint32_t ulNotifiedValue;

    for( ;; )
    {
        /* Block indefinitely (without a timeout, so no need to check the function's
        return value) to wait for a notification.

        Bits in this RTOS task's notification value are set by the notifying
        tasks and interrupts to indicate which events have occurred. */
        xTaskNotifyWaitIndexed( 0,         /* Wait for 0th notification. */
                                0x00,      /* Don't clear any notification bits on entry. */
                                ULONG_MAX, /* Reset the notification value to 0 on exit. */
                                &ulNotifiedValue, /* Notified value pass out in
                                                     ulNotifiedValue. */
                                portMAX_DELAY );  /* Block indefinitely. */

        /* Process any events that have been latched in the notified value. */

        if( ( ulNotifiedValue & 0x01 ) != 0 )
        {
            /* Bit 0 was set - process whichever event is represented by bit 0. */
            prvProcessBit0Event();
        }

        if( ( ulNotifiedValue & 0x02 ) != 0 )
        {
            /* Bit 1 was set - process whichever event is represented by bit 1. */
            prvProcessBit1Event();
        }

        if( ( ulNotifiedValue & 0x04 ) != 0 )
        {
            /* Bit 2 was set - process whichever event is represented by bit 2. */
            prvProcessBit2Event();
        }

        /* Etc. */
    }
}