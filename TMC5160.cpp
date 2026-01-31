#include <Arduino.h>
#include <TMC5160.h>
#include <TMCStepper.h>
#include <SPI.h>
#include <Wire.h>

// Your custom pins
const uint8_t PIN_MOSI = 11;
const uint8_t PIN_MISO = 13;
const uint8_t PIN_SCK = 12;
const uint8_t PIN_CS = 10;
const uint8_t PIN_DRV_ENN = 6;
const uint8_t PIN_STEP = 48; // Only needed if using STEP/DIR mode
const uint8_t PIN_DIR = 45;  // Only needed if using STEP/DIR mode

#define R_SENSE 0.075f

TMC5160_SPI motor = TMC5160_SPI(PIN_CS);
TMC5160Stepper driver = TMC5160Stepper(PIN_CS, R_SENSE);

////////////

void StepperInit()
{

    Serial.println("Initializing Stepper Motor...");
    pinMode(PIN_DRV_ENN, OUTPUT);
    digitalWrite(PIN_DRV_ENN, LOW); // Active low = enabled
    pinMode(PIN_STEP, OUTPUT);
    pinMode(PIN_DIR, OUTPUT);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    TMC5160::PowerStageParameters powerStageParams; // defaults
    TMC5160::MotorParameters motorParams;
    // motorParams.globalScaler = 98;
    //   motorParams.irun = 31;
    // motorParams.ihold = 16;
    motorParams.globalScaler = 140;
    motorParams.irun = 31;
    motorParams.ihold = 16;

    motor.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
    motor.setRampMode(TMC5160::VELOCITY_MODE);
    motor.setMaxSpeed(400);
    motor.setAcceleration(500);
    Serial.println("Stepper Initialized");
    delay(1000);

    // motor.setTargetPosition(3000);
}
void StepperInit_Lib()
{
    pinMode(PIN_DRV_ENN, OUTPUT);
    digitalWrite(PIN_DRV_ENN, LOW);

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    driver.begin();
    driver.toff(5);
    driver.rms_current(1200); // previous 800
    driver.microsteps(32);
    // driver.en_pwm_mode(true);
    // driver.pwm_autoscale(true);

    Serial.println("Driver initialized");

    // === Internal ramp generator example ===
    driver.RAMPMODE(0); // Positioning mode
    driver.XACTUAL(0);
    driver.VMAX(0);                   // Max velocity (units: [tpwmth])
    driver.AMAX(1000);                // Acceleration (units: [tpwmth/s])
    driver.XTARGET(driver.XACTUAL()); // Move to position
    //  driver.XTARGET(5000000);
}

void setup()
{
    Serial.begin(115200);

    StepperInit();
    delay(1000);
    StepperInit_Lib();

    driver.RAMPMODE(0); // Positioning mode
    driver.XTARGET(5000000);
    delay(10000);
    driver.VMAX(5000);
    delay(10000);
    driver.VMAX(10000);
    delay(10000);  

}