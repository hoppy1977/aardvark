// Digital pins
const byte ledPin = 7;

// Analog pins
const byte userInputPotPin = A2;

// Potentiometer constants
const int userInputPotMinimumValue = 1023;
const int userInputPotMaximumValue = 0;
const int userInputMinimumValue = 1;
const int userInputMaximumValue = 72;

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
	Serial.println("Beginning loop...");

	int rawValue = analogRead(userInputPotPin);
    int mappedValue = map(rawValue, userInputPotMinimumValue, userInputPotMaximumValue, userInputMinimumValue, userInputMaximumValue);

	Serial.print(
        "Raw:    " + String(rawValue) + " "
        "Mapped: " + String(mappedValue)
    );

    if(mappedValue > threshold)
    {
        Serial.print("Threshold exceeded!");
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }

    Serial.println();
    
    Serial.println();
}
