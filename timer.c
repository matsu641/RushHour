#include <stdint.h>

#define HEX3_HEX0_BASE  (0xFF200020) //0xFF200020 for HEX3..HEX0
#define HEX7_HEX4_BASE  (0xFF200030) //0xFF200030 for HEX7..HEX4

// Create pointers to these addresses for 32-bit writes
volatile unsigned int *HEX3_HEX0_ptr = (volatile unsigned int *) HEX3_HEX0_BASE;
volatile unsigned int *HEX7_HEX4_ptr = (volatile unsigned int *) HEX7_HEX4_BASE;

#define WAIT_CYCLES (50000000) // For ~1s on a 50MHz system

//7 seg 
static const unsigned char seven_seg_lookup[16] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
};

unsigned char digit_to_7seg(int digit) {
    if (digit < 0 || digit > 15) {
        return 0x00; 
    }
    return seven_seg_lookup[digit];
}

//Display a decimal value on HEX5-HEX0

void display_on_hex_6(int count) {
    // Extract decimal digits
    int d0 = count % 10;
    int d1 = (count / 10) % 10;
    int d2 = (count / 100) % 10; 
    int d3 = (count / 1000) % 10;
    int d4 = (count / 10000) % 10;
    int d5 = (count / 100000) % 10;

    // Convert to 7-seg codes
    unsigned char seg0 = digit_to_7seg(d0); //HEX0
    unsigned char seg1 = digit_to_7seg(d1); //HEX1
    unsigned char seg2 = digit_to_7seg(d2); //HEX2
    unsigned char seg3 = digit_to_7seg(d3); //HEX3
    unsigned char seg4 = digit_to_7seg(d4); //HEX4
    unsigned char seg5 = digit_to_7seg(d5); //HEX5

    // Pack HEX3..HEX0 by shifting the corresponding bit
    unsigned int lower_4 =  (seg3 << 24) | (seg2 << 16) | (seg1 <<  8) | (seg0 <<  0);

    // Pack HEX7..HEX4
    unsigned int upper_4 =  (seg5 <<  8) | (seg4 <<  0);

    // Write number on each HEX
    *HEX3_HEX0_ptr = lower_4;
    *HEX7_HEX4_ptr = upper_4;
}

//delay
void delay_1s() {
    for (volatile int i = 0; i < WAIT_CYCLES; i++) {
        // burn cycles
    }
}

//main
int main(void) {
    int count = 0;

    while (1) {
        display_on_hex_6(count); //
        count = (count + 1) % 1000000; // increment, rollover at 999999
        delay_1s(); // ~1 second
    }

    return 0;
}
