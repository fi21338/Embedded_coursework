// FILE:   Lab4_cpu01.c

#include "F28x_Project.h"

uint32_t shared_array_C1[20] = {
    10, 11, 12, 13, 14,
    15, 16, 17, 18, 19,
    20, 21, 22, 23, 24,
    25, 26, 27, 28, 30 };

// Function Prototypes

interrupt void ipc1_isr(void);

// Variables
Uint16 dacbOutput;
Uint16 AdcaResult;


void main(void)
{

    // This is CPU 1
    // Initialize System Control
    InitSysCtrl();

    // Clear all interrupts and initialize PIE control registers
    DINT;
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table
    InitPieVectTable();

    // Map ISR functions
    EALLOW;

    PieVectTable.IPC0_INT = &ipc1_isr;      // IPC1 interrupt
    EDIS;


    /*  This is typically done at the start of a program to ensure that no old or
    leftover IPC signals are present,
    providing a clean state for subsequent IPC operations. */

    IpcRegs.IPCCLR.all = 0xFFFFFFFF;




    // Enable PIE interrupts

    PieCtrlRegs.PIEIER1.bit.INTx13 = 1;     // IPC1 ISR

    // Enable global interrupts and higher priority real-time debug events:
    IER |= M_INT1;                      // Enable group 1 interrupts
    EINT;                               // Enable Global interrupt INTM
    ERTM;                               // Enable Global realtime interrupt DBGM

    // Wait here until CPU02 is ready
    while (IpcRegs.IPCSTS.bit.IPC17 == 0) ; // Wait for CPU02 to set IPC17
    IpcRegs.IPCACK.bit.IPC17 = 1;           // Acknowledge and clear IPC17

    // Main loop
    uint16_t i = 0;  // Index to iterate shared_array_C1
    while (i < 20)
    {
        // Ensure IPC flag is cleared before sending the next value
        if (IpcRegs.IPCSTS.bit.IPC1 == 0)
        {
            // Send data to CPU02
            IpcRegs.IPCSENDDATA = shared_array_C1[i];  // Load current array value into IPC data register
            IpcRegs.IPCSET.bit.IPC1 = 1;              // Trigger IPC1 flag for CPU02

            // Wait for CPU2 to explicitly acknowledge this value
            while (IpcRegs.IPCSTS.bit.IPC17 == 0);    // Wait for CPU2 to set acknowledgment flag
            IpcRegs.IPCACK.bit.IPC17 = 1;            // Clear acknowledgment flag

            i++;  // Move to the next value in the array
        }
    }

    while(1) ;
}


interrupt void ipc1_isr(void)
{
    // Read data from IPC receive register
    unsigned int receivedata = (Uint16) IpcRegs.IPCRECVDATA; // Read data on IPC receive register
    unsigned int transmitdata = receivedata + 1;             // Increment received data

    // Write the incremented data to IPC send register
    IpcRegs.IPCSENDDATA = transmitdata;                      // Write data to IPC send register
    IpcRegs.IPCSET.bit.IPC1 = 1;                             // Set IPC1 flag to notify CPU2

    // Wait if received data is 100 (prevents exit until data changes)
    while (receivedata == 100);

    // Clear interrupt flags
    IpcRegs.IPCACK.bit.IPC0 = 1;                             // Clear IPC0 flag to acknowledge CPU1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;                  // Acknowledge PIE group 1 interrupt
}



 // end of file










