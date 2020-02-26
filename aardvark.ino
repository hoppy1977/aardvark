#include <Ethernet.h>
#include <HttpClient.h>

#include "src/RTClib/RTClib.h"
#include "src/Sha/sha1.h"

// Digital pins
const byte underThresholdLedPin = 7;
const byte overThresholdLedPin = 8;

// Analog pins
const byte userInputPotPin = A0;

// Potentiometer constants
const int userInputPotMinimumValue = 0;
const int userInputPotMaximumValue = 1023;
const int userInputMinimumValue = 1;
const int userInputMaximumValue = 100;

// Variables
const int threshold = 90;

// *****************************************************************************
unsigned long currentTime = 1581834590; // TODO:

// Amazon AWS configuration
#define     AWS_ACCESS_KEY         "TODO:"						// Put your AWS access key here.
#define     AWS_SECRET_ACCESS_KEY  "TODO:"						// Put your AWS secret access key here.
#define     AWS_REGION             "ap-southeast-2"													// The region where your SNS topic lives.
																									// See the table at: http://docs.aws.amazon.com/general/latest/gr/rande.html#sns_region
#define     AWS_HOST               "google.com"								// The host URL for the region where your SNS topic lives.
																									// See the table at: http://docs.aws.amazon.com/general/latest/gr/rande.html#sns_region
#define     SNS_TOPIC_ARN          "arn%3Aaws%3Asns%3Aap-southeast-2%3A244360212732%3Aaardvark-demo-MySNSTopic-EH981W003O13"	// Amazon resource name (ARN) for the SNS topic to receive notifications.
																																// Note: This ARN _MUST_ be URL encoded!  See http://meyerweb.com/eric/tools/dencoder/ for an example URL encoder tool.
// SHA1 hash details
#define     SHA1_HASH_LENGTH       20

// MAC address for your Ethernet shield
byte mac[] = { 0x96, 0x97, 0xFC, 0x1D, 0xF4, 0xA2 }; // Randomly selected

EthernetClient client;
// *****************************************************************************

void setup()
{
	Serial.begin(9600);

	// Digital pins
	pinMode(underThresholdLedPin, OUTPUT);
	pinMode(overThresholdLedPin, OUTPUT);
	
	// Analog pins
	pinMode(userInputPotPin, INPUT);
	// // Check for Ethernet hardware present
	// if (Ethernet.hardwareStatus() == EthernetNoHardware) {
	// 	Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
	// 	while (true) {
	// 		delay(1); // do nothing, no point running without Ethernet hardware
	// 	}
	// }

	// while (Ethernet.linkStatus() == LinkOFF) {
	// 	Serial.println("Ethernet cable is not connected, trying again...");
	// 	delay(5000);
	// }

	while (Ethernet.begin(mac) != 1) {
		Serial.println("Error getting IP address via DHCP, trying again...");
		delay(5000);
	}

	Serial.print("DHCP assigned IP ");
	Serial.println(Ethernet.localIP());
}

void loop()
{
	int rawValue = analogRead(userInputPotPin);
    int mappedValue = map(rawValue, userInputPotMinimumValue, userInputPotMaximumValue, userInputMinimumValue, userInputMaximumValue);

    String messageText;
	messageText += "Raw:   " + String(rawValue) + "\t";
    messageText += "Mapped:" + String(mappedValue);

    if(mappedValue > threshold)
    {
        messageText += "\tThreshold exceeded!";
        digitalWrite(underThresholdLedPin, LOW);
        digitalWrite(overThresholdLedPin, HIGH);

        snsPublish(SNS_TOPIC_ARN, "Arduino%20rules%21");
		delay(10000);
    }
    else
    {
        digitalWrite(underThresholdLedPin, HIGH);
        digitalWrite(overThresholdLedPin, LOW);
    }

    Serial.println(messageText);
    
    delay(100);
}

// Publish a message to an SNS topic.
// Note, both the topic and message strings _MUST_ be URL encoded before calling this function!
void snsPublish(const char* topic, const char* message)
{
	// Make request
	int connectionResult = client.connect(AWS_HOST, 80);
	if (connectionResult == 1)
	{
		Serial.println("Connected to host...");

		String text;
		text += F("GET ");
		text += AWS_HOST;
		text += F(" HTTP/1.1\r\nHost: ");
		text += AWS_HOST;
		text += F("\r\nConnection: close\r\n");
		text += F("\r\nContent-Length: 0\r\n\r\n");

		Serial.print(text);
		client.print(text);

		// Print response to Serial Monitor
		Serial.println();
		Serial.println("Response:");
		while (client.connected())
		{
			while (client.available())
			{
				char c = client.read();
				Serial.print(c);
			}
		}

		Serial.println("<End>");

		// Close the connection after the server closes its end
		client.stop();
	}
	else
	{
		Serial.println("Error: Failed to connect to AWS! (" + String(connectionResult) + ")");
	}

	Serial.println("Finished publishing message");

	Serial.println();
	Serial.println();
}

// Fill a 24 character buffer with the date in URL-encoded ISO8601 format, like '2013-01-01T01%3A01%3A01Z'.  
// Buffer MUST be at least 24 characters long!
void dateTime8601UrlEncoded(int year, byte month, byte day, byte hour, byte minute, byte seconds, char* buffer)
{
	ultoa(year, buffer, 10);
	buffer[4] = '-';
	btoa2Padded(month, buffer + 5, 10);
	buffer[7] = '-';
	btoa2Padded(day, buffer + 8, 10);
	buffer[10] = 'T';
	btoa2Padded(hour, buffer + 11, 10);
	buffer[13] = '%';
	buffer[14] = '3';
	buffer[15] = 'A';
	btoa2Padded(minute, buffer + 16, 10);
	buffer[18] = '%';
	buffer[19] = '3';
	buffer[20] = 'A';
	btoa2Padded(seconds, buffer + 21, 10);
	buffer[23] = 'Z';
}

// Print a value from 0-99 to a 2 character 0 padded character buffer.
// Buffer MUST be at least 2 characters long!
void btoa2Padded(uint8_t value, char* buffer, int base)
{
	if (value < base) {
		*buffer = '0';
		ultoa(value, buffer + 1, base);
	}
	else {
		ultoa(value, buffer, base);
	}
}
