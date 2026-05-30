#include "tablero.h"
#include <cmath>

Tablero::Tablero(Jugador* jugador1, Jugador* jugador2)
{
    jugadores_[0] = jugador1;
    jugadores_[1] = jugador2;
    inicializarTablero();

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
            casillas_[i][j] = jugadores_[0]->getAnimales()[j * Constantes::FILAS_TABLERO + i];

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
            casillas_[i][8 - j] = jugadores_[1]->getAnimales()[j * Constantes::FILAS_TABLERO + i];
}

Tablero::~Tablero() {} // las piezas se destruyen en el jugador, no en el tablero

void Tablero::inicializarTablero()
{
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
    {
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++)
        {
            casillas_[i][j] = nullptr;

            if ((i + j) % 2 == 0)
                color_casilla_[i][j] = CASILLA_LUZ;
            else
                color_casilla_[i][j] = CASILLA_OSCURA;
        }
    }
}

void Tablero::actualizar(float dt)
{
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++)
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++)
            if (casillas_[i][j] != nullptr)
                casillas_[i][j]->actualizarEnTablero(dt);

    if (getJugadorActivo()->tienePiezaAgarrada())
        getJugadorActivo()->getPiezaSeleccionada()->actualizarEnTablero(dt);

    actualizarColision();

    setLetreroPosX(102 + turno_actual_ * 273);
    letreroTurnos_.animar(dt);

    determinarGanador();
}

void Tablero::recibirMovimiento(int jugador, int dx, int dy) // dx y dy son -1, 0 o 1 dependiendo de la dirección del movimiento
{
    if (casillas_[8][0] != nullptr && casillas_[8][0]->getIntroTablero()) return;

    Cursor& cursor = jugador == 0 ? cursorJ1_ : cursorJ2_;

    if (jugador == turno_actual_)
    {
        Jugador* jugadorActivo = getJugadorActivo();

        if (estadoHechizo_ != INACTIVO) // si hay un hechizo activo, se mueve el cursor
        {
            cursor.mover(dx, dy);
            return; // corta aquí para que no intente mover al granjero
        }

		if (!jugadorActivo->tienePiezaAgarrada()) // si no tiene pieza agarrada, se mueve el cursor
        {
            cursor.mover(dx, dy);
        }
		else // si tiene una pieza agarrada, se intenta mover la pieza
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();
            if (pieza->getEnMovimiento()) return;

            bool movimiento_valido = false;
            movimiento_valido = pieza->mover(TABLERO, dx, dy);
        }
    }
}

void Tablero::seleccionarPieza(int jugador, RenderizadorAudio* audio)
{
    if (turno_actual_ == jugador)
    {
        Cursor& cursor = getCursorActivo();
        Jugador* jugadorActivo = getJugadorActivo();
        Animal* casilla = casillas_[cursor.fila][cursor.columna];

        if (estadoHechizo_ != INACTIVO) {
            ejecutarPasoHechizo(casilla, cursor.fila, cursor.columna);
            return;
        }

        // CASO 1: LEVANTAR UNA PIEZA
        if (!jugadorActivo->tienePiezaAgarrada() && casilla != nullptr)
        {
            if (casilla->equipo_ == jugador)
            {
                if (casilla->atrapado_) {
                    std::cout << "[!] Este animal esta atrapado en la red. Le quedan " << casilla->ciclos_atrapado_ << " turnos.\n";
                    return; // bloquea recoger la pieza
                }

                // guardar el origen antes de levantarla físicamente
                casilla->casillaInicial_ = { cursor.fila, cursor.columna };

                jugadorActivo->agarrarPieza(casilla);
                casillas_[cursor.fila][cursor.columna] = nullptr;

                audio->sonarPickeo(casilla);
            }
        }
        // CASO 2: SOLTAR UNA PIEZA (Intentar movimiento)
        else if (jugadorActivo->tienePiezaAgarrada())
        {
            Animal* pieza = jugadorActivo->getPiezaSeleccionada();

            // si la pieza sigue desplazándose visualmente, evitar soltarla
            if (pieza->getEnMovimiento()) return;

            Movimiento m;
            m.origen = pieza->casillaInicial_;

            // leer la coordenada destino matemática a partir de los píxeles
            m.destino.columna = std::round((pieza->getPosX() - 152.0f) / 22.0f);
            m.destino.fila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

            if (esMovimientoLegal(m))
            {
                if (getHayColision()) // asignar animales de combate a piezas chocantes (J1 izquierda, J2 derecha siempre)
                {
                    if (pieza->equipo_ == 0)
                    {
                        jugadores_[0]->setAnimalEnCombate(pieza);
                        jugadores_[1]->setAnimalEnCombate(casillas_[m.destino.fila][m.destino.columna]);
                    }
                    else
                    {
                        jugadores_[0]->setAnimalEnCombate(casillas_[m.destino.fila][m.destino.columna]);
                        jugadores_[1]->setAnimalEnCombate(pieza);
                    }

                    casillaDisputada = m.destino;
                    enBatalla = true;
                }

                mover(m);

                // teletransporta el cursor a la nueva casilla
                // aprovechando cursor.mover iterando hasta la meta con while
                while (cursor.columna < m.destino.columna) cursor.mover(1, 0);
                while (cursor.columna > m.destino.columna) cursor.mover(-1, 0);
                while (cursor.fila > m.destino.fila) cursor.mover(0, 1);  // dy=1 es ARRIBA (resta fila)
                while (cursor.fila < m.destino.fila) cursor.mover(0, -1);

                jugadorActivo->soltarPieza();

                turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
                letreroTurnos_.setState(0, turno_actual_);
            }
            else
            {
                // MOVIMIENTO ILEGAL: volver a colocar la pieza en su posición original
                casillas_[m.origen.fila][m.origen.columna] = pieza;

                float origX = 141.0f + 11.0f + (22.0f * m.origen.columna);
                float origY = 36.0f + 11.0f + (22.0f * (8 - m.origen.fila)); // lo de (8 - m.origen.fila) se explica más abajo en la linea 274
                pieza->setPosX(origX);
                pieza->setPosy(origY);

                // parar el movimiento de la pieza, por si acaso
                pieza->setVelX(0);
                pieza->setVelY(0);
                pieza->setEnMovimiento(false);
                pieza->avanzando_casilla_ = 0;
                pieza->casillas_movidas_ = 0;
                pieza->casillas_movidas_x_ = 0;
                pieza->casillas_movidas_y_ = 0;

                jugadorActivo->soltarPieza(); // soltar la pieza aunque el movimiento sea ilegal
                // no se cambia el turno, el jugador vuelve a intentarlo
            }
        }
    }
}

void Tablero::actualizarColision()
{
    Jugador* jugadorActivo = getJugadorActivo();

    if (jugadorActivo->tienePiezaAgarrada())
    {
        Animal* pieza = jugadorActivo->getPiezaSeleccionada();

        // calcula la casilla sobre la que está volando el animal
        int destCol = std::round((pieza->getPosX() - 152.0f) / 22.0f); // round sirve para redondear al entero más cercano
        int destFila = 8 - std::round((pieza->getPosY() - 47.0f) / 22.0f);

        // Si está dentro del tablero y hay un enemigo, activar colisión
        if (destFila >= 0 && destFila < Constantes::FILAS_TABLERO &&
            destCol >= 0 && destCol < Constantes::COLUMNAS_TABLERO)
        {
            if (casillas_[destFila][destCol] != nullptr &&
                casillas_[destFila][destCol]->equipo_ != jugadorActivo->getEquipo())
            {
                hay_colision_ = true;

                return;
            }
        }
    }

    hay_colision_ = false;
}

void Letrero::animar(float dt)
{
    timer = timer + dt;
    if (timer > msStep)
    {
        if (frameActualX_ < nFrames - 1) frameActualX_++;
        else if (loop) frameActualX_ = 0;
        timer = timer - msStep;
    }
}

void Letrero::setState(int frameX, int frameY)
{
    frameActualX_ = frameX;
    frameActualY_ = frameY;
    loop = false;
}

bool Tablero::esMovimientoLegal(const Movimiento& m) const
{
    // validar que el destino esté dentro de las dimensiones del tablero
    if (m.destino.fila < 0 || m.destino.fila >= Constantes::FILAS_TABLERO ||
        m.destino.columna < 0 || m.destino.columna >= Constantes::COLUMNAS_TABLERO)
        return false;

    // obtener el jugador activo
    Jugador* jugadorActivo = jugadores_[turno_actual_];
    if (!jugadorActivo || !jugadorActivo->tienePiezaAgarrada())
        return false;

    // obtener la pieza seleccionada
    Animal* pieza = jugadorActivo->getPiezaSeleccionada();
    if (!pieza)
        return false;

    // comprobar colisión con piezas del propio equipo
    Animal* casillaDestino = casillas_[m.destino.fila][m.destino.columna];
    if (casillaDestino != nullptr && casillaDestino->equipo_ == turno_actual_)
        return false;

    // conexión con los vectores de movimientos posibles de cada animal
    std::vector<Movimiento> permitidos = pieza->movimientosPosibles();
    bool esValidoGeometricamente = false;

    for (const auto& mov : permitidos)
    {
        // comprobar si el destino propuesto coincide con uno de los destinos calculados por el animal
        if (mov.destino.fila == m.destino.fila && mov.destino.columna == m.destino.columna)
        {
            esValidoGeometricamente = true;
            break;
        }
    }

    return esValidoGeometricamente;
}

void Tablero::mover(const Movimiento& m)
{
    Jugador* jugadorActivo = getJugadorActivo();
    Animal* pieza = jugadorActivo->getPiezaSeleccionada();

    // mover la pieza en la matriz lógica del tablero
    casillas_[m.destino.fila][m.destino.columna] = pieza;

    // sincronizar la posición física/gráfica del animal con su nuevo destino
    float nuevaPosX = 141.0f + 11.0f + (22.0f * m.destino.columna);
    float nuevaPosY = 36.0f + 11.0f + (22.0f * (8 - m.destino.fila)); // invertir el eje Y para que la fila 0 esté en la parte inferior del tablero
    // esto es porque en la lógica del tablero, la fila 0 es la inferior, pero en el dibujo, la fila 0 está en la parte superior.
    // habría que cambiar alguna de las dos cosas para que no haya que hacer esta conversión, pero es un detalle menor y no afecta a la lógica del juego

    pieza->setPosicion(Vector2D(nuevaPosX, nuevaPosY)); // este tipo de uso de vector2D hay que hacerlo en todo el código

    // parar el movimiento de la pieza, por si acasoS
    pieza->setVelocidad(Vector2D(0, 0));

    pieza->setEnMovimiento(false);
    pieza->avanzando_casilla_ = 0;

    // limpiar los contadores de casillas movidas
    pieza->casillas_movidas_ = 0;
    pieza->casillas_movidas_x_ = 0;
    pieza->casillas_movidas_y_ = 0;
}

int Tablero::determinarGanador() {

    if (casillas_[4][4] != nullptr && casillas_[4][0] != nullptr && casillas_[4][8] != nullptr && casillas_[0][4] != nullptr && casillas_[8][4] != nullptr)
        if (casillas_[4][4]->equipo_ == casillas_[4][0]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[4][8]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[0][4]->equipo_ &&
            casillas_[4][4]->equipo_ == casillas_[8][4]->equipo_)
            return casillas_[4][4]->equipo_;

    // + condición de ganar por eliminación

    return -1;
}

void Tablero::acomodarGanador(Animal* animalGanador)
{
    casillas_[casillaDisputada.fila][casillaDisputada.columna] = animalGanador;
    casillas_[casillaDisputada.fila][casillaDisputada.columna]->
    setPosicion({ 141.0f + 11.0f + 22.0f * casillaDisputada.columna, 36.0f + 11.0f + 22.0f * (8 - casillaDisputada.fila) });
}

// HECHIZOS
void Tablero::procesarTeclaHechizo(int tecla) { 
    Jugador* jugadorActivo = getJugadorActivo();

    // solo se pueden lanzar hechizos si tienes a alguien agarrado (de nada, chile)
    if (!jugadorActivo->tienePiezaAgarrada()) return;

    Animal* pieza = jugadorActivo->getPiezaSeleccionada();

    // solo el granjero puede lanzar hechizos
    if (pieza->getEspecie() != GRANJERO) return;

    // si pulsas la misma tecla con la que activaste el hechizo, se cancela
    if (teclaHechizoActivo_ == tecla) {
        std::cout << "Hechizo cancelado. Mueve el Granjero o elige otro hechizo.\n";
        estadoHechizo_ = INACTIVO;
        teclaHechizoActivo_ = -1;
        primerObjetivoHechizo_ = nullptr;
        return;
    }

    int bando = jugadorActivo->getEquipo(); // 0 luz, 1 oscuridad
    int tipoHechizo = 0;

    // teclas: J1 usa 1-5, J2 usa 6-0
    if (bando == 0) {
        if (tecla >= 1 && tecla <= 5) tipoHechizo = tecla;
        else return;
    }
    else {
        if (tecla >= 6 && tecla <= 9) tipoHechizo = tecla - 5;
        else if (tecla == 0) tipoHechizo = 5;
        else return;
    }

    teclaHechizoActivo_ = tecla;
    std::cout << "\n--- Jugador " << (bando + 1) << " invocando hechizo ---\n";

    // asignar el hechizo y guiar al usuario, 
    // esto de momento en consola, habría que usar ETSIDILIB para escribirlo en pantalla en algún sitio libre
    switch (tipoHechizo) {
    case 1: // TELETRANSPORTE
        estadoHechizo_ = TELETRANSPORTE_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Teletransporte.\n";
        std::cout << "> Selecciona un animal de tu equipo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 2: // CURAR
        estadoHechizo_ = CURAR_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Curar.\n";
        std::cout << "> Selecciona un animal de tu equipo para sanarlo al maximo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 3: // INTERCAMBIO
        estadoHechizo_ = INTERCAMBIO_SELECCIONAR_ALIADO;
        std::cout << "HECHIZO: Intercambio.\n";
        std::cout << "> Selecciona un animal de tu equipo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    case 4: // ATRAPAR
        estadoHechizo_ = ATRAPAR_SELECCIONAR_ENEMIGO;
        std::cout << "HECHIZO: Atrapar.\n";
        std::cout << "> Selecciona un animal rival para inmovilizarlo.\n";
        std::cout << "> Pulsa " << tecla << " para cancelar.\n";
        break;
    default:
        std::cout << "Hechizo " << tipoHechizo << " no existe.\n";
        estadoHechizo_ = INACTIVO;
        teclaHechizoActivo_ = -1;
        break;
    }
}

void Tablero::ejecutarPasoHechizo(Animal* casilla, int fila, int col) {
    Jugador* jugadorActivo = getJugadorActivo();
    int bando = jugadorActivo->getEquipo();

    switch (estadoHechizo_) {
    case TELETRANSPORTE_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando && casilla->getEspecie() != GRANJERO) {
            primerObjetivoHechizo_ = casilla;
            estadoHechizo_ = TELETRANSPORTE_SELECCIONAR_DESTINO;
            std::cout << "> Animal Aliado seleccionado.\n> Ahora mueve el cursor a una cailla vacia y seleccionala como destino.\n";
        }
        else {
            std::cout << "[!] Debes seleccionar un animal de tu equipo (que no sea el Granjero).\n";
        }
        break;

    case TELETRANSPORTE_SELECCIONAR_DESTINO:
        if (casilla == nullptr) { // Casilla vacía
            casillas_[fila][col] = primerObjetivoHechizo_;
            casillas_[primerObjetivoHechizo_->casillaInicial_.fila][primerObjetivoHechizo_->casillaInicial_.columna] = nullptr;

            primerObjetivoHechizo_->casillaInicial_ = { fila, col };
            float nuevaPosX = 141.0f + 11.0f + (22.0f * col);
            float nuevaPosY = 36.0f + 11.0f + (22.0f * (8 - fila));
            primerObjetivoHechizo_->setPosicion(Vector2D(nuevaPosX, nuevaPosY));

            std::cout << ">> Teletransporte completado con exito!\n";
            finalizarHechizo();
        }
        else {
            std::cout << "[!] La parcela de destino debe estar vacia.\n";
        }
        break;

    case CURAR_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando) {
            casilla->vida_ = 10; // FALTA PONER LA VIDA MAXIMA DE CADA ANIMAL, habrá que hacer un getVidaMaxima() o algo así
            std::cout << ">> Animal curado al maximo de vida!\n";
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal de tu equipo.\n";
        }
        break;

    case INTERCAMBIO_SELECCIONAR_ALIADO:
        if (casilla && casilla->getEquipo() == bando && casilla->getEspecie() != GRANJERO) {
            primerObjetivoHechizo_ = casilla;
            estadoHechizo_ = INTERCAMBIO_SELECCIONAR_ENEMIGO;
            std::cout << "> Animal Aliado seleccionado.\n> Ahora mueve el cursor y selecciona un animal RIVAL.\n";
        }
        else {
            std::cout << "[!] Selecciona un animal de tu equipo (que no sea el Granjero).\n";
        }
        break;

    case INTERCAMBIO_SELECCIONAR_ENEMIGO:
        if (casilla && casilla->getEquipo() != bando) {
            Casilla pos1 = primerObjetivoHechizo_->casillaInicial_;
            Casilla pos2 = casilla->casillaInicial_;

            // intercambio en la matriz
            casillas_[pos2.fila][pos2.columna] = primerObjetivoHechizo_;
            casillas_[pos1.fila][pos1.columna] = casilla;

            // intercambio de atributos lógicos
            primerObjetivoHechizo_->casillaInicial_ = pos2;
            casilla->casillaInicial_ = pos1;

            // actualizar física aliado
            float nuevaPosX1 = 141.0f + 11.0f + (22.0f * pos2.columna);
            float nuevaPosY1 = 36.0f + 11.0f + (22.0f * (8 - pos2.fila));
            primerObjetivoHechizo_->setPosicion(Vector2D(nuevaPosX1, nuevaPosY1));

            // actualizar física rival
            float nuevaPosX2 = 141.0f + 11.0f + (22.0f * pos1.columna);
            float nuevaPosY2 = 36.0f + 11.0f + (22.0f * (8 - pos1.fila));
            casilla->setPosicion(Vector2D(nuevaPosX2, nuevaPosY2));

            std::cout << ">> Intercambio completado!\n";
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal rival.\n";
        }
        break;

    case ATRAPAR_SELECCIONAR_ENEMIGO:
        if (casilla && casilla->getEquipo() != bando) {
            casilla->atrapado_ = true;
            casilla->ciclos_atrapado_ = 3;
            std::cout << ">> Animal rival atrapado e inmovilizado por 3 turnos!\n";
            finalizarHechizo();
        }
        else {
            std::cout << "[!] Debes seleccionar un animal rival.\n";
        }
        break;

    default:
        break;
    }
}

void Tablero::finalizarHechizo() {
    Jugador* jugadorActivo = getJugadorActivo();
    Animal* granjero = jugadorActivo->getPiezaSeleccionada();

    // devolver el granjero a su casilla original ya que castear el hechizo consume su acción
    float origX = 141.0f + 11.0f + (22.0f * granjero->casillaInicial_.columna);
    float origY = 36.0f + 11.0f + (22.0f * (8 - granjero->casillaInicial_.fila));
    granjero->setPosicion(Vector2D(origX, origY));
    casillas_[granjero->casillaInicial_.fila][granjero->casillaInicial_.columna] = granjero;

    jugadorActivo->soltarPieza();

    // reset de la  máquina de estados
    estadoHechizo_ = INACTIVO;
    teclaHechizoActivo_ = -1;
    primerObjetivoHechizo_ = nullptr;

    // cambio de turno
    turno_actual_ = (turno_actual_ == 0) ? 1 : 0;
    letreroTurnos_.setState(0, turno_actual_);

    avanzarTurnosAtrapados();
}

void Tablero::avanzarTurnosAtrapados() {
    for (int i = 0; i < Constantes::FILAS_TABLERO; i++) {
        for (int j = 0; j < Constantes::COLUMNAS_TABLERO; j++) {
            Animal* anim = casillas_[i][j];
            // disminuye en 1 si es el turno del jugador al que pertenece el animal atrapado
            if (anim != nullptr && anim->atrapado_ && anim->equipo_ == turno_actual_) {
                anim->ciclos_atrapado_--;
                if (anim->ciclos_atrapado_ <= 0) {
                    anim->atrapado_ = false;
                    std::cout << "\n>> Un animal atrapado del jugador " << (turno_actual_ + 1) << " se ha liberado!\n";
                }
            }
        }
    }
}