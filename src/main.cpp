#include <Arduino.h>
#include <M3DGo.h>
void setup(){
    go.begin();
}

void loop(){
    go.ScratchLoop();
}