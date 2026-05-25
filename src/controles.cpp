#include "controles.h"

void PalomaControles::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer_ = timer_ - msStep_;
    }
}

void PalomaControles::setState(int frameX, int frameY) 
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
}


void Controles::reset() {

    posx_ = 755;
    posy_ = 135;

    fin_ = false;
}

void Controles::actualizar(float dt) {

    if (posx_ > posFinal_)
    {
       // posx_ -= 0.8;
 
    }
    else
        fin_ = true;

    if (paloma_.posy_ > 53) {

        paloma_.posy_ -= 2;
        paloma_.animar(dt);

    }
    else
        paloma_.setState(0,0);

}

void Controles::dibujar(Renderizador* motor) {

    motor->dibujarSprite("../assets/Sprites/controles/fondo.png", 3*512, 3*128, posx_, posy_, -1);
    motor->dibujarSprite("../assets/Sprites/controles/palomaControles.png", 3*256, 3*128, paloma_.posx_, paloma_.posy_, -2, 4,8,paloma_.frameActualX_,paloma_.frameActualY_);

}
