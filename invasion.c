#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <bcm_host.h>
#include <time.h>


#define BLOCK_SIZE 4096

// TODO Define the GPIO pin offset
#define GPIO_OFFSET 0x200000

volatile unsigned int *gpio;
void *gpioMap;
int fdGPIO;

// TODO Define the GPIO pin numbers here. Look at top left of 'explorer-hat-schematic' image

#define TX 9    //MISO  output
#define RX 10   // MOSI input

#define GPFSEL0 0
#define GPFSEL1 1

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD 37
#define GPPUDCLK0 38



// TODO Define the GPFSEL and GPSET and GPCLR registers here - look in peripheral manual starting at pp. 89





void initSensor() {
    unsigned peripheralBase = bcm_host_get_peripheral_address();
    
    // TODO print out peripheralBase and peripheralBase + OFFSET to confirm
    fprintf( stderr, "%08x %08x\n", peripheralBase, peripheralBase + GPIO_OFFSET );
    fdGPIO = open("/dev/mem", O_RDWR|O_SYNC);
    gpioMap = (unsigned int *)mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        fdGPIO,
        peripheralBase + GPIO_OFFSET
    );
    
    if ( gpioMap == MAP_FAILED ) {
        fprintf( stderr, "The memory map initialization failed.\n");
        perror( "mmap" );
        return;
    }
    
    gpio = (volatile unsigned int *) gpioMap;

    register unsigned int r;
    // configures gpio pin 9 to output
    r = gpio[GPFSEL0];
    r &= ~(0x7 << 27);
    r |= (0x1 << 27);  
    gpio[GPFSEL0] = r;
    
    //configure gpio 10 to input
    r = gpio[GPFSEL1];
    r &= ~(0x7);//clear the LSB in GPSEL1
    gpio[GPFSEL1] = r; // update memory
    
    // TODO Set LED1 to output
    /// STEPS:
    /// set register r by accessing gpio[]
    /// Clear the 3 bits for this pin to 0 using &=
    /// Set the 3 bits to output using |=
    /// Set gpio[] = r
    
    // TODO Set LED4 to output
    /// STEPS:
    /// set register r by accessing gpio[]
    /// Clear the 3 bits for this pin to 0 using &=
    /// Set the 3 bits to output using |=
    /// Set gpio[] = r

        

    /// NOTHING TODO HERE. Disabling done for you.
    // Disable the pull-up/pull-down control line for GPIO pin 23. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    gpio[GPPUD] = 0x0;
    r = 150;
    while (r--) {
      asm volatile("nop");
    }
    gpio[GPPUDCLK0] = (0x1 << TX);
    r = 150;
    while (r--) {
      asm volatile("nop");
    }
    gpio[GPPUDCLK0] = 0;
    
    // clearing the output line
    gpio[GPCLR0] = 1 << TX;
    //gpio[GPCLR0] = 1 << RX;
}


void freeSensor() {
    munmap( gpioMap, BLOCK_SIZE );
    close( fdGPIO );
}

// TODO Implement all 4 functions here
void setTX() {
    register unsigned int r;
    
    r = (0x1 << 9);
    gpio[GPSET0] = r;
}

void clearTX() {
    register unsigned int r;
    
    r = (0x1 << 9);
    gpio[GPCLR0] = r;
    
}

int readRX() {
    register unsigned int r = gpio[GPLEV0];
    
    r = ((r >> RX) & 0x1);
    return r;
}

    

