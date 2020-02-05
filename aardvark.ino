// Digital pins
const byte ledPin = 7;

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
	pinMode(ledPin, OUTPUT);
	
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
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }

    Serial.println(messageText);
    
    delay(100);
}
