#pragma once

extern std::string status;
void setStatus(std::string stat); 
int printPerformance(); 
void displayHome(); 

void drawdirection(double angle); 
void setTargetHeading(double targ); 
void displayHeading();

void displayPowerflow();

void displayAuton();
void displayAutonRed(); 
void displayAutonBlue();

void displayLogo(); 
void displayUtil();

int Monitor();