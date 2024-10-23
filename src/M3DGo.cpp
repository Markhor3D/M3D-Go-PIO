#include <M3DGo.h>
#include "go_main.h"

M3DGo::M3DGo(/* args */)
{
}

M3DGo::~M3DGo()
{
}

void M3DGo::begin(){
    go_setup();
}
void M3DGo::ScratchLoop(){    
  DisplayLoop();
  BLELoop();
  MechanicalLoop();
}

// Hinge

Hinge::Hinge(int index)
{
}

Hinge::~Hinge()
{
}


// Display

Display::Display(/* args */)
{
}

Display::~Display()
{
}

// Range Finder


RangeFinder::RangeFinder(/* args */)
{
}

RangeFinder::~RangeFinder()
{
}

// Line Sensor

LineSensor::LineSensor()
{
}

LineSensor::~LineSensor()
{
}




// Default Objects

M3DGo go;
Hinge hingeA(0);
Hinge hingeB(1);
Display display;
RangeFinder range;
LineSensor line;

