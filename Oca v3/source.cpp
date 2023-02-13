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
const int NUM_FILAS = 3;
const int NUM_COLUMNAS = NUM_CASILLAS / NUM_FILAS;
const bool MODO_DEBUGS = true;

typedef enum {
	NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA,
	LABERINTO, DADO1, DADO2, CARCEL, CALAVERA
} tCasilla;

typedef tCasilla tTablero[NUM_CASILLAS];

struct tEstadoJugador {
	int posicion = 0;
	int penalizacion = 0;
};

typedef tEstadoJugador tEstadoJugadores[NUM_JUGADORES];

struct tEstadoPartida {
	tEstadoJugadores jugadores;
	tTablero tablero;
	int jugadorActivo = 0;
};

typedef tEstadoPartida tEstadoPartidas[MAX_PARTIDAS];

struct tListaPartidas {
	tEstadoPartidas partidas;
	int contador = 0;
};

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
// Subprogramas que se heredan de la v1
int tirarDado();
int tirarDadoManual();
bool esMeta(int casilla);
int quienEmpieza();
void cambioTurno(int& jugadorActivo);


// Subprogramas para configurar el tablero heredados de la v2
bool cargaTablero(tTablero tablero);
void iniciaTablero(tTablero tablero);


// Subprogramas que sustituyen a otros implementados en la versi�n 1 
// relacionados con la identificaci�n de casillas especiales y con el efecto provocado por las mismas
bool esCasillaPremio(const tTablero tablero, int casilla);
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);
void efectoTirada(const tTablero tablero, tEstadoJugador& jugador);    // NUEVO PROTOTIPO EN V3; SUSTITUYE A    void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);

// Subprogramas para la l�gica de las tiradas y de la partida
void iniciaJugadores(tEstadoJugadores jugadores);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
int partida(tEstadoPartida& estado);  // NUEVO PROTOTIPO EN V3; SUSTITUYE A   int partida(const tTablero tablero);


// Subprogramas para la gestion de la lista de partidas abandonadas
bool cargaPartidas(tListaPartidas& partidas);
void cargaTablero(tTablero tablero, ifstream& archivo);
void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);
void eliminarPartida(tListaPartidas& partidas, int indice);
void insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca);
void guardaPartidas(const tListaPartidas& partidas);
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo);


// Subprograma para pintar el tablero...
void pintaTablero(const tEstadoPartida& partida);   //NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
// y sus auxiliares
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);

char menuTipoPartida();
int menuNumeroPartida(const tListaPartidas listaPartidas);
bool iniciaPartida(tEstadoPartida& estadoPartida);
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

string enumToStringCompleto(tCasilla casilla);

int main() {
	srand(time(NULL));
	tListaPartidas listaPartidas;
	tEstadoPartida estadoPartida;

	cargaPartidas(listaPartidas);

	int numPartida;
	char opcion;
	bool tableroAbierto = true;
	if (listaPartidas.contador < 1) opcion = 'N';
	else if(listaPartidas.contador == MAX_PARTIDAS) opcion = 'E';
	else opcion = menuTipoPartida();

	if (toupper(opcion) == 'E') {
		numPartida = menuNumeroPartida(listaPartidas);
		estadoPartida = listaPartidas.partidas[--numPartida];
	} else if (toupper(opcion) == 'N') {
		numPartida = 0;
		tableroAbierto = iniciaPartida(estadoPartida);
	}
	if (!tableroAbierto) {
		cout << "No pude abrir el archivo del tablero" << endl;
	} else if (validarTablero(estadoPartida.tablero)) {
		int ganador = partida(estadoPartida);
		if (ganador == -1) {
			insertaNuevaPartida(listaPartidas, estadoPartida);
			guardaPartidas(listaPartidas);
			cout << endl << "JUEGO PARADO Y GUARDADO" << endl;
		}
		else {
			if(toupper(opcion) == 'E') eliminarPartida(listaPartidas, numPartida);
			cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
		}
	}
	else {
		cout << "El tablero de la partida es invalido";
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
	ifstream archivo;
	string name;

	bool valido = false;
	cout << "Dime el nombre del archivo que contiene el tablero: ";
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
	return valido;
}

bool validarTablero(const tTablero tablero) {
	bool valido = true;
	for (int i = 0; i < NUM_CASILLAS; i++) {
		int valInicial = i;
		if (tablero[i] == PUENTE1) {
			buscaCasillaAvanzando(tablero, PUENTE2, i);
			if (valInicial == i) valido = false;
		}
		else if (tablero[i] == DADO1) {
			buscaCasillaAvanzando(tablero, DADO2, i);
			if (valInicial == i) valido = false;
		}
	}
	return valido;
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
	return rand() % (NUM_JUGADORES);
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
	bool abandonado = false;
	pintaTablero(estado);
	
	cout << endl << "**** EMPIEZA EL JUGADOR " << estado.jugadorActivo + 1 << " ****" << endl;
	while (!esMeta(estado.jugadores[estado.jugadorActivo].posicion) && !abandonado) {
		tEstadoJugador jugador = estado.jugadores[estado.jugadorActivo];
		int penalizacion = jugador.penalizacion;

		if (!penalizacion) {
			do {
				tirada(estado.tablero, jugador);
				pintaTablero(estado);
			} while (esCasillaPremio(estado.tablero, jugador.posicion) && !esMeta(jugador.posicion));

			if (!esMeta(jugador.posicion)) {
				estado.jugadores[estado.jugadorActivo] = jugador;
				cambioTurno(estado.jugadorActivo);
			}
		}
		else {
			cout << "... PERO NO PUEDE " << (penalizacion > 1 ? "Y LE QUEDAN " + to_string(penalizacion) + " TURNOS SIN JUGAR" : "HASTA EL SIGUIENTE TURNO") << endl;
			jugador.penalizacion -= 1;
			estado.jugadores[estado.jugadorActivo] = jugador;
			cambioTurno(estado.jugadorActivo);
		}
		pintaTablero(estado);
		char seguir;
		cout << endl << "Quieres seguir jugando? Y/N" << endl;
		cin >> seguir;
		if (toupper(seguir) == 'N') abandonado = true;
		
	}
	if (!abandonado) {
		cout << "** FIN DEL JUEGO **" << endl;
	}
	pintaTablero(estado);
	return abandonado ? -1 : estado.jugadorActivo + 1; // se devuelve el jugador que ha ganado la partida (ya sin tener en cuenta la posicion del array (0 -> 1, 1 -> 2, etc.)) o -1 si se ha abandonado
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
	cout << "CASILLA ACTUAL: " << estadoJug.posicion + 1 << endl;
	cout << "INTRODUCE EL VALOR DEL DADO: ";
	int valorDado = (MODO_DEBUGS ? tirarDadoManual() : tirarDado());
	cout << "VALOR DEL DADO: " << valorDado << endl;
	estadoJug.posicion += valorDado;
	cout << "PASAS A LA CASILLA " << estadoJug.posicion + 1 << endl;
	if (!esMeta(estadoJug.posicion)) {
		efectoTirada(tablero, estadoJug);
	}
}

void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug) {
	tCasilla casillaTablero = tablero[estadoJug.posicion];
	if (esCasillaPremio(tablero, estadoJug.posicion)) {
		estadoJug.posicion = saltaACasilla(tablero, estadoJug.posicion);
		if (casillaTablero == OCA) {
			cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA" << endl;
			cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		else if (casillaTablero == DADO1 || casillaTablero == DADO2) {
			cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
			cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		else if (casillaTablero == PUENTE1 || casillaTablero == PUENTE2) {
			cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
			cout << "SALTAS AL PUENTE EN LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		if (!esMeta(estadoJug.posicion)) cout << "Y VUELVES A TIRAR" << endl;
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
			estadoJug.posicion -= PENALIZACION_LABERINTO;
			cout << "CAES EN EL LABERINTO" << endl;
			cout << "SALTAS A LA CASILLA " << estadoJug.posicion + 1 << endl;
		}
		else if (casillaTablero == CALAVERA) {
			estadoJug.posicion = CASILLA_PARTIDA - 1;
			cout << "MUERTE!!! VUELVES A EMPEZAR" << endl;
			cout << "HAS CAIDO EN LA MUERTE Y VUELVES A EMPEZAR. VAS A LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		if (turnosPerdidos) {
			estadoJug.penalizacion += turnosPerdidos;
			cout << "PIERDES " << turnosPerdidos << " TURNOS" << endl;
		}
	}
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
		pintaJugadores(partida.jugadores, fila, casillasPorFila);
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
		cout << "|" << setw(4) << setfill(' ') << enumToString(tablero[i - 1 + fila * casillasPorFila]);
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
			if (estadosJ[jug].posicion == casilla)
				cout << jug + 1;
			else
				cout << " ";
		}
		cout << b;
		cout << "|";
	}
	cout << endl;

}

char menuTipoPartida() {
	char opcion;
	do {
		cout << "Deseas jugar partida nueva o una existente (N=nueva, E=existente)? ";
		cin >> opcion;
	} while (toupper(opcion) != 'N' && toupper(opcion) != 'E');
	return opcion;
}

int menuNumeroPartida(const tListaPartidas listaPartidas) {
	int numPartida;
	do {
		cout << "Los identificadores disponibles son: ";
		for (int i = 1; i <= listaPartidas.contador; i++) {
			cout << i << " ";
		}
		cout << endl << "Que partida quieres continuar? ";
		cin >> numPartida;
	} while (numPartida > listaPartidas.contador || numPartida < 1);
	return numPartida;
}

bool iniciaPartida(tEstadoPartida& estadoPartida) {
	cout << "Vas a jugar una partida nueva" << endl;
	iniciaTablero(estadoPartida.tablero);
	iniciaJugadores(estadoPartida.jugadores);
	estadoPartida.jugadorActivo = quienEmpieza();
	return cargaTablero(estadoPartida.tablero);
}


void iniciaJugadores(tEstadoJugadores jugadores) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		jugadores[i].posicion = 0;
		jugadores[i].penalizacion = 0;
	}
}

void iniciaTablero(tTablero tablero) {
	for (int i = 0; i < NUM_CASILLAS; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo = (jugadorActivo + 1) % NUM_JUGADORES;
	cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo + 1 << endl;
}

bool cargaPartidas(tListaPartidas& partidas) {
	ifstream archivo;
	string name;

	cout << "Introduce el nombre del archivo a cargar: ";
	cin >> name;
	archivo.open(name);
	bool abierto = archivo.is_open();
	if (abierto) {
		archivo >> partidas.contador;
		
		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida;
			iniciaTablero(partida.tablero);
			cargaTablero(partida.tablero, archivo);
			archivo >> partida.jugadorActivo;
			cargaJugadores(partida.jugadores, archivo);
			partidas.partidas[i] = partida;
		}
	}
	else {
		cout << "No pude abrir el archivo" << endl;
	}
	return abierto;
}

void cargaTablero(tTablero tablero, ifstream& archivo) {
	int numCasilla;
	string tipoCasilla;
	archivo >> numCasilla;
	while (numCasilla != CENTINELA) {
		archivo >> tipoCasilla;
		if (numCasilla < NUM_CASILLAS) tablero[numCasilla - 1] = stringToEnum(tipoCasilla);
		archivo >> numCasilla;
	}
}

void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		archivo >> jugadores[i].posicion;
		archivo >> jugadores[i].penalizacion;
	}
}

void eliminarPartida(tListaPartidas& partidas, int indice) {
	for (int i = indice; i < partidas.contador - 1; i++) {
		partidas.partidas[i] = partidas.partidas[i + 1];
	}
	partidas.contador--;
}

void insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca) {
	if (partidas.contador < MAX_PARTIDAS) {
		partidas.partidas[partidas.contador] = partidaOca;
		partidas.contador++;
	}
}

void guardaPartidas(const tListaPartidas& partidas) {
	ofstream archivo;
	string name;

	cout << "Dime el nombre del fichero al que guardar las partidas: ";
	cin >> name;
	archivo.open(name);
	if (archivo.is_open()) {
		archivo << partidas.contador << "\n";
		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida = partidas.partidas[i];
			guardaTablero(partida.tablero, archivo);
			archivo << partida.jugadorActivo << "\n";
			guardaJugadores(partida.jugadores, archivo);
		}
		archivo.close();
	}
	else {
		cout << "No pude abrir el archivo";
	}
}

void guardaTablero(const tTablero tablero, ofstream& archivo) {
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		if(tablero[i] != NORMAL) {
			archivo << i + 1 << " " << enumToStringCompleto(tablero[i]) << "\n";
		}
	}
	archivo << 0 << "\n";
}

void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		archivo << jugadores[i].posicion << " " << jugadores[i].penalizacion << "\n";
	}
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

string enumToStringCompleto(tCasilla casilla) {
	string cadena;
	switch (casilla) {
	case NORMAL:
		cadena = " ";
		break;
	case OCA:
		cadena = "OCA";
		break;
	case DADO1:
		cadena = "DADO1";
		break;
	case DADO2:
		cadena = "DADO2";
		break;
	case PUENTE1:
		cadena = "PUENTE1";
		break;
	case PUENTE2:
		cadena = "PUENTE2";
		break;
	case POSADA:
		cadena = "POSADA";
		break;
	case CALAVERA:
		cadena = "CALAVERA";
		break;
	case LABERINTO:
		cadena = "LABERINTO";
		break;
	case POZO:
		cadena = "POZO";
		break;
	case CARCEL:
		cadena = "CARCEL";
		break;
	}
	return cadena;
}
