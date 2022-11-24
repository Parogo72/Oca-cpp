#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// Definicion de constantes

const int NUM_CASILLAS = 63;
const int CASILLA_PARTIDA = 1;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int PENALIZACION_LABERINTO = 12;
const int DADO_MAXIMO = 6;
const int DADO_MINIMO = 1;
const int NUM_JUGADORES = 2;
const int NUM_JUGADORES_MAX = 4;
const int CENTINELA = 0;
const int NUM_FILAS = 3;
const int NUM_COLUMNAS = NUM_CASILLAS / NUM_FILAS;
const bool MODO_DEBUGS = true;

typedef enum {
	NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA,
	LABERINTO, DADO1, DADO2, CARCEL, CALAVERA
} tCasilla;

typedef tCasilla tTablero[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];

/**
   * Comprueba si una casilla conlleva un premio (avance o retroceso) y una tirada extra (OCA, DADO, PUENTE)
   * @example
   * // Expected output: true
   * esCasillaPremio([OCA, NORMAL, LABERINTO], 0);
   *
   * // Expected output: false
   * // Porque LABERINTO no conlleva un premio.
   * esCasillaPremio([OCA, NORMAL, LABERINTO], 2);
*/
bool esCasillaPremio(const tTablero tablero, int casilla);

/**
   * Comprueba si una casilla es meta
   * @example
   * // Expected output: true
   * esMeta(63);
   *
   * // Expected output: false
   * esMeta(25);
*/
bool esMeta(int casilla);

/**
   * Pide por pantalla el nombre del fichero para abrir y devuelve un boolean conforme se pudo abrir o no
*/
bool cargaTablero(tTablero tablero);

/**
   * Saca un numero aleatorio entre DADO_MAXIMO y DADO_MINIMO y lo devuelve
*/
int tirarDado();

/**
   * Pide el valor del dado a tirar por pantalla y lo devuelve
*/
int tirarDadoManual();

/**
   * Saca un numero aleatorio entre NUM_JUGADORES y 1 y lo devuelve
*/
int quienEmpieza();

/**
   * Busca la casilla de avance o retroceso que ocasiona la casilla introducida y devuelve su posicion
   * @example
   * // Expected output: 5
   * esCasillaPremio([OCA, NORMAL, LABERINTO, NORMAL, NORMAL, OCA], 0);
*/
int saltaACasilla(const tTablero tablero, int casillaActual);

/**
   * Se encarga de que la partida se lleve a cabo y devuelve el numero del jugador ganador
*/
int partida(const tTablero tablero);

/**
   * Busca la posicion de la siguiente casilla igual a tipo empezando desde "posicion" hasta el final y cambia el valor de "posicion"
*/
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);

/**
   * Busca la posicion de la siguiente casilla igual a tipo empezando desde "posicion" hasta el principio y cambia el valor de "posicion"
*/
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);

/**
   * Implementa la logica de cada tirada
   * Lanza el dado, muestra la nueva posicion y llama a efectoTirada() si es necesario
*/
void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);

/**
   * Determina el efecto de la casilla en la que ha caido el jugador y cambia los valores de casillaJ y penalizacionJ de ser necesario
*/
void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);

/**
   * Inicializa los arrays de casillasJ y penalizacionesJ de acorde a NUM_JUGADORES
*/
void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);

/**
   * Inicializa el tablero poniendo las casillas a NORMAL y la ultima a OCA
*/
void iniciaTablero(tTablero tablero);

/**
   * Se encarga de la logica para cambiar de turno
*/
void cambioTurno(int& jugadorActivo);

/**
   * Devuelve como enum el valor de la string proporcionada
   * @example
   * // Expected output: OCA
   * stringToEnum("OCA");
   *
   * // Expected output: LABERINTO
   * stringToEnum("LABERINTO");
*/
tCasilla stringToEnum(string str);

/**
   * Devuelve como string simplificado el valor del enum proporcionado
   * @example
   * // Expected output: "OCA"
   * stringToEnum(OCA);
   *
   * // Expected output: LBRN
   * stringToEnum(LABERINTO);
*/
string enumToString(tCasilla type);

//-------------------------------------------------------------------------
// Subprogramas para pintar el tablero o mostrar informaci�n

void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);
//---------------------------------------------------------------------------


int main() {
	tTablero tablero;
	srand(time(NULL));

	iniciaTablero(tablero);
	if (cargaTablero(tablero)) {
		int ganador = partida(tablero);
		cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
	}
	else {
		cout << "No pude abrir el archivo o no existe";
	}
	return 0;
}

bool esCasillaPremio(const tTablero tablero, int casilla) {
	int casillaTablero = tablero[casilla];
	return casillaTablero == OCA || casillaTablero == DADO1 || casillaTablero == DADO2 || casillaTablero == PUENTE1 || casillaTablero == PUENTE2;
}

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS - 1;
}

bool cargaTablero(tTablero tablero) {
	fstream archivo;
	string name;
	bool opened = false, ended = false;
	cout << "Que archivo quieres abrir? ";
	cin >> name;
	archivo.open(name);
	opened = archivo.is_open();
	if (opened) {
		int numCasilla;
		string tipoCasilla;
		archivo >> numCasilla;
		while (numCasilla != CENTINELA) {
			archivo >> tipoCasilla;
			tablero[numCasilla - 1] = stringToEnum(tipoCasilla);
			archivo >> numCasilla;
		}
		archivo.close();
	}
	return opened;
}

int tirarDado() {
	int valorDado = DADO_MINIMO + rand() % (DADO_MAXIMO + 1 - DADO_MINIMO);
	cout << valorDado << endl;
	return valorDado;
}

int tirarDadoManual() {
	int valorDado;
	cin >> valorDado;
	return valorDado;
}

int quienEmpieza() {
	return 1 + rand() % (NUM_JUGADORES);
}

int saltaACasilla(const tTablero tablero, int casillaActual) {
	tCasilla casillaTablero = tablero[casillaActual];
	if (casillaTablero == OCA) {
		buscaCasillaAvanzando(tablero, OCA, casillaActual);
	}
	else if (casillaTablero == DADO1) {
		buscaCasillaAvanzando(tablero, DADO2, casillaActual);
	}
	else if (casillaTablero == PUENTE1) {
		buscaCasillaAvanzando(tablero, PUENTE2, casillaActual);
	}
	else if (casillaTablero == DADO2) {
		buscaCasillaRetrocediendo(tablero, DADO1, casillaActual);
	}
	else if (casillaTablero == PUENTE2) {
		buscaCasillaRetrocediendo(tablero, PUENTE1, casillaActual);
	}
	return casillaActual;
}

int partida(const tTablero tablero) {
	tJugadores casillasJ, penalizacionJ;
	int jugadorActivo = quienEmpieza();

	iniciaJugadores(casillasJ, penalizacionJ);
	pintaTablero(tablero, casillasJ);
	cout << "Comienza el juego" << endl;
	cout << endl << "**** EMPIEZA EL JUGADOR " << jugadorActivo << " ****" << endl;
	while (!esMeta(casillasJ[jugadorActivo - 1])) {
		int penalizacion = penalizacionJ[jugadorActivo - 1];
		if (!penalizacion) {
			do {
				tirada(tablero, casillasJ[jugadorActivo - 1], penalizacionJ[jugadorActivo - 1]);
				pintaTablero(tablero, casillasJ);
			} while (esCasillaPremio(tablero, casillasJ[jugadorActivo - 1]) && !esMeta(casillasJ[jugadorActivo - 1]));

			if (!esMeta(casillasJ[jugadorActivo - 1])) {
				cambioTurno(jugadorActivo);
			}
		}
		else {
			cout << "... PERO NO PUEDE " << (penalizacion > 1 ? "Y LE QUEDAN " + to_string(penalizacion) + " TURNOS SIN JUGAR" : " HASTA EL SIGUIENTE TURNO") << endl;
			penalizacionJ[jugadorActivo - 1] -= 1;
			cambioTurno(jugadorActivo);
		}
	}
	cout << "** FIN DEL JUEGO **" << endl;
	pintaTablero(tablero, casillasJ);
	return jugadorActivo;
}

void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion) {
	bool found = false;
	int i = posicion + 1;
	while (!found && i < NUM_CASILLAS) {
		if (tablero[i] == tipo) {
			posicion = i;
			found = true;
		}
		i++;
	}
}

void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion) {
	bool found = false;
	int i = posicion - 1;
	while (!found && i >= 0) {
		if (tablero[i] == tipo) {
			posicion = i;
			found = true;
		}
		i--;
	}
}

void tirada(const tTablero tablero, int& casillaActual, int& penalizacion) {
	cout << "CASILLA ACTUAL: " << casillaActual + 1 << endl;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	int valorDado = (MODO_DEBUGS ? tirarDadoManual() : tirarDado());
	cout << "VALOR DEL DADO: " << valorDado << endl;
	casillaActual += valorDado;
	cout << "PASAS A LA CASILLA " << casillaActual + 1 << endl;
	if (!esMeta(casillaActual)) {
		efectoTirada(tablero, casillaActual, penalizacion);
	}
}

void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ) {
	tCasilla casillaTablero = tablero[casillaJ];
	if (esCasillaPremio(tablero, casillaJ)) {
		casillaJ = saltaACasilla(tablero, casillaJ);
		if (casillaTablero == OCA) {
			cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA" << endl;
			cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << casillaJ + 1 << endl;
		}
		else if (casillaTablero == DADO1 || casillaTablero == DADO2) {
			cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
			cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << casillaJ + 1 << endl;
		}
		else if (casillaTablero == PUENTE1 || casillaTablero == PUENTE2) {
			cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
			cout << "SALTAS AL PUENTE EN LA CASILLA: " << casillaJ + 1 << endl;
		}
		if (!esMeta(casillaJ)) cout << "Y VUELVES A TIRAR" << endl;
	}
	else {
		int turnosPerdidos = 0;
		if (casillaTablero == POSADA) {
			turnosPerdidos += TURNOS_POSADA;
			cout << "HAS CAIDO EN LA POSADA" << endl;
		}
		else if (casillaTablero == CARCEL) {
			turnosPerdidos += TURNOS_PRISION;
			cout << "HAS CAIDO EN LA PRISION" << endl;
		}
		else if (casillaTablero == POZO) {
			turnosPerdidos += TURNOS_POZO;
			cout << "HAS CAIDO EN EL POZO" << endl;
		}
		else if (casillaTablero == LABERINTO) {
			casillaJ -= PENALIZACION_LABERINTO;
			cout << "CAES EN EL LABERINTO" << endl;
			cout << "SALTAS A LA CASILLA " << casillaJ + 1 << endl;
		}
		else if (casillaTablero == CALAVERA) {
			casillaJ = CASILLA_PARTIDA - 1;
			cout << "MUERTE!!! VUELVES A EMPEZAR" << endl;
			cout << "HAS CAIDO EN LA MUERTE Y VUELVES A EMPEZAR. VAS A LA CASILLA: " << casillaJ + 1 << endl;
		}
		if (turnosPerdidos) {
			penalizacionJ += turnosPerdidos;
			cout << "PIERDES " << turnosPerdidos << " TURNOS" << endl;
		}
	}
}

void pintaTablero(const tTablero tablero, const tJugadores casillasJ) {
	int casillasPorFila = NUM_CASILLAS / NUM_FILAS;
	cout << endl;
	for (int fila = 0; fila < NUM_FILAS; fila++) {
		pintaBorde(casillasPorFila);
		pintaNumCasilla(fila, casillasPorFila);
		pintaTipoCasilla(tablero, fila, casillasPorFila);
		pintaJugadores(casillasJ, fila, casillasPorFila);
	}
	pintaBorde(casillasPorFila);
	cout << endl;
}


void pintaBorde(int casillasPorFila) {
	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|====";
	cout << "|" << endl;
}


void pintaNumCasilla(int fila, int casillasPorFila) {
	for (int i = 1; i <= casillasPorFila; i++)
		cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";
	cout << "|" << endl;
}


void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {
	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|" << setw(4) << setfill(' ') << enumToString(tablero[i - 1 + fila * casillasPorFila]);
	cout << "|" << endl;
}

void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila) {
	int casilla;
	int blancos = NUM_JUGADORES_MAX - NUM_JUGADORES;
	string b = "";
	for (int i = 0; i < blancos; i++) b = b + " ";
	cout << "|";
	for (int i = 1; i <= casillasPorFila; i++) {
		casilla = i - 1 + fila * casillasPorFila;
		for (int jug = 0; jug < NUM_JUGADORES; jug++) {
			if (casillasJ[jug] == casilla)
				cout << jug + 1;
			else
				cout << " ";
		}
		cout << b;
		cout << "|";
	}
	cout << endl;
}

void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		casillasJ[i] = 0;
		penalizacionesJ[i] = 0;
	}
}

void iniciaTablero(tTablero tablero) {
	for (int i = 0; i < NUM_CASILLAS; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo == NUM_JUGADORES ? jugadorActivo = 1 : jugadorActivo++;
	cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo << endl;
}

tCasilla stringToEnum(string str) {
	tCasilla newEnum = NORMAL;
	if (str == "OCA") newEnum = OCA;
	else if (str == "PUENTE1") newEnum = PUENTE1;
	else if (str == "PUENTE2") newEnum = PUENTE2;
	else if (str == "POZO") newEnum = POZO;
	else if (str == "POSADA") newEnum = POSADA;
	else if (str == "LABERINTO") newEnum = LABERINTO;
	else if (str == "DADO1") newEnum = DADO1;
	else if (str == "DADO2") newEnum = DADO2;
	else if (str == "CARCEL") newEnum = CARCEL;
	else if (str == "CALAVERA") newEnum = CALAVERA;
	return newEnum;
}

string enumToString(tCasilla casilla) {
	string cadena;
	switch (casilla) {
	case NORMAL:
		cadena = " ";
		break;
	case OCA:
		cadena = "OCA";
		break;
	case DADO1:
	case DADO2:
		cadena = "DADO";
		break;
	case PUENTE1:
	case PUENTE2:
		cadena = "PNTE";
		break;
	case POSADA:
		cadena = "PSDA";
		break;
	case CALAVERA:
		cadena = "MUER";
		break;
	case LABERINTO:
		cadena = "LBRN";
		break;
	case POZO:
		cadena = "POZO";
		break;
	case CARCEL:
		cadena = "CRCL";
		break;
	}
	return cadena;
}
