/**
 * The Light Dependant Resistor
 * analog input pin.
 *
 * [A0] (Analog-Number) are constants, depending on how many
 * analog input your chip supports. M328P-PU has 6 in total.
 *
 * @var int
 */
const int LDR = A0;

/**
 * Potentiometer input pin number.
 * Being the lowest, this potentiometer
 * controls the first light limit.
 *
 * @var int
 */
const int POTENTIOMETER_0 = A1;

/**
 * Potentiometer input pin number.
 * Being the mid, this potentiometer
 * controls the second light limit.
 *
 * @var int
 */
const int POTENTIOMETER_1 = A2;

/**
 * Potentiometer input pin number.
 * Being the highest, this potentiometer
 * controls the third light limit.
 *
 * @var int
 */
const int POTENTIOMETER_2 = A3;

/**
 * Relay output pin number.
 * Being the lowest, this relay controls the first light
 * (Ranging from Most Light to Least Light [Day - Night]).
 *
 * @var int
 */
const int RELAY_0 = 9;

/**
 * Relay output pin number.
 * Being the mid, this relay controls the second light
 * (Ranging from Most Light to Least Light [Day - Night]).
 *
 * @var int
 */
const int RELAY_1 = 10;

/**
 * Relay output pin number.
 * Being the highest, this relay controls the third light
 * (Ranging from Most Light to Least Light [Day - Night]).
 *
 * @var int
 */
const int RELAY_2 = 11;

/**
 * Relay status.
 *
 * @var string
 */
String RELAY_0_STATUS = "OFF";

/**
 * Relay status.
 *
 * @var string
 */
String RELAY_1_STATUS = "OFF";

/**
 * Relay status.
 *
 * @var string
 */
String RELAY_2_STATUS = "OFF";

/**
 * The main reference to calculate between
 * three levels of light.
 *
 * @var int
 */
int LIGHT_POINT_0 = 600;

/**
 * The main reference to calculate between
 * three levels of light.
 *
 * @var int
 */
int LIGHT_POINT_1 = 300;

/**
 * The main reference to calculate between
 * three levels of light.
 *
 * @var int
 */
int LIGHT_POINT_2 = 150;

/**
 * The light limit margin is a number
 * in which the main light point will oscilate
 *
 * Example:
 * If light point is 512 (half the max) then:
 * 512 + 50 AND 512 - 15 would be an appropriate condition.
 *
 * @var int
 */
int LIGHT_LIMIT_MARGIN = 50;

/**
 * The frequency the system will work at.
 *
 * @var int
 */
const int readFrequency = 100; // Milliseconds.

/**
 * The system status led.
 * turns on as long as the system has energy to do so.
 *
 * @var int
 */
const int SYSTEM_STATUS_LED = 13;

/**
 * LDR read global value.
 *
 * @var int.
 */
int LightIntensity = 0;

/**
 * Stores the system startup check status.
 * Should only trigger ONCE after you turn on
 * your system.
 *
 * @var boolean.
 */
boolean SYSTEM_VERIFIED = false;

/**
 * Reads the LDR (Light Dependant Resistor).
 *
 * @return void
 */
int readLDR()
{
  return LightIntensity = analogRead(LDR);
}

/**
 * Sets the light points from User input.
 *
 * @return void
 */
void setTriggerBoundaries()
{
	LIGHT_POINT_0 = analogRead(POTENTIOMETER_0);
	LIGHT_POINT_1 = analogRead(POTENTIOMETER_1);
	LIGHT_POINT_2 = analogRead(POTENTIOMETER_2);
}

/**
 * Turns ON or OFF the relay and stores
 * the relay status in a global variable.
 *
 * @param  string status ON or OFF
 * @return void
 */
void switchRelay0(String status)
{
	if (status == "ON")
	{
		digitalWrite(RELAY_0, HIGH);
	}
	if (status == "OFF")
	{
		digitalWrite(RELAY_0, LOW);
	}

	RELAY_0_STATUS = status;
}

/**
 * Turns ON or OFF the relay and stores
 * the relay status in a global variable.
 *
 * @param  string status ON or OFF
 * @return void
 */
void switchRelay1(String status)
{
	if (status == "ON")
	{
		digitalWrite(RELAY_1, HIGH);
	}
	if (status == "OFF")
	{
		digitalWrite(RELAY_1, LOW);
	}

	RELAY_1_STATUS = status;
}

/**
 * Turns ON or OFF the relay and stores
 * the relay status in a global variable.
 *
 * @param  string status ON or OFF
 * @return void
 */
void switchRelay2(String status)
{
	if (status == "ON")
	{
		digitalWrite(RELAY_2, HIGH);
	}
	if (status == "OFF")
	{
		digitalWrite(RELAY_2, LOW);
	}

	RELAY_2_STATUS = status;
}


/**
 * Calculates the point where the light
 * must turn off based on the declared
 * margin (constant of 50 units).
 *
 * @param  int light_point
 * @return int
 */
int offLimit(int light_point)
{
	return light_point + LIGHT_LIMIT_MARGIN;
}

/**
 * Calculates the point where the light
 * must turn on based on the declared
 * margin (constant of 50 units).
 *
 * @param  int light_point
 * @return int
 */
int onLimit(int light_point)
{
	return light_point - LIGHT_LIMIT_MARGIN;
}

/**
 * Performs system check.
 * Turns on LEDs and Relays to have feedback if they work.
 *
 * @return void
 */
void systemCheck()
{
	if (SYSTEM_VERIFIED)
	{
		return;
	}

	switchRelay0("ON");
	switchRelay1("ON");
	switchRelay2("ON");

	delay(3000);

	switchRelay0("OFF");
	switchRelay1("OFF");
	switchRelay2("OFF");

	SYSTEM_VERIFIED = true;
}

/**
 * System Setup.
 *
 * @return void
 */
void setup()
{
	digitalWrite(LED_BUILTIN, HIGH);

	pinMode(RELAY_0, OUTPUT);
	pinMode(RELAY_1, OUTPUT);
	pinMode(RELAY_2, OUTPUT);

	Serial.begin(9600);
}

/**
 * Main loop.
 *
 * @return void
 */
void loop()
{
	readLDR();
	setTriggerBoundaries();

	// Turns on or off first light.
	if (LightIntensity > offLimit(LIGHT_POINT_0) && RELAY_0_STATUS == "ON")
	{
		switchRelay0("OFF");

	}
	else if (LightIntensity < onLimit(LIGHT_POINT_0) && RELAY_0_STATUS == "OFF")
	{
		switchRelay0("ON");
	}

	// Turns on or off second light.
	if (LightIntensity > offLimit(LIGHT_POINT_1) && RELAY_1_STATUS == "ON")
	{
		switchRelay1("OFF");
	}
	else if (LightIntensity < onLimit(LIGHT_POINT_1) && RELAY_1_STATUS == "OFF")
	{
		switchRelay1("ON");
	}

	// Turns on or off third light.
	if (LightIntensity > offLimit(LIGHT_POINT_2) && RELAY_2_STATUS == "ON")
	{
		switchRelay2("OFF");
	}
	else if (LightIntensity < onLimit(LIGHT_POINT_2) && RELAY_2_STATUS == "OFF")
	{
		switchRelay2("ON");
	}

	if (LIGHT_POINT_0 < 50)
	{
		switchRelay0("ON");
	}
	else if (LIGHT_POINT_0 > 980)
	{
		switchRelay0("OFF");
	}

	if (LIGHT_POINT_1 < 50)
	{
		switchRelay1("ON");
	}
	else if (LIGHT_POINT_1 > 980)
	{
		switchRelay1("OFF");
	}

	if (LIGHT_POINT_2 < 50)
	{
		switchRelay2("ON");
	}
	else if (LIGHT_POINT_2 > 980)
	{
		switchRelay2("OFF");
	}

	delay(readFrequency);
}
