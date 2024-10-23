#include <M3DGo.h>

void setup(){
    go.begin();
    go.calibrate();

    go.travel_cm(100);
    go.spinAngle(180);
    go.travel_cm(100);
    go.spinAngle(180);
}

void loop(){

}