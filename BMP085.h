#ifndef BMP085_h
#define BMP085_h

#include <Arduino.h>
#include <I2C.h>
//#include <digitalWriteFast.h>

#define BMP085_ADDRESS 0x77
#define OSS 0x00
#define READY_PIN 6
#define POLL_RATE 1000

class BMP085{
	public:
		void PollPressure(void);
		void init(void);
		long Pressure(unsigned long up);
		short Temperature(unsigned int ut);
		unsigned long ReadUP(void);
		unsigned int ReadUT(void);
		void StartUP(void);
		void StartUT(void);
		struct{
			long pressure;
			short temperature;
		}vars;
		boolean newData;
	private:

		int pressureState;
		int ac1;
		int ac2;
		int ac3;
		unsigned int ac4;
		unsigned int ac5;
		unsigned int ac6;
		int b1;
		int b2;
		int mb;
		int mc;
		int md;
		unsigned char msb;
		unsigned char lsb;
		unsigned char xlsb;
		long x1;
		long x2;
		long x3;
		long b3;
		long b5;
		long b6;
		long p;
		unsigned long b4;
		unsigned long b7;
		unsigned int ut;
		unsigned long up;
		unsigned long timer;
};

#endif
