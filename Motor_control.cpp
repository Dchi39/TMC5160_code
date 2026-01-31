#include <Arduino.h>
#include <TMCStepper.h>
#include <SPI.h>

#define R_SENSE 0.075f

// ===== Pin Definitions =====
#define PIN_MOSI 11
#define PIN_MISO 13
#define PIN_SCK  12
#define PIN_CS   10
#define PIN_ENN  6

// ===== Driver Object =====
TMC5160Stepper driver(PIN_CS, R_SENSE);

// ===== Function Prototypes =====
void Motor_Init();
void Motor_Move_Test();

// ==========================
void setup() {
  Serial.begin(115200);
  delay(500);

  Motor_Init();

  Serial.println("Motor initialized");
  delay(1000);
}

void loop() {
  Motor_Move_Test();
}

// ==========================
// Motor initialization
// ==========================
void Motor_Init() {

  // Enable driver (ENN is active LOW)
  pinMode(PIN_ENN, OUTPUT);
  digitalWrite(PIN_ENN, LOW);

  // Start SPI
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

  // --- Driver basic setup ---
  driver.begin();
  driver.toff(5);             // Enable chopper
  driver.blank_time(24);      // Safe blank time

  // --- Motor current & resolution ---
  driver.rms_current(1200);   // 1.2A RMS
  driver.microsteps(32);

  // --- Quiet running ---
  driver.en_pwm_mode(true);   // StealthChop
  driver.pwm_autoscale(true);

  // --- Clear driver errors ---
  driver.GSTAT(0x7);

  // --- Internal ramp generator ---
  driver.RAMPMODE(0);         // Position mode
  driver.XACTUAL(0);          // Reset position

  driver.AMAX(2000);          // Acceleration
  driver.DMAX(2000);          // Deceleration
  driver.VMAX(10000);         // Max velocity
}

// ==========================
// Motion test (CW / CCW)
// ==========================
void Motor_Move_Test() {

  Serial.println("CW");
  driver.XTARGET(200000);     // Clockwise
  delay(3000);

  Serial.println("CCW");
  driver.XTARGET(-200000);    // Counter-clockwise
  delay(3000);
}
