#define NOP __asm__ __volatile__ ("nop\n\t")
#define NOP2 __asm__ __volatile__ ("nop\n\t" "nop\n\t")
#define out 0
#define numLeds 120

void sendBytes(int numBytes, uint8_t output[]);

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t bytes[numLeds * 3];
	for (int j = 0; j < (numLeds * 3); j += 3)
	{
		bytes[j] = g;
		bytes[j + 1] = r;
		bytes[j + 2] = b;
	}

	sendBytes(sizeof(bytes) / sizeof(bytes[0]), bytes);
}

void setup()
{
	Serial.begin(115200);
	delay(100);
	pinMode(out, OUTPUT);
	setColor(0, 0, 0);
	//setColor(255, 242, 112);
	//uint8_t bytes[9] = { 0,100,0,139, 214, 10 ,214, 10, 95};
	//sendBytes(sizeof(bytes)/sizeof(bytes[0]), bytes);
}

void loop()
{
	//Serial.println("loop");
	
	
//	uint8_t bytes[numLeds * 3];
//
//	for (int i = 1; i <= 255; i++) {
//		for (int j = 0; j < (numLeds * 3); j += 3)
//		{
//			bytes[j] = 128 + sin((i * 3 + 1)*1.3) * 128;
//			bytes[j + 1] = 128 + sin((i * 3 + 0)*1.3) * 128;
//			bytes[j + 2] = 128 + sin((i * 3 + 2)*1.3) * 128;
//
//			
//
//			
//			//Serial.println(i);
//		}
//		Serial.print(128 + sin((i * 3 + 0)*1.3) * 128);
//		Serial.print(128 + sin((i * 3 + 1)*1.3) * 128);
//		Serial.println(128 + sin((i * 3 + 2)*1.3) * 128);
//		sendBytes(sizeof(bytes) / sizeof(bytes[0]), bytes);
//		delay(10);
//	}
	
	//pinMode(out, OUTPUT);
//	uint8_t bytes[6] = { 0,0,0,50,50,50};
//	sendBytes(sizeof(bytes) / sizeof(bytes[0]), bytes);
//
//	delay(10);
}



void sendBytes(const int numBytes, uint8_t output[])
{
	#define WAIT0H NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
	#define WAIT1H NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;
	#define WAIT0L NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP2;NOP2;
	#define WAIT1L NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;

	//Serial.print(numBytes);
	//Serial.println();

	//std::vector<bool> bits;

//	for(int i= 0; i < numBytes; i++)
//	{
//		output[i] = (output[i] / 255) * 100;
//	}

	noInterrupts();

	for (int i = 0; i < numBytes; i++)
	{
		for (int j = 7; j > -1; j--)
		{

			bool val = (output[i] & (1 << j)) != 0;

			if (val) {
				GPOS = (1 << out);
				WAIT1H;
				GPOC = (1 << out);
				WAIT1L;
			}
			else {
				GPOS = (1 << out);
				WAIT0H;
				GPOC = (1 << out);
				WAIT0L;
			}


//			Serial.print((output[i] & (1 << j)) != 0);
//			bits.push_back(((output[i] & (1 << j)) != 0));
			//bits[(i * 8) + j] = bitRead(output[i], j);
		}
		//Serial.println();
	}

	interrupts();

//	Serial.println();
//	for(int i = 0; i < numBytes * 8; i++)
//	{
//		Serial.print(bits[i]);
//	}

	
		
	noInterrupts();
}
