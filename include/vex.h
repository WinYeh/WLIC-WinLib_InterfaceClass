#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>  // C++ string class (std::string)
#include <vector>  // C++ vector class (std::vector)

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
#include "WLIC.h"
#include "display.h"

#define pi 3.14159265358979323846
#define inchToCm 2.54      // 1 inch = 2.54 cm
#define wheelDiameter 3.25 // inches
#define wheelCircumferenceCm (pi * wheelDiameter * inchToCm) // cm
#define odometry_wheel diamater 2.0 // inches
#define WL 20
#define WB 10 


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)


#define Status_Idle "Idle"
#define Status_Moving "Moving"
#define Status_Turning "Turning"
#define Status_Target "Target"
#define Status_Stopped "Stopped"