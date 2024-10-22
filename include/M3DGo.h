#include <Arduino.h>

class M3DGo
{
private:
    
public:
    // Needed at the beginning of the code. Does the bare minimum mechanical setup
    void setup(); 
    // Needed only in case SCRATCH Core is also needed. In case its required, must be called every 1ms or faster
    void loop(); 
    M3DGo();
    ~M3DGo();
};
extern M3DGo go;