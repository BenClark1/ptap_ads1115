//Ben Clark

//Pressure Tap Board Library
//Function definitions
#include "ptap_ads1115.h"

void writeReg(byte deviceAddress, byte pointerRegister, byte val1, byte val2) {
//bit 1 and 0 in the pointerRegister determine what register is being accessed,
//so 00000000 is for the conversion register and 00000001 is for the configuration register.
  Wire.beginTransmission(deviceAddress); 
  Wire.write(pointerRegister);  
  Wire.write(val1); //queue bytes for trasmission
  Wire.write(val2);
  Wire.endTransmission(); 
}

uint16_t readReg(byte deviceAddress, byte pointerRegister) { 
  Wire.beginTransmission(deviceAddress);
  Wire.write(pointerRegister);
  Wire.endTransmission();
  
  //read the register for two bytes
  Wire.requestFrom(deviceAddress, 2);
  uint16_t val = Wire.read();
  val = (val << 8) | Wire.read();
  return val; 
}

void prime_ads1115(byte deviceAddress, int channel) {
  //the channel argument takes 0, 1, or 2 for AIN0, AIN1, and AIN2
  
  byte init_config = BLANK_INIT_CONFIG | (channel << 4);
  int next_channel = (1 << channel) & 0b011;
  byte next_ch_config = BLANK_CHANNEL_CONFIG | (next_channel << 4);
  
  if (deviceAddress == ADC_A_ADDR) {
    //power off A and set the channel 
    writeReg(ADC_A_ADDR, POINT_REG_CONFIG, init_config, INIT_CONFIG2); 
    //tell it to start a conversion and set the configuration for the NEXT channel
    writeReg(ADC_A_ADDR, POINT_REG_CONFIG, next_ch_config, AINX_CONFIG2);   
  }
  else if (deviceAddress == ADC_B_ADDR) {
    writeReg(ADC_B_ADDR, POINT_REG_CONFIG, init_config, INIT_CONFIG2);
    writeReg(ADC_B_ADDR, POINT_REG_CONFIG, next_ch_config, AINX_CONFIG2);
  }
}
 
