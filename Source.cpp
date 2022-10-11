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
	
	return 0;
}

bool esOca(int casilla) {

}

bool esPuente(int casilla) {
	
}

bool esDados(int casilla) {
	
}

bool esLaberinto(int casilla) {
	
}

bool esMuerte(int casilla) {
	
}

bool esPosada(int casilla) {
	
}

bool esPrision(int casilla) {
	
}

bool esPozo(int casilla) {
	
}

bool esMeta(int casilla) {
	
}

int siguienteOca(int casilla) {
	
}

int siguientePuente(int casilla) {
	
}

int siguienteDado(int casilla) {
	
}

int siguienteLaberinto(int casilla) {
	
}

int siguienteMuerte(int casilla) {
	
}

int tirarDado() {
	
}

int quienEmpieza() {
	
}
