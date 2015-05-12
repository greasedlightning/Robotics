#ifndef SANDY_H_
#define SANDY_H_

float exponentialJoystick(int joyVal);
void sticksUp();
void sticksDown();
void retainBalls();
void releaseBalls();
void retainAutoBall();
void allStop();
void raiseLift(int powerLevel);
void lowerLift(int powerLevel);
void intakeIn(int powerLevel);
void intakeOut(int powerLevel);
void forward(int powerLevel);
void backward(int powerLevel);
void left(int powerLevel);
void right(int powerLevel);
void init();
void drive(int direction,float time,int powerLevel);
void driveSonar(int direction,float distance,int powerLevel);
void turn(int direction, int degrees,int powerLevel);

#endif //SANDY_H_