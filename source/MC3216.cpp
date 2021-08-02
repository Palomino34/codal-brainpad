#include "MC3216.h"
#include "CodalConfig.h"
#include "ErrorNo.h"
#include "CodalCompat.h"
#include "CodalFiber.h"
#include "platform/mbed_wait_api.h"

#include "BrainPad.h"
#include "Timer.h"

using namespace codal;

// static const KeyValueTableEntry rangeRegisterData[] = {
    // { 2, 0x00 },
    // { 4, 0x01 },
    // { 8, 0x02 },
// };

// static const KeyValueTableEntry rangeDivisorData[] = {
    // { 2, 256 },
    // { 4, 128 },
    // { 8, 64 },
// };

// CREATE_KEY_VALUE_TABLE(rangeRegister, rangeRegisterData);
// CREATE_KEY_VALUE_TABLE(rangeDivisor, rangeDivisorData);

MC3216::MC3216(codal::I2C& _i2c, Pin& _int1, CoordinateSpace& coordinateSpace, uint16_t address, uint16_t id) : Accelerometer(coordinateSpace, id), i2c(_i2c), int1(_int1) {

    this->address = address;
	this->mylocker = false;
	this->current_ms = system_timer_current_time();

    configure();
}

MC3216::~MC3216() {

}

void MC3216::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t data[2];

    data[0] = reg;
    data[1] = val;

    if (&i2c == nullptr)
		return;
	
	i2c.write(address, data, 2, 0);
}

int MC3216::updateSample() {
    if (this->mylocker) {
		// while (this->mylocker) {
			// wait_ms(1);
		// }

		return DEVICE_OK;
	}
	
		
	if (&i2c == nullptr)
		return DEVICE_OK;

	
	
	
   
	
    

    if (system_timer_current_time() - 100 > this->current_ms) {
		
		uint8_t dataX[1];
		uint8_t dataY[1];
		uint8_t dataZ[1];
		 
        i2c.readRegister(address, MC3216_XOut, dataX, 1);		
		int32_t x = dataX[0];
		
		i2c.readRegister(address, MC3216_YOut, dataY, 1);		
		int32_t y = dataY[0];
		
		i2c.readRegister(address, MC3216_ZOut, dataZ, 1);		
		int32_t z = dataZ[0];

        // int32_t x = (data[0] << 2) | (data[1] >> 6);
        // int32_t y = (data[2] << 2) | (data[3] >> 6);
        // int32_t z = (data[4] << 2) | (data[5] >> 6);

        // if (x >= 512) x -= 1024;
        // if (y >= 512) y -= 1024;
        // if (z >= 512) z -= 1024;

        // x *= 1024;
        // x /= divisor;

        // y *= 1024;
        // y /= divisor;

        // z *= 1024;
        // z /= divisor;
		this->mylocker = true;
        update({ -y, x, -z }); //To transform to ENU
				
		this->current_ms = system_timer_current_time();
		this->mylocker = false;
    }

    return DEVICE_OK;
}

int MC3216::configure() {
    // writeRegister(CTRL_REG1, CTRL_REG1_SLEEP);

    // writeRegister(XYZ_DATA_CFG, rangeRegister.get(getRange()));
    // writeRegister(CTRL_REG4, CTRL_REG4_INT_DATA);
    // writeRegister(CTRL_REG5, CTRL_REG5_SET_INT_PIN);
    // writeRegister(CTRL_REG1, CTRL_REG1_ACTIVE);
	
	uint8_t data[1];
	
	writeRegister(MC3216_Outcfg, 2);
	writeRegister(MC3216_Mode, 1);
	
	i2c.readRegister(address, MC3216_Opstat, data, 1);
	
	if ((data[0] & 0x1) != 0x1 )
		while(1);

    return DEVICE_OK;
}

int MC3216::requestUpdate() {
    updateSample();

    return DEVICE_OK;
}

void MC3216::idleCallback() {
    updateSample();
}
