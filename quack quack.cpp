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
const int NUM_JUGADORES = 2; //Elegir aquí el numero de jugadores con los que quieres jugar.
const int NUM_JUGADORES_MAX = 4;
const int MAX_PARTIDAS = 10;
const int CENTINELA = 0;
const int PARTIDA_NO_FINALIZADA = -1;
const int NUM_FILAS = 3;
const int NUM_COLUMNAS = NUM_CASILLAS / NUM_FILAS;
const bool MODO_DEBUG = true;

//Casillas especiales
typedef enum {
	NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA,
	LABERINTO, DADO1, DADO2, CARCEL, CALAVERA
} tCasilla;

//Estructura de cada jugador.
struct tEstadoJugador {
	int casilla;
	int penalizaciones;
};

//Array jugadores con casillas y penalizaciones.
typedef tEstadoJugador tEstadoJugadores[NUM_JUGADORES];

//Estructura de la partida.
struct tEstadoPartida {
	tEstadoJugadores estadoJug;
	tTablero tablero;
	int turno;
};

//Array de posibles partidas.
typedef tEstadoPartida tPartidas[MAX_PARTIDAS];

//Estructura posibles partidas con un indicador de cuantas estan disponibles para jugar.
struct tListaPartidas {
	tPartidas partidas;
	int contador;
};

//Tablero de juego.
typedef tCasilla tTablero[NUM_CASILLAS];



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
   * Comprueba si un tablero es valido o no
   * @example
   * // Expected output: true
   * validarTablero([NORMAL, OCA, NORMAL, OCA]);
   *
   * // Expected output: false
   * // Porque PUENTE2 no puede estar antes que PUENTE1
   * validarTablero([PUENTE2, PUENTE1, NORMAL, OCA]);
*/
bool validarTablero(const tTablero tablero);

/**
   * Pide por pantalla el nombre del fichero para abrir y devuelve un boolean conforme se pudo abrir o no
*/
void cargaTablero(tTablero tablero, ifstream& archivo);

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
int partida(tEstadoPartida& estado);

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
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);

/**
   * Determina el efecto de la casilla en la que ha caido el jugador y cambia los valores de casillaJ y penalizacionJ de ser necesario
*/
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);

/**
   * Inicializa los arrays de casillasJ y penalizacionesJ de acorde a NUM_JUGADORES
*/
void iniciaJugadores(tEstadoJugadores jugadores);

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

bool cargaPartidas(tListaPartidas& partidas);
void eliminarPartida(tListaPartidas& partidas, int indice);
bool insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca);
void guardaPartidas(const tListaPartidas& partidas);
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo);

void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);


//-------------------------------------------------------------------------
// Subprogramas para pintar el tablero o mostrar informaci�n
void pintaTablero(const tEstadoPartida& partida); 
// y sus auxiliares
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);
string casillaAstring(tCasilla casilla);

//---------------------------------------------------------------------------


int seleccionadorPartidasExistentes(const tListaPartidas& partidas);

int main() {
	srand(time(NULL));
	tListaPartidas partidas;
	char seleccionPartida;
	
	if (cargaPartidas(partidas)) {
		cout << "Fichero cargado correctamente. \n\n" << "Desea jugar una partida nueva o existente [N=nueva / E=existente]";
		cin >> seleccionPartida;
		if (seleccionPartida == 'N' || seleccionPartida == 'n') {
			cout << "Vas a jugar una partida nueva." << endl;
			tEstadoPartida partidaNueva;
			cout << "Indica el fichero que contiene el tablero de la oca: ";
			partidaNueva.turno = quienEmpieza();
			int ganador = partida(partidaNueva);
			if (ganador == PARTIDA_NO_FINALIZADA) {
				insertaNuevaPartida(partidas, partidaNueva);
				guardaPartidas(partidas);
			}
			else {
				cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
			}

		}
		else if (seleccionPartida == 'E' || seleccionPartida == 'e') {
			int partidaSeleccionada = seleccionadorPartidasExistentes(partidas);
			int ganador = partida(partidas.partidas[partidaSeleccionada - 1]);
			if (ganador != PARTIDA_NO_FINALIZADA) {
				cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
				cout << "La partida " << partidaSeleccionada << " ha terminado. Se elimina de la lista de partidas. ";
				eliminarPartida(partidas, partidaSeleccionada);

			}
			guardaPartidas(partidas);
		} 

	}
	else {
		cout << "No pude abrir el archivo o el tablero es invalido";
	}
	return 0;
}

int seleccionadorPartidasExistentes(const tListaPartidas& partidas) {
	int partidaSeleccionada;
	cout << "Los identificadoes disponibles son: ";
	for (int i = 1; i <= partidas.contador; i++) { cout << i << " "; }
	cout << "Que partida quieres continuar? ";
	cin >> partidaSeleccionada;
	return partidaSeleccionada;
}

void iniciaJugadores(tEstadoJugadores jugadores) {
	tEstadoJugador jugadorAux;
	jugadorAux.casilla = 0;
	jugadorAux.penalizaciones = 0;
	for (int i = 0; i < NUM_JUGADORES; i++) {
		jugadores[i] = jugadorAux;
	}
}

void iniciaTablero(tTablero tablero) {
	for (int i = 0; i < NUM_CASILLAS; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}
void cargaTablero(tTablero tablero, ifstream& archivo) {
	ifstream archivo;
	string name;
	
	bool valido = false;
	cout << "Que archivo quieres abrir? ";
	cin >> name;
	archivo.open(name);
	valido = archivo.is_open();
	if (valido) {
		int numCasilla;
		string tipoCasilla;
		archivo >> numCasilla;
		while (numCasilla != CENTINELA) {
			archivo >> tipoCasilla;
			if (numCasilla < NUM_CASILLAS) tablero[numCasilla - 1] = stringToEnum(tipoCasilla);
			archivo >> numCasilla;
		}
		archivo.close();
	}
}

void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		archivo >> jugadores[i].casilla;
		archivo >> jugadores[i].penalizaciones;
	}
}

bool cargaPartidas(tListaPartidas& partidas) {
	ifstream archivo;
	string name;

	bool valido = false;
	cout << "Nombre del archivo que contiene las partidas: ";
	cin >> name;
	archivo.open(name);
	valido = archivo.is_open();
	if (valido) {
		archivo >> partidas.contador;
		for (int i = 0; i < partidas.contador; i++) {
			cargaTablero(partidas.partidas[i].tablero, archivo);
			archivo >> partidas.partidas[i].turno;
			cargaJugadores(partidas.partidas[i].estadoJug, archivo);
		}
		archivo.close();
	}
	return valido;
}



void eliminarPartida(tListaPartidas& partidas, int indice) {
	for (int i = indice - 1; i < partidas.contador; i++) {
		partidas.partidas[i] = partidas.partidas[i + 1];
	}
	partidas.contador--;
}

bool insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca) {
	bool pudoInsertarse = false;
	if (partidas.contador < MAX_PARTIDAS) {
		partidas.partidas[partidas.contador] = partidaOca;
		partidas.contador++;
		pudoInsertarse = true;
	}
	return pudoInsertarse;
}

void guardaPartidas(const tListaPartidas& partidas) {
	ofstream archivo;
	string nombre;
	cout << "Indique en que archivo desea guardar las partidas: ";
	cin >> nombre;
	archivo.open(nombre);
	if (archivo.is_open()) {
		archivo << partidas.contador << endl;
		for (int i = 0; i < partidas.contador; i++) {
			guardaTablero(partidas.partidas[i].tablero, archivo);
			archivo << partidas.partidas[i].turno;
			guardaJugadores(partidas.partidas[i].estadoJug, archivo);
		}
		archivo.close();
		cout << "La partida se ha guardado correctamente en " << nombre;
	}
	else {
		cout << "No se encontro el archivo deseado. \n";
	}
}

void guardaTablero(const tTablero tablero, ofstream& archivo) {
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		if (tablero[i] != NORMAL) {
			archivo << i + 1 << " " << enumToString(tablero[i]) << endl;
		}
	}
	archivo << "0" << endl;
}

void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		archivo << jugadores[i].casilla << " " << jugadores[i].penalizaciones << endl;
	}
}



bool esCasillaPremio(const tTablero tablero, int casilla) {
	int casillaTablero = tablero[casilla];
	return casillaTablero == OCA || casillaTablero == DADO1 || casillaTablero == DADO2 || casillaTablero == PUENTE1 || casillaTablero == PUENTE2;
}

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS - 1;
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
	return 1 + (rand() % (NUM_JUGADORES));
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

int partida(tEstadoPartida& estado) {
	bool continuarPartida = true, ganado = false;
	char caracterContinuarPartida;
	pintaTablero(estado);
	cout << endl << "**** EMPIEZA EL JUGADOR " << estado.turno  << " ****" << endl;
	while (!esMeta(estado.estadoJug[estado.turno - 1].casilla) && continuarPartida) {
		int penalizacion = estado.estadoJug[estado.turno - 1].penalizaciones;
		if (!estado.estadoJug[estado.turno - 1].penalizaciones) {
			do {
				tirada(estado.tablero, estado.estadoJug[estado.turno - 1]);
				pintaTablero(estado);
			} while (esCasillaPremio(estado.tablero, estado.estadoJug[estado.turno - 1].casilla) && !esMeta(estado.estadoJug[estado.turno - 1].casilla));

			if (!esMeta(estado.estadoJug[estado.turno - 1].casilla)) {
				cambioTurno(estado.turno);
			}
			else ganado = true;
		}
		else {
			cout << "... PERO NO PUEDE " << (penalizacion > 1 ? "Y LE QUEDAN " + to_string(penalizacion) + " TURNOS SIN JUGAR" : "HASTA EL SIGUIENTE TURNO") << endl;
			estado.estadoJug[estado.turno - 1].penalizaciones -= 1;
			cambioTurno(estado.turno);
		}
		if(!ganado)
		cout << "Si quieres abandonar pulse la A. Para continuar pulse cualquier otra tecla... ";
		cin >> caracterContinuarPartida;
		if (caracterContinuarPartida == 'A' || caracterContinuarPartida == 'a') continuarPartida = false;
	}
	if (continuarPartida) {
		cout << "** FIN DEL JUEGO **" << endl; 
		pintaTablero(estado);
	}
	else cout << "PARTIDA SIN ACABAR" << endl;
	
	return continuarPartida ? estado.turno : PARTIDA_NO_FINALIZADA; // se devuelve el jugador que ha ganado la partida (ya sin tener en cuenta la posicion del array (0 -> 1, 1 -> 2, etc.))
}

void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion) {
	bool encontrado = false;
	int i = posicion + 1;
	while (!encontrado && i < NUM_CASILLAS) {
		if (tablero[i] == tipo) {
			posicion = i;
			encontrado = true;
		}
		i++;
	}
}

void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion) {
	bool encontrado = false;
	int i = posicion - 1;
	while (!encontrado && i >= 0) {
		if (tablero[i] == tipo) {
			posicion = i;
			encontrado = true;
		}
		i--;
	}
}

void tirada(const tTablero tablero, tEstadoJugador& estadoJug) {
	cout << "CASILLA ACTUAL: " << estadoJug.casilla + 1 << endl;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	int valorDado = (MODO_DEBUG ? tirarDadoManual() : tirarDado());
	cout << "VALOR DEL DADO: " << valorDado << endl;
	estadoJug.casilla += valorDado;
	cout << "PASAS A LA CASILLA " << estadoJug.casilla + 1 << endl;
	if (!esMeta(estadoJug.casilla)) {
		efectoTirada(tablero, estadoJug);
	}
}

void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug) {
	tCasilla casillaTablero = tablero[estadoJug.casilla];
	if (esCasillaPremio(tablero, estadoJug.casilla)) {
		estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
		if (casillaTablero == OCA) {
			cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA" << endl;
			cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << estadoJug.casilla + 1 << endl;
		}
		else if (casillaTablero == DADO1 || casillaTablero == DADO2) {
			cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
			cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << estadoJug.casilla + 1 << endl;
		}
		else if (casillaTablero == PUENTE1 || casillaTablero == PUENTE2) {
			cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
			cout << "SALTAS AL PUENTE EN LA CASILLA: " << estadoJug.casilla + 1 << endl;
		}
		if (!esMeta(estadoJug.casilla)) cout << "Y VUELVES A TIRAR" << endl;
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
			estadoJug.casilla -= PENALIZACION_LABERINTO;
			cout << "CAES EN EL LABERINTO" << endl;
			cout << "SALTAS A LA CASILLA " << estadoJug.casilla + 1 << endl;
		}
		else if (casillaTablero == CALAVERA) {
			estadoJug.casilla = CASILLA_PARTIDA - 1;
			cout << "MUERTE!!! VUELVES A EMPEZAR" << endl;
			cout << "HAS CAIDO EN LA MUERTE Y VUELVES A EMPEZAR. VAS A LA CASILLA: " << casillaJ + 1 << endl;
		}
		if (turnosPerdidos) {
			estadoJug.penalizaciones += turnosPerdidos;
			cout << "PIERDES " << turnosPerdidos << " TURNOS" << endl;
		}
	}
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo = (jugadorActivo  % NUM_JUGADORES) + 1;
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

// Subprograma para pintar el tablero...
void pintaTablero(const tEstadoPartida& partida) {

	int numFilas = 3;
	int casillasPorFila = NUM_CASILLAS / numFilas; // Es 21


	cout << endl;
	for (int fila = 0; fila < numFilas; fila++) {
		pintaBorde(casillasPorFila);
		pintaNumCasilla(fila, casillasPorFila);
		pintaTipoCasilla(partida.tablero, fila, casillasPorFila);
		pintaJugadores(partida.estadoJug, fila, casillasPorFila);
	}

	pintaBorde(casillasPorFila);
	cout << endl;


}

// y sus auxiliares

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
		cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);
	cout << "|" << endl;
}

void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila) {
	int casilla;
	int blancos = 4 - NUM_JUGADORES;
	string b = "";
	for (int i = 0; i < blancos; i++) b = b + " ";
	cout << "|";
	for (int i = 1; i <= casillasPorFila; i++) {
		casilla = i - 1 + fila * casillasPorFila;
		for (int jug = 0; jug < NUM_JUGADORES; jug++) {
			if (estadosJ[jug].casilla == casilla)
				cout << jug + 1;
			else
				cout << " ";
		}
		cout << b;
		cout << "|";
	}
	cout << endl;

}

string casillaAstring(tCasilla casilla) {
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
