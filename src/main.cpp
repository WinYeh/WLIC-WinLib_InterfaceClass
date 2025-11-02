#include "vex.h"
using namespace vex;
competition Competition;

void pre_auton(void) 
{
  Monitor(); 
}

void autonomous(void) 
{ 
  
}

void usercontrol(void) 
{

}


int main() 
{
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
