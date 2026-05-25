#include "creditos.h"

void GallinaCreditos::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else frameActualX_ = 0;
        timer_ = timer_ - msStep_;
    }
}

void BotellaCreditos::animar(float dt) 
{
    timer_ = timer_ + dt;
    if (timer_ > msStep_)
    {
        if (frameActualX_ < nFrames_ - 1) frameActualX_++;
        else if (loop_) frameActualX_ = 0;

        timer_ = timer_ - msStep_;
    }
}

void Creditos::reset() 
{
    posx_ = 726;
    posy_ = 135;

    botella_.posx_ = 364;

    fin_ = false;
}

void Creditos::actualizar(float dt) 
{
    if (posx_ > posFinal_)
    {
        posx_ -= 0.8;
        botella_.posx_ -= 0.8;
    }
    else
        fin_ = true;

    gallina_.animar(dt);

    if(gallina_.posx_ > botella_.posx_ - 54)
    botella_.animar(dt);
}


void Creditos::dibujar(Renderizador* motor) 
{
    motor->dibujarSprite("../assets/Sprites/creditos/creditos.png", 2048, 512, posx_, posy_, -1);
    motor->dibujarSprite("../assets/Sprites/creditos/botella.png", 3 * 256, 3 * 32, botella_.posx_, botella_.posy_, -1.9, 1, 8, botella_.frameActualX_, botella_.frameActualY_);
    motor->dibujarSprite("../assets/Sprites/creditos/gallinaCreditos.png", 3 * 256, 3 * 32, gallina_.posx_, gallina_.posy_, -2, 1, 8, gallina_.frameActualX_, gallina_.frameActualY_);
}


