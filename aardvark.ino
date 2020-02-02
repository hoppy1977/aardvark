// Digital pins
const byte ledPin = 7;

// Analog pins
const byte userInputPotPin = A2;

// Potentiometer constants
const int userInputPotMinimumValue = 148;
const int userInputPotMaximumValue = 255;
const int userInputMinimumValue = 1;
const int userInputMaximumValue = 100;

// Variables
const int threshold = 90;

void setup()
{
	Serial.begin(9600);

	// Digital pins
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);

	// Analog pins
	pinMode(userInputPotPin, INPUT);
}

void loop()
{
	int rawValue = analogRead(userInputPotPin);
    int mappedValue = map(rawValue, userInputPotMinimumValue, userInputPotMaximumValue, userInputMinimumValue, userInputMaximumValue);

    String messageText;
	messageText += "Raw:    " + String(rawValue) + " ";
    messageText += "Mapped: " + String(mappedValue);

    if(mappedValue > threshold)
    {
        messageText += " Threshold exceeded!";
        digitalWrite(ledPin, HIGH);

        //Serial.println(messageText);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }

    Serial.println(messageText);
    
    delay(100);
}
