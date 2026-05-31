#pragma once
#include <cmath>

namespace Constantes 
{
	const float ANCHO_VENTANA = 480.0f;
	const float ALTO_VENTANA = 270.0f;
	const float ASPECT_RATIO = ANCHO_VENTANA / ALTO_VENTANA;
	inline constexpr int NUMERO_ANIMALES = 18;
	inline constexpr int FILAS_TABLERO = 9;
	inline constexpr int COLUMNAS_TABLERO = 9;
}

namespace ParametrosAnimales
{
	inline constexpr float RECARGA_CABRA = 3.0f;
	inline constexpr float RECARGA_CERDO = 3.0f;
	inline constexpr float RECARGA_GALLINA = 3.0f;
	inline constexpr float RECARGA_GRANJERO = 3.0f;
	inline constexpr float RECARGA_LLAMA = 3.0f;
	inline constexpr float RECARGA_OVEJA = 3.0f;

	inline constexpr int VIDA_CABRA = 10;
	inline constexpr int VIDA_CERDO =  10;
	inline constexpr int VIDA_GALLINA = 10;
	inline constexpr int VIDA_GRANJERO = 10;
	inline constexpr int VIDA_LLAMA =  10;
	inline constexpr int VIDA_OVEJA =  10;
}

struct Casilla {
	int fila;
	int columna;
};

struct Movimiento {
	Casilla origen;
	Casilla destino;
};

struct Animacion {
	float timer;
	float msStep;
	int nFrames;
};

struct Vector2D {
	float x;
	float y;

	Vector2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
	float modulo() const { return std::sqrtf(x * x + y * y); }

	Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
	Vector2D operator*(float escalar) const { return Vector2D(x * escalar, y * escalar); }
	void operator+=(const Vector2D& v) { x += v.x; y += v.y; }
};