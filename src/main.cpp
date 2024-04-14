#include <Arduino.h>
#include <ezButton.h>

// Generic constants
const long mspm = 60000; // Milliseconds per minute

// Set up valve constants
const int   v1_pin    = 10;         // Valve 1 pin number
const float v1_rpd    = float(1)/float(2);        // Valve 1 runs per day
const long  v1_ms     = 10*mspm;    // Number of milliseconds to run valve 1
bool        v1_actv   = 0;          // Boolean to tell whether valve 1 is currently active
bool        istimer1  = 0;          // Boolean to tell whether a timer is currently active or if the program was just initialized

const int   v2_pin    = 11;         // Valve 2 pin number
const float v2_rpd    = 1;          // Valve 2 runs per day
const long  v2_ms     = 5*mspm;     // Number of milliseconds to run valve 2
bool        v2_actv   = 0;          // Boolean to tell whether valve 1 is currently active
bool        istimer2  = 0;          // Boolean to tell whether a timer is currently active or if the program was just initialized


const int    v3_pin    = 12;         // Valve 3 pin number
const float  v3_rpd    = float(1)/float(3);        // Valve 3 runs per day
const long   v3_ms     = 30*mspm;    // Number of milliseconds to run valve 3
bool         v3_actv   = 0;          // Boolean to tell whether valve 1 is currently active
bool         istimer3  = 0;          // Boolean to tell whether a timer is currently active or if the program was just initialized


// Timing calculations
const long mspd = 24*60*mspm;     // Milliseconds per day
const long msv1 = mspd/v1_rpd;    // Number of milliseconds to wait until next valve 1 run
const long msv2 = mspd/v2_rpd;    // Number of milliseconds to wait until next valve 2 run
const long msv3 = mspd/v3_rpd;    // Number of milliseconds to wait until next valve 3 run

// Set up switches
const int v1_switchPin  = 5; // Set pin 5 as the pin for valve 1 switch
const int v2_switchPin  = 6; // Set pin 6 as the pin for valve 2 switch
const int v3_switchPin  = 7; // Set pin 7 as the pin for valve 3 switch

ezButton S1(v1_switchPin);
ezButton S2(v2_switchPin);
ezButton S3(v3_switchPin);
int S1state = 0;
int S2state = 0;
int S3state = 0;

void setup() {

  // Currently wired to main irrigation to garden bed an flowers
  pinMode(v1_pin,OUTPUT);

  // Currently wired to trees, texas ranger, and plumbagos
  pinMode(v2_pin,OUTPUT);

  // Currently not wired
  pinMode(v3_pin,OUTPUT);

  // Start Serial Connection
  Serial.begin(9600);

  // Set Button debounce timing
  S1.setDebounceTime(5); // set debounce time to 50 milliseconds
  S2.setDebounceTime(5); // set debounce time to 50 milliseconds
  S3.setDebounceTime(5); // set debounce time to 50 milliseconds

}

// To initialize program with valves ON, set counters to delay times
// long v1_count  = msv1;          // Inialize valve 1 counter
// long v2_count  = msv2;          // Inialize valve 2 counter
// long v3_count  = msv3;          // Inialize valve 3 counter

// To initialize program with valves OFF, set counters to 1
long v1_count  = 2;          // Inialize valve 1 counter
long v2_count  = 2;          // Inialize valve 2 counter
long v3_count  = 2;          // Inialize valve 3 counter

void loop() {

  S1.loop();
  S2.loop();
  S3.loop();

  S1state = S1.getState();
  S2state = S2.getState();
  S3state = S3.getState();

  ////////////////
  // VALVE 1 logic
  ////////////////
  if (v1_count>=msv1 && !v2_actv && !v3_actv) {
    v1_actv = 1; // Set valve 1 as active
    digitalWrite(v1_pin, HIGH); // Turn on valve 1
    v1_count = 1; // Reset valve 1 counter
    istimer1 = 1; // Set timer 1 boolean to on

    Serial.print("Valve 1 timer is active!\n");
  }

  if (v1_actv && v1_count==v1_ms) {
    if (digitalRead(v1_switchPin)) {
      digitalWrite(v1_pin, LOW); // Turn off valve 1, Wont turn off if valve's swtich is active, but will still reset timer boolean
      v1_actv = 0; // Set valve 1 as inactive
    }
    istimer1 = 0; // Set timer 1 boolean to off

    Serial.print("Valve 1 timer is up!\n");
  }

  // Option to manually activate valve 1 using a switch without affecting any of the counts
  if (!S1state && !v1_actv) {
    digitalWrite(v1_pin, HIGH); // Turn on valve 1
    v1_actv = 1; // Set valve 1 as active

    Serial.print("Valve 1 was turned on by the switch!\n");
  }
  if (S1state && v1_actv && !istimer1) { // Wont turn off if valve's timer is currently active 
    digitalWrite(v1_pin, LOW); // Turn off valve 1
    v1_actv = 0; // Set valve 1 as inactive

    Serial.print("Valve 1 was turned off by the switch!\n");
  }

  ////////////////
  // VALVE 2 logic
  ////////////////
  if (v2_count>=msv2 && !v1_actv && !v3_actv) {
    v2_actv = 1; // Set valve 2 as active
    digitalWrite(v2_pin, HIGH); // Turn on valve 2
    v2_count = 1; // Reset valve 2 counter
    istimer2 = 1; // Set timer 2 boolean to on

    Serial.print("Valve 2 timer is active!\n");
  }

  if (v2_actv && v2_count==v2_ms) { // Wont turn off if valve's swtich is active
    if (digitalRead(v2_switchPin)) {
      digitalWrite(v2_pin, LOW); // Turn off valve 2
      v2_actv = 0; // Set valve 2 as inactive
    }
    istimer2 = 0; // Set timer 2 boolean to off

    Serial.print("Valve 2 timer is up!\n");
  }

    // Option to manually activate valve 2 using a switch without affecting any of the counts  
  if (!S2state && !v2_actv) {
    digitalWrite(v2_pin, HIGH); // Turn on valve 2
    v2_actv = 1; // Set valve 2 as active

    Serial.print("Valve 2 was turned on by the switch!\n");
  }
  if (S2state && v2_actv && !istimer2) { // Wont turn off if valve's timer is currently active
    digitalWrite(v2_pin, LOW); // Turn off valve 2
    v2_actv = 0; // Set valve 2 as inactive

    Serial.print("Valve 2 was turned off by the switch!\n");
  }

    // Option to manually activate valve 3 using a switch without affecting any of the counts  
  if (!S3state && !v3_actv) {
    digitalWrite(v3_pin, HIGH); // Turn on valve 3
    v3_actv = 1; // Set valve 3 as active

    Serial.print("Valve 3 was turned on by the switch!\n");
  }
  if (S3state && v3_actv && !istimer3) { // Wont turn off if valve's timer is currently active
    digitalWrite(v3_pin, LOW); // Turn off valve 3
    v3_actv = 0; // Set valve 3 as inactive

    Serial.print("Valve 3 was turned off by the switch!\n");
  }

  // Increment valve counters
  v1_count += 1;
  v2_count += 1;
  v3_count += 1;

  // Serial.print("v1_count = ");
  // Serial.print(v1_count);
  // Serial.print("\n");

  delay(1);

  // int v1 = S1.getState();
  // Serial.print(v1);
  // Serial.print("\n");
}
