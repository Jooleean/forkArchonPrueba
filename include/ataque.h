#pragma once
#include <cmath>

// El dibujo del ataque lo hace renderizador.cpp usando getSprite(), getPosX(), getPosY(), etc.

class Ataque {

protected:
    int dano_;
    float alcance_;
    float recarga_;
    const char* sprite_;
    float tamano_;
    float duracion_visual_;
    bool contacto_detectado_ = false;

    float posX_ = 0, posY_ = 0;
    float dirX_ = 0, dirY_ = 0;
    bool  activo_ = false;
    float tiempoActivo_ = 0;

public:

    Ataque(int dano, float alcance, float recarga, const char* sprite, float tamanio, float duracion_visual)
        : dano_(dano), alcance_(alcance), recarga_(recarga),
        sprite_(sprite), tamano_(tamanio), duracion_visual_(duracion_visual) {}

    virtual ~Ataque() {}

    int getDano() const { return dano_; }
    float getAlcance() const { return alcance_; }
    float getRecarga() const { return recarga_; }
    const char* getSprite()  const { return sprite_; }
    float getTamanio() const { return tamano_; }
    float getPosX() const { return posX_; }
    float getPosY() const { return posY_; }
    bool isActivo() const { return activo_; }
    float getTiempoActivo() const { return tiempoActivo_; }
    void setContactoDetectado(bool v) { contacto_detectado_ = v; }
    bool getContactoDetectado() const { return contacto_detectado_; }

    virtual void activar(float x, float y, float dirX, float dirY) {
        posX_ = x;
        posY_ = y;
        dirX_ = dirX; 
        dirY_ = dirY;
        activo_ = true;
        tiempoActivo_ = 0;
    }

    void desactivar() {
        activo_ = false;
        tiempoActivo_ = 0;
        contacto_detectado_ = false;
        frameActualX_ = 0; 
    }
  
    virtual void mover(float dt) = 0;

    virtual void actualizar(float dt);

    int frameActualX_ = 0;
    int frameActualY_ = 0;
    int nFrames;
    float timer = 0;
    float msStep = 50;
    bool pausa = true;
    int ancho = 0;
    int alto = 0;
    bool loop = false;
    void setState(int frameX, int frameY);
    void animar(float dt);

    int getFrameActualX() const { return frameActualX_; }
    int getFrameActualY() const { return frameActualY_; }

};