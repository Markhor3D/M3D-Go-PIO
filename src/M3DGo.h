class Hinge;
class Display;
class RangeFinder;
class LineSensor;

extern Hinge hingeA;
extern Hinge hingeB;
extern Display display;
extern RangeFinder range;
extern LineSensor line;

class M3DGo
{
private:
    /* data */
public:
    M3DGo(/* args */);
    void begin();
    void ScratchLoop();
    ~M3DGo();
};

extern M3DGo go;

class Hinge
{
private:
    int index = -1;
public:
    Hinge(int index);
    ~Hinge();
};

class Display
{
private:
    /* data */
    bool hasBegun = false;
public:
    Display(/* args */);
    ~Display();
};

class RangeFinder
{
private:
    /* data */
public:
    RangeFinder(/* args */);
    ~RangeFinder();
};

class LineSensor
{
private:
    /* data */
public:
    LineSensor(/* args */);
    ~LineSensor();
};
