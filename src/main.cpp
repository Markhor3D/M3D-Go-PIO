#include <M3DGo.h>

void setup(){
    go.begin();
}

void loop(){

    remote.waitForButton("Should I begin?", "Begin!");
    go.spinClockwise(50);
    while(line.touching()) // already on the line, make sure it is off the line
        delay(1);

    while(line.notOnLine())
    {
        remote.indication1.polarPlot("Distance", range.get_cm());
        delay(50);
    }
    go.stop();
}