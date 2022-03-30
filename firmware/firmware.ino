#include <AccelStepper.h>

#define X_STEP 5
#define X_DIR 2
#define Y_STEP 6
#define Y_DIR 3

#define Z_STEP 7
#define Z_DIR 4

#define HOME_PIN 10

// 5d * 16 / 1.8(d/s) * 4 gear ratio
  


struct State {
  int16_t yaw;
  int16_t yaw_to;
  bool homing;
};

enum {
  HOME = 'h',
  YAW = 'y',
};

struct Packet {
  uint8_t action;
  int16_t value;
};

AccelStepper yawStepper = AccelStepper(AccelStepper::DRIVER,Y_STEP,Y_DIR);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(HOME_PIN, INPUT);
  yawStepper.setMaxSpeed(1000);
  yawStepper.setAcceleration(576);
//  yawStepper.setPinsInverted(true,false,false);
}

Packet command;
State state = {0,0, false};
void loop() {
  
  if (Serial.available() >= sizeof(Packet)) {
    Serial.readBytes((byte *)&command, sizeof(Packet));

    if (command.action == HOME) {
      state.homing = true;
    }
    else if (command.action == YAW) {
      state.yaw_to = command.value;
    }
    else {
      while (Serial.available()){
        Serial.read();
      }
    }
  }

  if (state.homing == true) {
    digitalWrite(LED_BUILTIN, HIGH);
    yawStepper.setSpeed(200);
    while (digitalRead(HOME_PIN) == 0) {
      yawStepper.runSpeed();
    }
    yawStepper.setSpeed(0);
    yawStepper.setCurrentPosition(0);
    state.yaw = 0;
    state.yaw_to = 0;
    state.homing = false;
    Serial.write("d");
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (state.yaw_to != state.yaw) {
     digitalWrite(LED_BUILTIN, HIGH);
     yawStepper.runToNewPosition(state.yaw_to);
     state.yaw = yawStepper.currentPosition();
     Serial.write("d");
     digitalWrite(LED_BUILTIN, LOW);
  }
}
