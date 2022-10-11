#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int NUM_CASILLAS = 63;
const int CASILLA_PARTIDA = 1;
const int CASILLA_PUENTE_1 = 6;
const int CASILLA_PUENTE_2 = 12;
const int CASILLA_DADOS_1 = 26;
const int CASILLA_DADOS_2 = 53;
const int CASILLA_POSADA = 19;
const int CASILLA_PRISION = 52;
const int CASILLA_POZO = 31;
const int CASILLA_LABERINTO = 42;
const int CASILLA_MUERTE = 58;
const int TURNOS_POSADA = 1;
const int TURNOS_PRISION = 1;
const int TURNOS_PUENTE = 3;
const int PENALIZACION_LABERINTO = 12;
const int DADO_MAXIMO = 6;
const int DADO_MINIMO = 1;
const int NUMERO_JUGADORES_MIN = 1;
const int NUMERO_JUGADORES_MAX = 2;

bool esOca(int casilla);
bool esPuente(int casilla);
bool esDados(int casilla);
bool esLaberinto(int casilla);
bool esMuerte(int casilla);
bool esPosada(int casilla);
bool esPrision(int casilla);
bool esPozo(int casilla);
bool esMeta(int casilla);

int siguienteOca(int casilla);
int siguientePuente(int casilla);
int siguienteDado(int casilla);
int siguienteLaberinto(int casilla);
int siguienteMuerte(int casilla);

int tirarDado();
int quienEmpieza();
int efectoPosicion(int casillaActual);
int efectoTiradas(int casillaActual, int numeroDeTiradas);
int main() {
	srand(time(NULL));
	cout << siguienteOca(5);
	return 0;
}

bool esOca(int casilla) {
	// 5, 9, 14, 18, 23, 27, 32, 36, 41, 45, 50, 54, 59, 63.
	return (casilla % 9 == 0 || casilla % 9 == 5);
}

bool esPuente(int casilla) {
	return casilla == CASILLA_PUENTE_1 || casilla == CASILLA_PUENTE_2;
}

bool esDados(int casilla) {
	return casilla == CASILLA_DADOS_1 || casilla == CASILLA_DADOS_2;
}

bool esLaberinto(int casilla) {
	return casilla == CASILLA_LABERINTO;
}

bool esMuerte(int casilla) {
	return casilla == CASILLA_MUERTE;
}

bool esPosada(int casilla) {
	return casilla == CASILLA_POSADA;
}

bool esPrision(int casilla) {
	return casilla == CASILLA_PRISION;
}

bool esPozo(int casilla) {
	return casilla == CASILLA_POZO;
}

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS;
}

int siguienteOca(int casilla) {
	return casilla + (casilla % 9 == 0 ? 5 : 4);
}

int siguientePuente(int casilla) {
	return casilla == CASILLA_PUENTE_1 ? CASILLA_PUENTE_1 : CASILLA_PUENTE_2;
}

int siguienteDado(int casilla) {
	return casilla == CASILLA_DADOS_1 ? CASILLA_DADOS_1 : CASILLA_DADOS_2;
}

int siguienteLaberinto(int casilla) {
	return casilla - PENALIZACION_LABERINTO;
}

int siguienteMuerte(int casilla) {
	return CASILLA_PARTIDA;
}

int tirarDado() {
	return DADO_MINIMO + rand() % (DADO_MAXIMO + 1 - DADO_MINIMO);
}

int quienEmpieza() {
	return NUMERO_JUGADORES_MIN + rand() % (NUMERO_JUGADORES_MAX + 1 - NUMERO_JUGADORES_MIN);
}

int efectoPosicion(int casillaActual) {
	int casillaNueva;
	if (esOca(casillaActual)) {
		casillaNueva = siguienteOca(casillaActual);
		//tirar dados
	}
	else if (esPuente(casillaActual)) {
		casillaNueva = siguientePuente(casillaActual);
		//tirar dados
	}
	else if (esDados(casillaActual)) {
		casillaNueva = siguienteDado(casillaActual);
		//tirar dados
	}
	else if (esLaberinto(casillaActual)) {
		//laberinto 
	}
	else if (esPosada(casillaActual)) {
		//posada
	}
	else if (esPozo(casillaActual)) {
		//pozo
	}
	else if (esMeta(casillaActual)) {
		//meta
	}
	return casillaNueva;
}