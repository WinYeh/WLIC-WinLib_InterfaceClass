#include "vex.h"
using namespace vex;

brain Brain; 
controller Controller = controller(primary);

motor L1 = motor(PORT9, ratio6_1, true);  
motor L2 = motor(PORT8, ratio6_1, true); 
motor L3 = motor(PORT20, ratio6_1, false); 
motor_group Left = motor_group(L1, L2, L3);

motor R1 = motor(PORT19, ratio6_1, false);  
motor R2 = motor(PORT7, ratio6_1, false);  
motor R3 = motor(PORT18, ratio6_1, true); 
motor_group Right = motor_group(R1, R2, R3);

inertial gyro1 = inertial(PORT1);

void vexcodeInit(void) 
{
    // Nothing to initialize
}