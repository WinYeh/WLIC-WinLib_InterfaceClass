#pragma once 
extern int brain_x, brain_y;

/*  To convert the drawing of an Interface into code, please go to 
VEX Image Converter: https://suhjae.github.io/vex-image/  */ 

extern class Interface
{
    protected: 
        int index;
        const std::vector<std::vector<double>>* button_coord; 
        std::vector<Interface*>* linked_Interface;
        void (*actions)(int); 
        void (*display)(); 
        int (*updateInterface)(); 
        task* updateItf_Task = nullptr; 
        static int isActive;
    public: 
        //  ------ Constructors  ------
        // ---- basics ---- 
        Interface (); 
        Interface (void (*display)()); 
        Interface (void (*display)(), 
                   int (*updateInterface)()); 

        // ---- + button only ---- 
        Interface (std::vector<std::vector<double>>* button_coord, 
                   void (*actions)(int),  
                   void (*display)());  
        Interface (std::vector<std::vector<double>>* button_coord, 
                   void (*actions)(int),  
                   void (*display)(),
                   int (*updateInterface)()); 
        
        // ---- + linked_interface only ----
        Interface (std::vector<std::vector<double>>* button_coord, 
                   std::vector<Interface*>* linked_Interface, 
                   void (*display)()); 
        Interface (std::vector<std::vector<double>>* button_coord, 
                   std::vector<Interface*>* linked_Interface,
                   void (*actions)(int),  
                   void (*display)()); 
    
        // ---- + update interface ---- 
        Interface (std::vector<std::vector<double>>* button_coord, 
                   std::vector<Interface*>* linked_Interface, 
                   void (*display)(),
                   int (*updateInterface)()); 
        Interface (std::vector<std::vector<double>>* button_coord, 
                   std::vector<Interface*>* linked_Interface,
                   void (*actions)(int),  
                   void (*display)(),
                   int (*updateInterface)()); 
        
        //  ------ Functions  ------
        int getIndex();
        void setIndex(int);
        int InterfaceChooser(); 
        void Display(); 
        int UpdateInterface();
        static int launchUpdateItf(void* obj);  
        void stopUpdateTask(); 
        void Action(); 
        void activate(); 
        static void reset(); 
};
/* ---------- Auton Class comming soon.... ------------ */
