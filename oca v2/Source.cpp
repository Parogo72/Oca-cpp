﻿#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

const int NUM_CASILLAS = 63;
const int CASILLA_PARTIDA = 1;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int PENALIZACION_LABERINTO = 12;
const int DADO_MAXIMO = 6;
const int DADO_MINIMO = 1;
const int NUM_JUGADORES = 4;
const bool MODO_DEBUGS = true;

typedef enum {
	NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA,
	LABERINTO, DADO1, DADO2, CARCEL, CALAVERA
} tCasilla;
typedef tCasilla tTablero[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];

bool esCasillaPremio(const tTablero tablero, int casilla);
bool esMeta(int casilla);
bool cargaTablero(tTablero tablero);

int tirarDado();
int tirarDadoManual();
int quienEmpieza();
int saltaACasilla(const tTablero tablero, int casillaActual);
int partida(const tTablero tablero);

void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);
void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);
void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
void iniciaTablero(tTablero tablero);
void cambioTurno(int& jugadorActivo);

tCasilla stringToEnum(string str);
string enumToString(tCasilla type);

int main() {
	tTablero tablero;
	srand(time(NULL));
	
	iniciaTablero(tablero);
	if (cargaTablero(tablero)) {
		int ganador = partida(tablero);
		cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
	}
	else {
		cout << "opss";
	}
	return 0;
}

bool esCasillaPremio(const tTablero tablero, int casilla) {
	int casillaTablero = tablero[casilla];
	return casillaTablero == OCA || casillaTablero == DADO1 || casillaTablero == DADO2 || casillaTablero == PUENTE1 || casillaTablero == PUENTE2;
}

bool esMeta(int casilla) {
	return casilla == NUM_CASILLAS - 1;
}

bool cargaTablero(tTablero tablero) {
	fstream archivo;
	string name;
	bool opened = false, ended = false;
	/*cout << "Que archivo quieres abrir? ";
	cin >> name;
	archivo.open(name);*/
	archivo.open("tablero.txt");
	opened = archivo.is_open();
	if (opened) {
		while (!ended) {
			int numCasilla;
			string tipoCasilla;
			archivo >> numCasilla;
			archivo >> tipoCasilla;
			if (numCasilla == 0) {
				ended = true;
			}
			else {
				tablero[numCasilla - 1] = stringToEnum(tipoCasilla);
			}
		}
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
	return 2 + rand() % (NUM_JUGADORES + 1 - 2);
}

int saltaACasilla(const tTablero tablero, int casillaActual) {
	tCasilla casillaTablero = tablero[casillaActual];
	if (casillaTablero == OCA) {
		buscaCasillaAvanzando(tablero, OCA, casillaActual);
	}
	else if (casillaTablero == DADO1) {
		buscaCasillaAvanzando(tablero, DADO2, casillaActual);
	} 
	else if(casillaTablero == PUENTE1) {
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
	int ultimoEnTirar = jugadorActivo;
	bool ended = false;

	pintaTablero(tablero, casillasJ);
	iniciaJugadores(casillasJ, penalizacionJ);
	cout << "Comienza el juego" << endl;
	cout << endl << "**** EMPIEZA EL JUGADOR " << jugadorActivo << " ****"<< endl;
	while (!esMeta(casillasJ[jugadorActivo - 1])) {
		int penalizacion = penalizacionJ[jugadorActivo - 1];
		if (!penalizacion) {
			if (ultimoEnTirar != jugadorActivo) {
				cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo << endl;
			}
			ultimoEnTirar = jugadorActivo;
			do {
				tirada(tablero, casillasJ[jugadorActivo - 1], penalizacionJ[jugadorActivo - 1]);
				pintaTablero(tablero, casillasJ);
			} while (esCasillaPremio(tablero, casillasJ[jugadorActivo - 1]) && !esMeta(casillasJ[jugadorActivo - 1]));
				
			if (!esMeta(casillasJ[jugadorActivo - 1])) {
				if (jugadorActivo == NUM_JUGADORES) jugadorActivo = 1;
				else jugadorActivo++;
			}
			else {
				ended = true;
			}
		}
		else {
			penalizacionJ[jugadorActivo - 1] -= 1;
			if (jugadorActivo == NUM_JUGADORES) jugadorActivo = 1;
			else jugadorActivo++;
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
	while (!found && i == 0) {
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
	if (esCasillaPremio(tablero, casillaJ) && !esMeta(casillaJ)) {
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
			cout << "HAS CAIDO EN EL LABERINTO" << endl;
			cout << "RETROCEDES A LA CASILLA " << casillaJ + 1 << endl;
		}
		else if (casillaTablero == CALAVERA) {
			casillaJ = CASILLA_PARTIDA;
			cout << "HAS CAIDO EN LA MUERTE" << endl;
			cout << "VUELVES A LA CASILLA " << casillaJ + 1 << endl;
		}
		if (turnosPerdidos) {
			penalizacionJ += turnosPerdidos;
			cout << "PIERDES " << turnosPerdidos << " TURNOS" << endl;
		}
	}
}

void pintaTablero(const tTablero tablero, const tJugadores casillasJ) {
	string linea1 = "|";
	cout << endl;
	for (int i = 1; i <= 3; i++) {
		string linea2 = "|";
		string linea3 = "|";
		string linea4 = "|";
		for (int j = 21 * (i - 1) + 1; j <= 21 * i; j++) {
			tCasilla type = tablero[j - 1];
			string tipo;
			if (i == 1) linea1 += "====|";
			linea2 += (j < 10 ? " 0" + to_string(j) + " " : " " + to_string(j) + " ") + "|";
			linea3 += enumToString(type) + "|";
			for (int k = 0; k < NUM_JUGADORES; k++) {
				linea4 += casillasJ[k] == j - 1 ? to_string(k + 1) : " ";
			}
			linea4 += "|";
		}
		cout << linea1 << endl << linea2 << endl << linea3 << endl << linea4 << endl;
	}
	cout << linea1 << endl << endl;
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
	if (jugadorActivo == NUM_JUGADORES) jugadorActivo = 1;
	else jugadorActivo++;
}

tCasilla stringToEnum(string str) {
	tCasilla newEnum;
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

string enumToString(tCasilla type) {
	string newString;
	if (type == OCA) newString = " OCA";
	else if (type == PUENTE1) newString = "PNTE";
	else if (type == PUENTE2) newString = "PNTE";
	else if (type == POZO) newString = "POZO";
	else if (type == POSADA) newString = "PSDA";
	else if (type == LABERINTO) newString = "LBRN";
	else if (type == DADO1) newString = "DADO";
	else if (type == DADO2) newString = "DADO";
	else if (type == CARCEL) newString = "CRCL";
	else if (type == CALAVERA) newString = "MUER";
	else if (type == NORMAL) newString = "    ";
	return newString;
}