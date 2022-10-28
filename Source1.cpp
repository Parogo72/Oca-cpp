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
const int TURNOS_PRISION = 2;
const int TURNOS_POZO = 3;
const int PENALIZACION_LABERINTO = 12;
const int DADO_MAXIMO = 6;
const int DADO_MINIMO = 1;
const int NUMERO_JUGADORES_MIN = 1;
const int NUMERO_JUGADORES_MAX = 2;
const bool MODO_DEBUGS = false;

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
int tirarDadoManual();
int quienEmpieza();
int efectoPosicion(int casillaActual);
int efectoTiradas(int casillaActual, int numeroDeTiradas);

int main() {
	srand(time(NULL));
	int jugadorActivo = quienEmpieza(); // Vemos que jugador empieza. Utilizaremos esta variable para saber el numero del jugador activo
	// Definimos variables para cada jugador
	int casillaPrincipal = CASILLA_PARTIDA;
	int casillaContrario = CASILLA_PARTIDA;
	int tiradasPrincipal = 1;
	int tiradasContrario = 1;
	// Esta variable nos deja saber si estamos al comienzo de un turno. Es decir, si empieza el jugador 2, si estamos en el jugador 2.
	bool comienzoRonda = true;

	cout << "**** EMPIEZA EL JUGADOR: " << jugadorActivo << "****" << endl;
	do {
		// Si ambos no tienen tiradas, se a�aden tiros
		if (tiradasPrincipal < 1 && tiradasContrario < 1) {
			tiradasPrincipal++;
			tiradasContrario++;
		}
		// Creamos unas variables auxiliares para referenciar al jugador activo.
		int casillaAux = casillaPrincipal;
		int tiradasAux = tiradasPrincipal;
		if (!comienzoRonda) {
			casillaAux = casillaContrario;
			tiradasAux = tiradasContrario;
		}

		cout << "CASILLA ACTUAL: " << casillaAux << endl;
		casillaAux += (MODO_DEBUGS ? tirarDadoManual() : tirarDado()); // Tiramos dado manual o normal
		tiradasAux--; // Restamos la tirada usada
		casillaAux = efectoPosicion(casillaAux);
		tiradasAux = efectoTiradas(casillaAux, tiradasAux);
		// Nos deshacemos de la variable auxiliar
		if (comienzoRonda) {
			casillaPrincipal = casillaAux;
			tiradasPrincipal = tiradasAux;
		}
		else {
			casillaContrario = casillaAux;
			tiradasContrario = tiradasAux;
		}
		if (esMeta(casillaPrincipal) || esMeta(casillaContrario)) {
			cout << "------ GANA EL JUGADOR " << jugadorActivo << " ------" << endl;
		}
		// Si aun estamos en el comienzo de la ronda y el contrario tiene m�s tiradas (Nosotros tenemos 0 o turnos negativos), cambiamos turno
		else if (comienzoRonda && tiradasPrincipal < tiradasContrario) {
			jugadorActivo = 2;
			comienzoRonda = !comienzoRonda;
			cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo << endl;
		}
		// Si estamos al final de la ronda y tenemos m�s o igual numero de tiradas, cambiamos de turno (0 y 0, o penalizaci�n del contrario)
		else if (!comienzoRonda && tiradasContrario <= tiradasPrincipal) {
			jugadorActivo = 1;
			comienzoRonda = !comienzoRonda;
			cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo << endl;
		}
	} while (!esMeta(casillaPrincipal) && !esMeta(casillaContrario));
	return 0;
}

bool esOca(int casilla) {
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
	return casilla == CASILLA_PUENTE_1 ? CASILLA_PUENTE_2 : CASILLA_PUENTE_1;
}

int siguienteDado(int casilla) {
	return casilla == CASILLA_DADOS_1 ? CASILLA_DADOS_2 : CASILLA_DADOS_1;
}

int siguienteLaberinto(int casilla) {
	return casilla - PENALIZACION_LABERINTO;
}

int siguienteMuerte(int casilla) {
	return CASILLA_PARTIDA;
}

int tirarDado() {
	int valorDado = DADO_MINIMO + rand() % (DADO_MAXIMO + 1 - DADO_MINIMO);
	cout << "INTRODUCE EL VALOR DEL DADO: " << valorDado << endl;
	cout << "VALOR DEL DADO: " << valorDado << endl;
	return valorDado;
}

int tirarDadoManual() {
	int valorDado;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	cin >> valorDado;
	cout << "VALOR DEL DADO: " << valorDado << endl;
	return valorDado;
}

int quienEmpieza() {
	return NUMERO_JUGADORES_MIN + rand() % (NUMERO_JUGADORES_MAX + 1 - NUMERO_JUGADORES_MIN);
}

int efectoPosicion(int casillaActual) {
	int casillaNueva = casillaActual;
	cout << "PASAS A LA CASILLA " << casillaActual << endl;
	if (!esMeta(casillaActual)) {
		if (esOca(casillaActual)) {
			casillaNueva = siguienteOca(casillaActual);
			cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << casillaNueva << endl;
		}
		else if (esPuente(casillaActual)) {
			casillaNueva = siguientePuente(casillaActual);
			cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
			cout << "SALTAS AL PUENTE EN LA CASILLA: " << casillaNueva << endl;
		}
		else if (esDados(casillaActual)) {
			casillaNueva = siguienteDado(casillaActual);
			cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
			cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << casillaNueva << endl;
		}
		else if (esLaberinto(casillaActual)) {
			casillaNueva = siguienteLaberinto(casillaActual);
			cout << "HAS CAIDO EN EL LABERINTO" << endl;
			cout << "RETROCEDES A LA CASILLA " << casillaNueva << endl;
		}
		else if (esPosada(casillaActual)) {
			casillaNueva = casillaActual;
			cout << "HAS CAIDO EN LA POSADA" << endl;
		}
		else if (esPrision(casillaActual)) {
			casillaNueva = casillaActual;
			cout << "HAS CAIDO EN LA PRISION" << endl;
		}
		else if (esPozo(casillaActual)) {
			casillaNueva = casillaActual;
			cout << "HAS CAIDO EN EL POZO" << endl;
		}
		else if (esMuerte(casillaActual)) {
			casillaNueva = siguienteMuerte(casillaActual);
			cout << "HAS CAIDO EN LA MUERTE" << endl;
			cout << "VUELVES A LA CASILLA " << casillaNueva << endl;
		}
	}
	return casillaNueva;
}

int efectoTiradas(int casillaActual, int numeroDeTiradas) {
	int numeroDeTiradasNuevas = numeroDeTiradas;
	if (esMeta(casillaActual)) {
		numeroDeTiradasNuevas = numeroDeTiradas;
	}
	else if (esOca(casillaActual) || esPuente(casillaActual) || esDados(casillaActual)) {
		numeroDeTiradasNuevas++;
	}
	else if (esPosada(casillaActual)) {
		numeroDeTiradasNuevas -= TURNOS_POSADA;
	}
	else if (esPrision(casillaActual)) {
		numeroDeTiradasNuevas -= TURNOS_PRISION;
	}
	else if (esPozo(casillaActual)) {
		numeroDeTiradasNuevas -= TURNOS_POZO;
	}

	if (numeroDeTiradas > numeroDeTiradasNuevas) {
		cout << "PIERDES " << -numeroDeTiradasNuevas << " TURNOS" << endl;
	}
	else if (numeroDeTiradas < numeroDeTiradasNuevas) {
		cout << "VUELVES A TIRAR" << endl;
	}
	return numeroDeTiradasNuevas;
}