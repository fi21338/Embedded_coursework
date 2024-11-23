// Included Files
#include "driverlib.h"
#include "device.h"
#include "board.h"


// Global Variables
volatile uint32_t debounceCounter = 0;
#define DEBOUNCE_DELAY 10000 // Wait of ~10ms


// Interrupt Handler
__interrupt void gpioInterruptHandler(void);


// Main
void main(void)
{
    // Initializes system control, device clock, and peripherals
    Device_init();

    // Initializes PIE and clear PIE registers. Disables CPU interrupts.
    // and clear all CPU interrupt flags.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell interrupt
    // Service Routines (ISR).
    Interrupt_initVectorTable();

    // Board Initialization
    Board_init();

    // GPIO Configuration
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_FALLING_EDGE);  // Trigger GPIO on a falling edge, occurs on release of button press.
    GPIO_setInterruptPin(myGPIOInputInterrupt0, GPIO_INT_XINT1);  // Map the input GPIO pin to the external interrupt (XINT1)
    GPIO_enableInterrupt(GPIO_INT_XINT1);

    // Interrupt Registration
    Interrupt_register(INT_XINT1, &gpioInterruptHandler);
    Interrupt_enable(INT_XINT1);

    // Enables CPU interrupts
    Interrupt_enableMaster();

    // Idle Loop.
    for(;;)
    {
        // Decrease counter if waiting.
        if (debounceCounter > 0) {
            debounceCounter--;
        }
    }
}

__interrupt void gpioInterruptHandler(void)
{
    // Waited long enough for debounce, perform action and reset the debounce counter.
    if (debounceCounter <= 0) {
        // Perform action, toggle GPIO pin.
        GPIO_togglePin(myGPIOOutput0);

        // Reset counter.
        debounceCounter = DEBOUNCE_DELAY;
    }


    // Clear the interrupt flag.
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


// End of File


