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

void setup()
{
	Serial.begin(9600);

	// Digital pins
	pinMode(underThresholdLedPin, OUTPUT);
	pinMode(overThresholdLedPin, OUTPUT);
	
	// Analog pins
	pinMode(userInputPotPin, INPUT);
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
        digitalWrite(underThresholdLedPin, HIGH);
    }
    else
    {
        digitalWrite(underThresholdLedPin, HIGH);
        digitalWrite(underThresholdLedPin, LOW);
    }

    Serial.println(messageText);
    
    delay(100);
}
