#include "vex.h"

int brain_x, brain_y = -1; 
int Interface::isActive = 0;

/* ---------- Interface Class ---------- */

// ------ basics ------ 
Interface::Interface ()
: button_coord(nullptr), linked_Interface(nullptr), actions(nullptr), display(nullptr), updateInterface(nullptr)
{
    this->index = -2; 
    this->isActive = 0;
}
Interface::Interface (void (*display)())
    : button_coord(nullptr), 
      linked_Interface(nullptr), 
      actions(nullptr),
      display(display), 
      updateInterface(nullptr)
{
    this->index = -2; 
    this->isActive = 0; 
}
Interface::Interface (void (*display)(), int (*updateInterface)())
    : button_coord(nullptr), 
      linked_Interface(nullptr), 
      actions(nullptr), display(display), 
      updateInterface(updateInterface)
{
    this->index = -2; 
    this->isActive = 0; 
}


// ------ + button only ------
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      void (*actions)(int i),  
                      void (*display)())
    : button_coord(button_coord), 
      linked_Interface(nullptr), 
      actions(actions), 
      display(display), 
      updateInterface(nullptr)
{
    this->index = -2; 
    this->isActive = 0; 
}
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      void (*actions)(int i),  
                      void (*display)(), 
                      int (*updateInterface)())
    : button_coord(button_coord), 
      linked_Interface(nullptr),
      actions(actions), display(display), 
      updateInterface(updateInterface)
{
    this->index = -2; 
    this->isActive = 0; 
}


// ------ + linked_interface ------
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      std::vector<Interface*>* linked_Interface, 
                      void (*display)())
    : button_coord(button_coord), 
      linked_Interface(linked_Interface), 
      actions(nullptr), display(display), 
      updateInterface(nullptr)
{
    this->index = -2; 
    this->isActive = 0; 
} 
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      std::vector<Interface*>* linked_Interface,
                      void (*actions)(int i),  
                      void (*display)())
    : button_coord(button_coord), 
      linked_Interface(linked_Interface), 
      actions(actions), display(display), 
      updateInterface(nullptr)
{
    this->index = -2; 
    this->isActive = 0; 
}


// ------ + update interface ------
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      std::vector<Interface*>* linked_Interface, 
                      void (*display)(),
                      int (*updateInterface)())
    : button_coord(button_coord), 
      linked_Interface(linked_Interface), 
      actions(nullptr), display(display), 
      updateInterface(updateInterface)
{
    this->index = -2; 
    this->isActive = 0; 
} 
Interface::Interface (std::vector<std::vector<double>>* button_coord, 
                      std::vector<Interface*>* linked_Interface,
                      void (*actions)(int i),  
                      void (*display)(), 
                      int (*updateInterface)())
    : button_coord(button_coord), 
      linked_Interface(linked_Interface), 
      actions(actions), display(display), 
      updateInterface(updateInterface)
{
    this->index = -2; 
    this->isActive = 0; 
}


// ------ Functions ------
int Interface::getIndex()
{
    return this->index; 
}
void Interface::setIndex(int i)
{
    this->index = i; 
}

int Interface::InterfaceChooser()
{
    for (int j = 0; j < button_coord->size(); j++)
    {
        if (brain_x >= (*button_coord)[j][0] && brain_x <= (*button_coord)[j][1] &&
            brain_y >= (*button_coord)[j][2] && brain_y <= (*button_coord)[j][3])
        {
            this->index = j; 
            break; 
        }
    }
    return this->index;  
}

void Interface::Display()
{
    if (this->display)
    {
        this->display(); 
    }
    if (this->updateInterface && !updateItf_Task)
    {
        updateItf_Task = new task (Interface::launchUpdateItf, this);
    }
}

int Interface::UpdateInterface()
{
    this->updateInterface(); 
    while (this->index == -1 && updateItf_Task)
    {
        if (this->updateInterface)
        {
            this->updateInterface(); 
        }
        wait(100, msec); 
    }
    
    printf("in \n\n"); 
    return 0; 
}
int Interface::launchUpdateItf(void* obj)
{
    if (obj)
    {
        Interface* interface = static_cast<Interface*>(obj);
        interface->UpdateInterface(); 
    }
    return 0;
}
void Interface::stopUpdateTask()
{
    if (updateItf_Task)
    {
        updateItf_Task->stop();
        wait(100, msec);
        delete updateItf_Task;
        updateItf_Task = nullptr;
    }
}

void Interface::Action()
{
    if (this->actions)
    {
        actions(this->index); 
    }
}
void Interface::activate()
{ 
    if (this->index == -1)
    {
        
        if (Interface::isActive == 0)
        {
            this->InterfaceChooser();
            if (this->linked_Interface && this->index > -1)
            {
                Interface* chosen = (*linked_Interface)[this->index];  
                chosen->setIndex(-1); 
                this->stopUpdateTask();
                Brain.Screen.clearScreen(); 
                chosen->Display();
                this->Action(); 
                this->index = -2;
            }
        }
        Interface::isActive = 1;
    }
}

void Interface::reset()
{
    if (Brain.Screen.pressing())
    {
        waitUntil(!Brain.Screen.pressing());
        brain_x = Brain.Screen.xPosition();
        brain_y = Brain.Screen.yPosition();
        isActive = 0;
    }
}
/* ---------- Interface Class Ends ---------- */

/* ---------- Auton Class comming soon.... ---------- */