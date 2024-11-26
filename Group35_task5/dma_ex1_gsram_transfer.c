
// Included Files
#include "driverlib.h"
#include "device.h"

//
// DMA data sections
//
#pragma DATA_SECTION(array_1, "ramgs0");  // map the TX data to memory
#pragma DATA_SECTION(array_2, "ramgs1");  // map the RX data to memory

//
// Defines
//
#define BURST       8       // write 8 to the register for a burst size of 8
#define TRANSFER    13     // [(MEM_BUFFER_SIZE/(BURST)]

//
// Globals
//
uint16_t array_1[100] = {
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
};;   // Send data buffer
uint16_t array_2[100];   // Receive data buffer
volatile uint16_t done;

//
// Function Prototypes
//
__interrupt void dmaCh6ISR(void);
void initDMA(void);
void error();

//
// Main
//
void main(void)
{
    uint16_t i;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    //Device_initGPIO(); //skipped for this example

    //
    //Initialize PIE and clear PIE registers. Disables CPU interrupts
    //
    Interrupt_initModule();

    //
    //Initialize the PIE vector table with pointers to the shell Interrupt
    //Service Routines (ISR)
    //
    Interrupt_initVectorTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    Interrupt_register(INT_DMA_CH6, &dmaCh6ISR);

    //
    // Initialize the Device Peripherals:
    //
    initDMA();  // set up the dma

    //
    // Ensure DMA is connected to Peripheral Frame 2 bridge (EALLOW protected)
    //
    SysCtl_selectSecMaster(0, SYSCTL_SEC_MASTER_DMA);

    //
    // User specific code, enable interrupts:
    // Initialize the data buffers
    //
    for(i = 0; i < 100; i++)
    {
        array_1[i] = i;
        array_2[i] = 0;
    }

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(INT_DMA_CH6);
    EINT;                                // Enable Global Interrupts
    // Start DMA channel
    DMA_startChannel(DMA_CH6_BASE);

    done = 0;           // Test is not done yet

    while(!done)        // wait until the DMA transfer is complete
    {
       DMA_forceTrigger(DMA_CH6_BASE);
       DEVICE_DELAY_US(1000);

    }

    //
    // When the DMA transfer is complete the program will stop here
    //
    ESTOP0;
}

//
// error - Error Function which will halt the debugger
//
void error(void)
{
    ESTOP0;  //Test failed!! Stop!
    for (;;);
}

//
// dma_init - DMA setup for both TX and RX channels.
//
void initDMA()
{
    //
    // Refer to dma.c for the descriptions of the following functions.
    //

    //
    //Initialize DMA
    //
    DMA_initController();

    const void *destAddr;
    const void *srcAddr;
    srcAddr = (const void *)array_1;
    destAddr = (const void *)array_2;

    //
    // configure DMA CH6
    //
    DMA_configAddresses(DMA_CH6_BASE, destAddr, srcAddr);
    DMA_configBurst(DMA_CH6_BASE,BURST,1,1);
    DMA_configTransfer(DMA_CH6_BASE,TRANSFER,1,1);
    DMA_configMode(DMA_CH6_BASE,DMA_TRIGGER_SOFTWARE, DMA_CFG_ONESHOT_DISABLE);
    DMA_setInterruptMode(DMA_CH6_BASE,DMA_INT_AT_END);
    DMA_enableTrigger(DMA_CH6_BASE);
    DMA_enableInterrupt(DMA_CH6_BASE);
}

//
// local_D_INTCH6_ISR - DMA Channel6 ISR
//
__interrupt void dmaCh6ISR(void)
{
    uint16_t i;

    DMA_stopChannel(DMA_CH6_BASE);
    // ACK to receive more interrupts from this PIE group
    EALLOW;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
    EDIS;

    for( i = 0; i < 100; i++ )
    {
        //
        // check for data integrity
        //
        if (array_2[i] != i)
        {
            error();
        }
    }

    done = 1; // Test done.
    return;
}

//
// End of file
//
