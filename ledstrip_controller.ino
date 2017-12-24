//#include <GDBStub.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define NOP __asm__ __volatile__ ("nop\n\t")
#define NOP2 __asm__ __volatile__ ("nop\n\t" "nop\n\t")
#define out 0
#define id 0

int numLeds = 120;

//AP credidentials
const char *ssid = "";
const char *password = "";

HTTPClient client;

void setup()
{
	Serial.begin(115200);
	delay(100);
	pinMode(out, OUTPUT);
	
	// Connect to AP
	WiFi.begin(ssid, password);

	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	//http://{ip}/controller?id=
	String url = "";
	url = String(url + id);
	
	client.begin(url);
	int httpCode = client.GET();
	//Serial.println(httpCode);

	if (httpCode > 0) {
		if (httpCode == HTTP_CODE_OK) {
			String payload = client.getString();
			//Serial.println("payload: ");
			//Serial.print(payload);;
			handleResponse(payload);
		}
	}
	
	
}

void stretchPattern(uint8_t* pattern, int patternLenght) {
	patternLenght *= 3;
	int it = 0;

	for (size_t i = patternLenght; i < numLeds * 3; i++)
	{
		pattern[i] = pattern[it];
		if (it == patternLenght - 1)
		{
			it = 0;
		}
		else {
			it++;
		}
	}
}

void append(char* s, char c)
{
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
}

void sendBytes(const int numBytes, uint8_t* output)
{
#define WAIT0H NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
#define WAIT1H NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;
#define WAIT0L NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP2;NOP2;
#define WAIT1L NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;NOP2;

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
		}
	}

	interrupts();
}

void handleResponse(String response) {
	String l;
	int i = 0;
	while (response[i] != '\n')
	{
		l += response[i];
		i++;
	}

	
	int length = l.toInt();
	response = response.substring(l.length());

	uint8_t *colors;
	colors = (uint8_t *) malloc(numLeds * 3);
	//if (colors != 0) {
	//	delete[] colors;
	//}
	//colors = new uint8_t[numLeds * 3];

	int charCount = 1;
	for (size_t i = 0; i < length*3; i+=3)
	{
		for (size_t j = 0; j < 3; j++)
		{			
			char color[3];
			for (size_t i = 0; i < 3; i++)
			{
				color[i] = '\0';
			}
			//int pos = 0;

			char c = response[charCount];
			while (c != ';' && c != '\n')
			{
				append(color, c);
				//pos++;
				charCount++;
				c = response[charCount];
			}
			charCount++;
			uint8_t col = (uint8_t)atoi(color);
			colors[i+j] = col;			
		}
	}	

	stretchPattern(colors, length);
	//Serial.println("pattern:");
	//for (size_t i = 0; i < numLeds * 3; i++)
	//{
	//	Serial.println(colors[i]);
	//}
	sendBytes(numLeds * 3 ,colors);
	free(colors);
}

void loop()
{
	client.setTimeout(-1);
	//http://{ip}/poll?id=
	String url = "";
	url = String(url + id);
	client.begin(url);
	//Serial.println("listening");
	int httpCode = client.GET();
	//Serial.println(httpCode);

	if (httpCode > 0) {
		if (httpCode == HTTP_CODE_OK) {
			String payload = client.getString();
			//Serial.println("payload: ");
			//Serial.print(payload);;
			handleResponse(payload);			
		}
	}
	//else
	//{
	//	Serial.println(client.errorToString(httpCode));		
	//}
}
