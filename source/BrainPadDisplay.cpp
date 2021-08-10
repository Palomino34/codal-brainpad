#include "BrainPadDisplay.h"

using namespace codal;

#ifdef BRAINPADPULSE

const int BrainPadDisplay::vramSize;

BrainPadDisplay::BrainPadDisplay(codal::I2C& _i2c, uint16_t address) : i2c(_i2c) {
    this->address = address;
	
	_mbed::Pin resetPin(0, RESET_PIN_NAME, PIN_CAPABILITY_DIGITAL);
	
	resetPin.setDigitalValue(0);
	wait_ms(50);
	resetPin.setDigitalValue(1);
	wait_ms(50);

    writeCommand(0xae);// turn off oled panel
    writeCommand(0x00);// set low column address
    writeCommand(0x10);// set high column address
    writeCommand(0x40);// set start line address
    writeCommand(0x81);// set contrast control register
    writeCommand(0xcf);
    writeCommand(0xa1);// set segment re-map 95 to 0
    writeCommand(0xa6);// set normal display
    writeCommand(0xa8);// set multiplex ratio(1 to 64)
    writeCommand(0x3f);// 1/64 duty
    writeCommand(0xd3);// set display offset
    writeCommand(0x00);// not offset
    writeCommand(0xd5);// set display clock divide ratio/oscillator frequency
    writeCommand(0x80);// set divide ratio
    writeCommand(0xd9);// set pre-charge period
    writeCommand(0xf1);
    writeCommand(0xda);// set com pins hardware configuration
    writeCommand(0x12);
    writeCommand(0xdb);//--set vcomh
    writeCommand(0x40);//--set startline 0x0
    writeCommand(0x8d);//--set Charge Pump enable/disable
    writeCommand(0x14);//--set(0x10) disable
    writeCommand(0xaf);//--turn on oled panel
    writeCommand(0xc8);// mirror the screen

    // Mapping
    writeCommand(0x20);
    writeCommand(0x00);
    writeCommand(0x21);
    writeCommand(0);
    writeCommand(128 - 1);
    writeCommand(0x22);
    writeCommand(0);
    writeCommand(7);

    memset(vram, 0, BrainPadDisplay::vramSize);

    vram[0] = 0x40;

    flush();
}

void BrainPadDisplay::writeCommand(int command) {
    data[0] = 0;
    data[1] = static_cast<uint8_t>(command);

    i2c.write(address, data, 2, false);
}

void BrainPadDisplay::flush() {
    i2c.write(address, vram, BrainPadDisplay::vramSize, false);
}

void BrainPadDisplay::writeScreenBuffer(uint8_t* buffer) {
    for (int x = 0; x < 128; x++) {
        for (int y = 0; y < 64; y++) {
            int offset = (y / 8) + (x * 8);
            int mask = 1 << (y % 8);
            int index = (x + (y / 8) * 128) + 1;

            if (buffer[offset] & mask)
                vram[index] |= static_cast<uint8_t>(1 << (y % 8));
            else
                vram[index] &= static_cast<uint8_t>(~(1 << (y % 8)));
        }
    }

    flush();
}
#else
BrainPadDisplay::BrainPadDisplay()  {
	
	const uint32_t pinArray[] = { BRAINPAD_TICK_LED1 , BRAINPAD_TICK_LED2 , BRAINPAD_TICK_LED3 , BRAINPAD_TICK_LED4 , BRAINPAD_TICK_LED5 ,
								  BRAINPAD_TICK_LED6 , BRAINPAD_TICK_LED7 , BRAINPAD_TICK_LED8 , BRAINPAD_TICK_LED9 , BRAINPAD_TICK_LED10,
								  BRAINPAD_TICK_LED11, BRAINPAD_TICK_LED12, BRAINPAD_TICK_LED13, BRAINPAD_TICK_LED14, BRAINPAD_TICK_LED15, 
								  BRAINPAD_TICK_LED16, BRAINPAD_TICK_LED17, BRAINPAD_TICK_LED18, BRAINPAD_TICK_LED19, BRAINPAD_TICK_LED20,
								  BRAINPAD_TICK_LED21, BRAINPAD_TICK_LED22, BRAINPAD_TICK_LED23, BRAINPAD_TICK_LED24, BRAINPAD_TICK_LED25};
	for (int i = 0; i < 25; i++) {						
		pixel[i] = new _mbed::Pin(0, pinArray[i], PIN_CAPABILITY_DIGITAL);
	}
}

void BrainPadDisplay::flush() {
    
}

void BrainPadDisplay::writeScreenBuffer(uint8_t* buffer) {    
	for (int i = 0; i < 25; i++) {
		if (buffer[i])
			pixel[i]->setDigitalValue(1);
		else 
			pixel[i]->setDigitalValue(0);
	}
    
}

#endif	