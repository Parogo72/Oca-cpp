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
const bool MODO_DEBUGS = true;

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
	int turno = quienEmpieza();
	int casillaJugador1 = CASILLA_PARTIDA;
	int casillaJugador2 = CASILLA_PARTIDA;
	int tiradasJugador1 = 1;
	int tiradasJugador2 = 1;
	int casillaPrincipal;
	int tiradasPrincipal;
	int tiradasContrario;
	cout << "**** EMPIEZA EL JUGADOR: " << turno << "****" << endl;
	do {
		if (turno == 1) {
			casillaPrincipal = casillaJugador1;
			tiradasPrincipal = tiradasJugador1;
			tiradasContrario = tiradasJugador2;
		}
		else if (turno == 2) {
			casillaPrincipal = casillaJugador2;
			tiradasPrincipal = tiradasJugador2;
			tiradasContrario = tiradasJugador1;
		}
		//(tengas tiradas o el contrario no tenga tiradas) y no hayas llegado a la meta
		while ((tiradasPrincipal >= tiradasContrario) && casillaPrincipal < NUM_CASILLAS) {
			cout << "CASILLA ACTUAL: " << casillaPrincipal << endl;
			cout << tiradasPrincipal << " " << tiradasContrario << endl;
			casillaPrincipal += (MODO_DEBUGS ? tirarDadoManual() : tirarDado());
			// se te quita la posibilidad a tirar
			if (tiradasPrincipal > 0) {
				tiradasPrincipal--;
			}
			tiradasPrincipal = efectoTiradas(casillaPrincipal, tiradasPrincipal); //a no ser que hayas caido en una casilla especial
			if (tiradasContrario < 1 && tiradasPrincipal < 1) {
				tiradasContrario++;
			}
			cout << tiradasPrincipal << " " << tiradasContrario << endl;
			casillaPrincipal = efectoPosicion(casillaPrincipal);
		}

		if (casillaPrincipal >= NUM_CASILLAS) {
			cout << "------ GANA EL JUGADOR " << turno << " ------" << endl;
		}
		else {
			if (turno == 1) {
				casillaJugador1 = casillaPrincipal;
				tiradasJugador1 = tiradasPrincipal;
				tiradasJugador2 = tiradasContrario;
				turno = 2;
			}
			else if (turno == 2) {
				casillaJugador2 = casillaPrincipal;
				tiradasJugador2 = tiradasPrincipal;
				tiradasJugador1 = tiradasContrario;
				turno = 1;
			}
			cout << endl << "TURNO PARA EL JUGADOR " << turno << endl;
		}
	} while (casillaPrincipal < NUM_CASILLAS);
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
	int casillaNueva;

	cout << "PASAS A LA CASILLA " << casillaActual << endl;

	if (casillaActual >= NUM_CASILLAS) {
		casillaNueva = casillaActual;
	}
	else if (esOca(casillaActual)) {
		casillaNueva = siguienteOca(casillaActual);
		cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << casillaNueva << endl;
		cout << "VUELVES A TIRAR" << endl;
	}
	else if (esPuente(casillaActual)) {
		casillaNueva = siguientePuente(casillaActual);
		cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
		cout << "SALTAS AL PUENTE EN LA CASILLA: " << casillaNueva << endl;
		cout << "VUELVES A TIRAR" << endl;
	}
	else if (esDados(casillaActual)) {
		casillaNueva = siguienteDado(casillaActual);
		cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
		cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << casillaNueva << endl;
		cout << "VUELVES A TIRAR" << endl;
	}
	else if (esLaberinto(casillaActual)) {
		casillaNueva = siguienteLaberinto(casillaActual);
		cout << "HAS CAIDO EN EL LABERINTO" << endl;
		cout << "RETROCEDES A LA CASILLA " << casillaNueva << endl;
	}
	else if (esPosada(casillaActual)) {
		casillaNueva = casillaActual;
		cout << "HAS CAIDO EN LA POSADA" << endl;
		cout << "PIERDES " << TURNOS_POSADA << " TURNOS" << endl;
	}
	else if (esPrision(casillaActual)) {
		casillaNueva = casillaActual;
		cout << "HAS CAIDO EN LA PRISION" << endl;
		cout << "PIERDES " << TURNOS_PRISION << " TURNOS" << endl;
	}
	else if (esPozo(casillaActual)) {
		casillaNueva = casillaActual;
		cout << "HAS CAIDO EN EL POZO" << endl;
		cout << "PIERDES " << TURNOS_POZO << " TURNOS" << endl;
	}
	else if (esMuerte(casillaActual)) {
		casillaNueva = siguienteMuerte(casillaActual);
		cout << "HAS CAIDO EN LA MUERTE" << endl;
		cout << "VUELVES A LA CASILLA " << casillaNueva << endl;
	}
	else {
		casillaNueva = casillaActual;
	}

	return casillaNueva;
}

int efectoTiradas(int casillaActual, int numeroDeTiradas) {
	if (esOca(casillaActual)) {
		numeroDeTiradas++;
	}
	else if (esPuente(casillaActual)) {
		numeroDeTiradas++;
	}
	else if (esDados(casillaActual)) {
		numeroDeTiradas++;
	}
	else if (esPosada(casillaActual)) {
		numeroDeTiradas -= TURNOS_POSADA;
	}
	else if (esPrision(casillaActual)) {
		numeroDeTiradas -= TURNOS_PRISION;
	}
	else if (esPozo(casillaActual)) {
		numeroDeTiradas -= TURNOS_POZO;
	}
	return numeroDeTiradas;
}
