/********
Default E80 Code
Current Author:
    Wilson Ives (wives@g.hmc.edu) '20 (contributed in 2018)
Previous Contributors:
    Christopher McElroy (cmcelroy@g.hmc.edu) '19 (contributed in 2017)  
    Josephine Wong (jowong@hmc.edu) '18 (contributed in 2016)
    Apoorva Sharma (asharma@hmc.edu) '17 (contributed in 2016)                    
*/

/* Libraries */

// general
#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>

// E80-specific
#include <SensorIMU.h>
#include <MotorDriver.h>
#include <Logger.h>
#include <Printer.h>


/* Global Variables */

// period in ms of logger and printer
#define LOOP_PERIOD 100

// Motors
MotorDriver motorDriver;

// IMU
SensorIMU imu;

// Logger
Logger logger;
bool keepLogging = true;

// Printer
Printer printer;

// loop start recorder
int loopStartTime;

void setup() {
  printer.init();

  /* Initialize the Logger */
  logger.include(&imu);
  logger.include(&motorDriver);
  logger.init();

  /* Initialise the sensors */
  imu.init();

  /* Initialize motor pins */
  motorDriver.init();

  /* Keep track of time */
  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
}


void loop() {

  int currentTime = millis() - loopStartTime;
  
  ///////////  Don't change code above here! ////////////////////
  // write code here to make the robot fire its motors in the sequence specified in the lab manual 
  // the currentTime variable contains the number of ms since the robot was turned on 
  // The motorDriver.drive function takes in 3 inputs arguments motorA_power, motorB_power, motorC_power: 
  //       void motorDriver.drive(int motorA_power,int motorB_power,int motorC_power); 
  // the value of motorX_power can range from -255 to 255, and sets the PWM applied to the motor 
  // The following example will turn on motor B for four seconds between seconds 4 and 8 

  // print time
  int seTime = currentTime / 1000;
  Serial.println(seTime);

  // SIMPLE MOTOR TEST: Turn on all motors for 4 seconds 
  if (currentTime > 4000 && currentTime <8000) {
    motorDriver.drive(120,120,120);
  } else {
    motorDriver.drive(0,0,0);
  }


  // FULL MOTOR TEST
  // if (currentTime > 5000 && currentTime < 7500) { // A  up for 2.5
  //   motorDriver.drive(120,0,0);
  //   printer.printMessage("Motor A forward", 10);

  // } if (currentTime > 7500 && currentTime < 10000) { // A down for 2.5
  //   motorDriver.drive(-120,0,0);
  //   printer.printMessage("Motor A backward", 10);

  // } if (currentTime > 10000 && currentTime < 12500) { // B  forward for 2.5
  //   motorDriver.drive(0,120,0);
  //   printer.printMessage("Motor B forward", 10);

  // } if (currentTime > 12500 && currentTime < 15000) { // B  backwards for 2.5
  //   motorDriver.drive(0,-120,0);
  //   printer.printMessage("Motor B backward", 10);

  // } if (currentTime > 15000 && currentTime < 17500) { // C  forward for 2.5
  //   motorDriver.drive(0,0,120);
  //   printer.printMessage("Motor C forward", 10);

  // } if (currentTime > 17500 && currentTime < 20000) { // C  backwards for 2.5
  //   motorDriver.drive(0,0,-120);
  //   printer.printMessage("Motor C backward", 10);

  // } if (currentTime > 22500 && currentTime < 27500) { // A B C for 5 after 2.5 pause
  //   motorDriver.drive(120,120,120);
  //   printer.printMessage("All forward", 10);
    
  // } else {
  //   motorDriver.drive(0,0,0);
  //   printer.printMessage("Motor Off", 10);
  // }

  // OBSTACLE COURSE
  // 3 minute buffer 
  // if (currentTime < 30000 | currentTime > 120000) {
  //   motorDriver.drive(0,0,0);
  // } 
  // // down for 2 seconds from yellow hoop
  // if (currentTime > 30000 && currentTime < 50000) { 
  //   motorDriver.drive(-120,0,0);
  // }
  // // forward for 5 seconds through middle hoop
  // if (currentTime > 50000 && currentTime < 100000) { 
  //   motorDriver.drive(0,-120,-120);
  // }
  // // up for 2 to surface in red hoop
  // if (currentTime > 100000 && currentTime < 120000) { 
  //   motorDriver.drive(120,0,0);
  // }



  // DONT CHANGE CODE BELOW THIS LINE 
  // --------------------------------------------------------------------------

  
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,imu.printAccels());
    printer.printValue(1,imu.printRollPitchHeading());
    printer.printValue(2,motorDriver.printState());
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read(); // this is a sequence of blocking I2C read calls
  }

  if ( currentTime-logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }

}
