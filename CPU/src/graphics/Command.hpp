#include <Turtle.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Command
{
public:
    virtual void execute(Turtle& turtle) = 0;

    void initDefault(float step, float rotation)
    {
        this->step = step;
        this->rotation = rotation;
    }

protected:
    float step;
    float rotation;
};
    
class StepForward : public Command
{
public:
    void execute(Turtle& turtle)
    {
        turtle.step(step);
    }
};

class RotatePositive : public Command
{
public:
    void execute(Turtle& turtle)
    {
        turtle.rotate(rotation/180*M_PI);
    }
};

class RotateNegative : public Command
{
public:
    void execute(Turtle& turtle)
    {
        turtle.rotate(-rotation/180*M_PI);
    }
};

class savePosition : public Command
{
public:
    void execute(Turtle& turtle)
    {
        turtle.savePosition();
    }
};

class usePosition : public Command
{
public:
    void execute(Turtle& turtle)
    {
        turtle.usePosition();
    }
};