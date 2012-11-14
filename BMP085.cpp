#include <BMP085.h>
#include <digitalWriteFast.h>

void  BMP085::GetAltitude(long *press,long *pressInit, float *alti){
	pressureRatio = (float) *press / (float) *pressInit;
	*alti = (1.0f - pow(pressureRatio, 0.190295f)) * 44330.0f;
}

void BMP085::PollPressureFast(void){
	if (millis() - timer > POLL_RATE){
		switch (pressureState){
			 case 0:
				 StartUP();
				 pressureState = 1;
			 break;
			 case 1://wait for ready signal
			 if (digitalReadFast(READY_PIN) == 1){
				 pressureState = 2;
				 up = ReadUP();
			 }
			 break;
			 case 2:
				vars.pressure = Pressure(up);
				pressureState = 0;
				newData = true;
				timer = millis();
			 break;

		}
	}
}

void BMP085::PollPressure(void){
	if (millis() - timer > POLL_RATE){
		switch (pressureState){
			 case 0://read ut
				 StartUT();
				 pressureState = 1;
			 break;
			 case 1://wait for ready signal
			 if (digitalReadFast(READY_PIN) == 1){
				 pressureState = 2;
				 ut = ReadUT();
			 }
			 break;
			 case 2://read up
				StartUP();
				pressureState = 3;
			 break;
			 case 3://wait for ready signal
			 if (digitalReadFast(READY_PIN) == 1){
				 pressureState = 4;
				 up = ReadUP();
			 }
			 break;
			 case 4://
				vars.temperature = Temperature(ut);
				vars.pressure = Pressure(up);
				pressureState = 0;
				newData = true;
				timer = millis();
			 break;
		}
	}
}

long BMP085::Pressure(unsigned long up){


  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  return p;
}

short BMP085::Temperature(unsigned int ut){

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);
}

void BMP085::StartUT(void){
	I2c.write(BMP085_ADDRESS,0xF4,0x2E);
}

unsigned int BMP085::ReadUT(void){



  I2c.read(BMP085_ADDRESS,0xF6,2);
  msb = I2c.receive();
  lsb = I2c.receive();

  return ((msb << 8) | lsb);
}

void BMP085::StartUP(void){
  I2c.write(BMP085_ADDRESS,0xF4,(0x34 + (OSS<<6)));
}

unsigned long BMP085::ReadUP(void){

  I2c.read(BMP085_ADDRESS,0xF6,3);
  msb = I2c.receive();
  lsb = I2c.receive();
  xlsb = I2c.receive();

  return ((((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS));
}

void BMP085::init(void){
  pinModeFast(READY_PIN,INPUT);
  pressureState = 0;
  timer = millis();
  newData = false;
  I2c.read(BMP085_ADDRESS,0xAA,22);
  msb = I2c.receive();
  lsb = I2c.receive();
  ac1 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  ac2 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  ac3 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  ac4 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  ac5 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  ac6 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  b1 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  b2 = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  mb = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  mc = (msb << 8) | lsb;

  msb = I2c.receive();
  lsb = I2c.receive();
  md = (msb << 8) | lsb;
  while (newData == false){
	  PollPressure();
  }
  newData = false;

}