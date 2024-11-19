//
// Included Files
//
#include "driverlib.h"
#include "device.h"

// Global Variables
long long timer0;
#include <math.h>

// Timer Globals
uint16_t cpuTimer0IntCount;
uint16_t cpuTimer1IntCount;

// FIR Filter Definitions
#define SAMPLING_FREQUENCY 44000     // Sampling frequency in Hz
#define SIGNAL_LENGTH 1024           // Length of the input signal
#define FIR_FILTER_LENGTH 127        // Number of taps for the FIR filter

// FIR Filter Coefficients
const float firCoeffs[FIR_FILTER_LENGTH] = {
    0.000345, 0.000300, 0.000243, 0.000171, 0.000085,
    -0.000018, -0.000137, -0.000271, -0.000416, -0.000568,
    -0.000719, -0.000860, -0.000980, -0.001066, -0.001105,
    -0.001083, -0.000990, -0.000815, -0.000554, -0.000205,
    0.000225, 0.000725, 0.001278, 0.001859, 0.002435,
    0.002973, 0.003432, 0.003773, 0.003954, 0.003940,
    0.003701, 0.003214, 0.002468, 0.001467, 0.000228,
    -0.001217, -0.002817, -0.004509, -0.006212, -0.007838,
    -0.009287, -0.010454, -0.011237, -0.011536, -0.011259,
    -0.010331, -0.008694, -0.006312, -0.003177, 0.000695,
    0.005255, 0.010427, 0.016109, 0.022172, 0.028468,
    0.034831, 0.041086, 0.047056, 0.052564, 0.057444,
    0.061546, 0.064741, 0.066930, 0.068042, 0.068042,
    0.066930, 0.064741, 0.061546, 0.057444, 0.052564,
    0.047056, 0.041086, 0.034831, 0.028468, 0.022172,
    0.016109, 0.010427, 0.005255, 0.000695, -0.003177,
    -0.006312, -0.008694, -0.010331, -0.011259, -0.011536,
    -0.011237, -0.010454, -0.009287, -0.007838, -0.006212,
    -0.004509, -0.002817, -0.001217, 0.000228, 0.001467,
    0.002468, 0.003214, 0.003701, 0.003940, 0.003954,
    0.003773, 0.003432, 0.002973, 0.002435, 0.001859,
    0.001278, 0.000725, 0.000225, -0.000205, -0.000554,
    -0.000815, -0.000990, -0.001083, -0.001105, -0.001066,
    -0.000980, -0.000860, -0.000719, -0.000568, -0.000416,
    -0.000271, -0.000137, -0.000018, 0.000085, 0.000171,
};

// FIR Filter Buffers
float filtbuff1[FIR_FILTER_LENGTH] = {0};    // Circular buffer for FIR filter
float signal[SIGNAL_LENGTH];                 // Input signal
int signal_FixedP[SIGNAL_LENGTH];            // Fixed-point representation of input signal
float filteredSignal[SIGNAL_LENGTH];         // Filtered signal
float firState[FIR_FILTER_LENGTH + SIGNAL_LENGTH - 1]; // FIR state buffer

// Variables to store intermediate data
float Input_Data;
float Output_Data1;

// Buffers for processing
float buffer0[SIGNAL_LENGTH];
float buffer1[SIGNAL_LENGTH];
int buffer1Fixed[SIGNAL_LENGTH];

// Function Prototypes
__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
void initCPUTimers(void);
void configCPUTimer(uint32_t, float, float);
void generateSignal(float *signal, int length);
float filter1(float);

// Main
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Initialize PIE and clear PIE registers
    Interrupt_initModule();
    Interrupt_initVectorTable();  // Set up ISR vector table

    // Register ISRs for each CPU Timer interrupt
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);

    // Initialize the device peripherals (CPU Timers)
    initCPUTimers();

    // Configure CPU-Timers 0 and 1
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 23);    // 0.5-second period
    configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 2000000); // 2-second period

    // Enable interrupts for CPU Timers
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);

    // Start CPU Timer 0
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Generate the input signal
    generateSignal(signal, SIGNAL_LENGTH);

    // Enable Global Interrupt (INTM) and real-time interrupt (DBGM)
    EINT;
    ERTM;

    // IDLE loop
    while (1)
    {
        // Wait for timer interrupts to handle processing
    }
}

// Initialize CPU Timers to a known state
void initCPUTimers(void)
{
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);   // Set timer period to maximum
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);

    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);         // No clock prescaling
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);

    CPUTimer_stopTimer(CPUTIMER0_BASE);               // Ensure timers are stopped initially
    CPUTimer_stopTimer(CPUTIMER1_BASE);

    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);      // Reload counter registers
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);

    cpuTimer0IntCount = 0;
    cpuTimer1IntCount = 0;
}

// Configure a CPU timer with specified frequency and period
void configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp = (uint32_t)(freq / 1000000 * period);

    CPUTimer_setPeriod(cpuTimer, temp);
    CPUTimer_setPreScaler(cpuTimer, 0);
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);

    // Reset interrupt counter
    if (cpuTimer == CPUTIMER0_BASE) {
        cpuTimer0IntCount = 0;
    } else if (cpuTimer == CPUTIMER1_BASE) {
        cpuTimer1IntCount = 0;
    }
}

// ISR for CPU Timer 0
__interrupt void cpuTimer0ISR(void)
{
    static short i;
    cpuTimer0IntCount++;

    Input_Data = signal[i++];
    Output_Data1 = filter1(Input_Data);

    buffer0[i] = Input_Data;
    buffer1[i] = Output_Data1;
    buffer1Fixed[i] = (int)(32768 * buffer1[i]);

    if (i == SIGNAL_LENGTH) {
        i = 0;  // Reset index if it reaches the end of the signal
    }

    // Acknowledge the interrupt to receive further interrupts from group 1
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

// ISR for CPU Timer 1
__interrupt void cpuTimer1ISR(void)
{
    cpuTimer1IntCount++;

    // Acknowledge the interrupt to receive further interrupts from group 1
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

// Function to generate an input signal (two sine waves combined)
void generateSignal(float *signal, int length)
{
    float t;
    int i;
    for (i = 0; i < length; i++) {
        t = (float)i / SAMPLING_FREQUENCY;
        signal[i] = (sinf(2 * M_PI * 1000 * t) + sinf(2 * M_PI * 2000 * t)) * 0.5;
        signal_FixedP[i] = (int)(signal[i] * 32768);  // Fixed-point representation
    }
}

// Function to implement FIR filtering
float filter1(float Input_Data)
{
    int i;
    float sum = 0;

    // shift old samples from filtbuff1 to make space for the new input sample
    for (i = FIR_FILTER_LENGTH - 1; i > 0; i--) {
        filtbuff1[i] = filtbuff1[i - 1];
    }

    // add the new sample at the start of the buffer
    filtbuff1[0] = Input_Data;

    // convolution of the input with the FIR coefficients
    for (i = 0; i < FIR_FILTER_LENGTH; i++) {
        // weighted sum of the samples
        sum += filtbuff1[i] * firCoeffs[i];
    }

    return sum;
}

//
// End of File
//



