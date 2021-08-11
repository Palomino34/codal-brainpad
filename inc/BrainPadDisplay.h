#ifndef BPDISPLAY_H
#define BPDISPLAY_H

#include "I2C.h"
#include "mbed.h"
#include "CodalConfig.h"
#include "MbedPin.h"
#include "BrainPadPin.h"

#include <cstdint>

#define DISPLAY_DEFAULT_ADDRESS 0x78
#define RESET_PIN_NAME PB_2
#define BRAINPADPULSE 1

#ifdef BRAINPADPULSE
namespace codal {
    class BrainPadDisplay {
        static const int vramSize = (128 * 64 / 8) + 1;

        codal::I2C& i2c;
        uint8_t data[2];
        uint8_t vram[vramSize];
        uint16_t address;

        void writeCommand(int command);
        void flush();

    public:
        BrainPadDisplay(codal::I2C& _i2c, uint16_t address = DISPLAY_DEFAULT_ADDRESS);

        void writeScreenBuffer(uint8_t* buffer);
    };
}
#else
#define BRAINPAD_TICK_LED1 PB_14
#define BRAINPAD_TICK_LED2 PA_10
#define BRAINPAD_TICK_LED3 PA_14
#define BRAINPAD_TICK_LED4 PA_15
#define BRAINPAD_TICK_LED5 PA_8
#define BRAINPAD_TICK_LED6 PB_13
#define BRAINPAD_TICK_LED7 PB_15
#define BRAINPAD_TICK_LED8 PB_6
#define BRAINPAD_TICK_LED9 PA_13
#define BRAINPAD_TICK_LED10 PC_14
#define BRAINPAD_TICK_LED11 PB_12
#define BRAINPAD_TICK_LED12 PB_2
#define BRAINPAD_TICK_LED13 PH_3
#define BRAINPAD_TICK_LED14 PC_15
#define BRAINPAD_TICK_LED15 PB_8
#define BRAINPAD_TICK_LED16 PB_1
#define BRAINPAD_TICK_LED17 PB_0
#define BRAINPAD_TICK_LED18 PA_4
#define BRAINPAD_TICK_LED19 PH_1
#define BRAINPAD_TICK_LED20 PB_9
#define BRAINPAD_TICK_LED21 PA_7
#define BRAINPAD_TICK_LED22 PA_6
#define BRAINPAD_TICK_LED23 PA_1
#define BRAINPAD_TICK_LED24 PA_0	
#define BRAINPAD_TICK_LED25 PH_0
	
namespace codal {
    class BrainPadDisplay {
        _mbed::Pin* pixel[25];                
        void flush();

    public:
        BrainPadDisplay();

        void writeScreenBuffer(uint8_t* buffer);
    };
}	
	
#endif
#endif
