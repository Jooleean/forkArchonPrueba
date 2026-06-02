#include "ataque.h"

void Ataque::animar(float dt)
{
    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;
        timer = timer - msStep;
    }
}

void Ataque::setState(int frameX, int frameY)
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
    pausa = true;

}

void Ataque::actualizar(float dt) {

    animar(dt);

}