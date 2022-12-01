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
const int CENTINELA_JUGADORES = -1;
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

struct tAjustesPartida {
	int numJugadores = 2;
	tTablero tablero;
	string nombreTablero = "tableroClasico63Casillas.txt";
	string nombrePartidas = "partidas.txt";
};

struct tEstadoPartida {
	tEstadoJugadores jugadores;
	tAjustesPartida ajustes;
	int jugadorActivo = 0;
	int numJugadores;
	int index;
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
bool cargaTablero(tTablero& tablero, string nombreTablero);
void iniciaTablero(tTablero tablero);


// Subprogramas que sustituyen a otros implementados en la versi�n 1 
// relacionados con la identificaci�n de casillas especiales y con el efecto provocado por las mismas
bool esCasillaPremio(const tTablero tablero, int casilla);
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);
void efectoTirada(const tTablero tablero, tEstadoJugador& jugador);    // NUEVO PROTOTIPO EN V3; SUSTITUYE A    void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);

// Subprogramas para la l�gica de las tiradas y de la partida
void iniciaJugadores(tEstadoJugadores jugadores, int numJugadores);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
int partida(tEstadoPartida& estado);  // NUEVO PROTOTIPO EN V3; SUSTITUYE A   int partida(const tTablero tablero);


// Subprogramas para la gestion de la lista de partidas abandonadas
bool cargaPartidas(tAjustesPartida ajustes, tListaPartidas& partidas);
void cargaTablero(tTablero tablero, ifstream& archivo);
void cargaJugadores(tEstadoPartida& partida, ifstream& archivo);
void eliminarPartida(tListaPartidas& partidas, int indice);
void insertaNuevaPartida(tListaPartidas& partidas, tEstadoPartida& partidaOca);
void guardaPartidas(const tAjustesPartida ajustes, const tListaPartidas& partidas);
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoPartida partida, ofstream& archivo);


// Subprograma para pintar el tablero...
void pintaTablero(const tEstadoPartida& partida);   //NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
void pintaTablero(const tTablero ajustes);
// y sus auxiliares
void pintaFilaBlanco(int casillasPorFila);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);

char menuTipoPartida();

int menuIncial();
int menuNumeroPartida(const tListaPartidas listaPartidas);
void iniciaPartida(tEstadoPartida& estadoPartida);
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

void Clear();

void pantallaAjustes(tAjustesPartida& ajustes);
void pantallaNuevaPartida(tAjustesPartida& ajustes, tListaPartidas& listaPartidas);
void pantallaListaPartidas(tAjustesPartida& ajustes, tListaPartidas& listaPartidas);
void pantallaGuardarPartida(tEstadoPartida& estadoPartida, tListaPartidas& listaPartidas);
void pantallaInfoPartida(tAjustesPartida& ajustes, tListaPartidas & listaPartidas, int index);
void pantallaSobreescribirPartida(tEstadoPartida& estadoPartida, tListaPartidas& listaPartidas, int index);
void pantallaBorrarPartida(tEstadoPartida & estadoPartida, tListaPartidas & listaPartidas, int index);
void setupInicial(tAjustesPartida& ajustes);
void guardarAjustes(tAjustesPartida ajustes);

int main() {
	srand(time(NULL));
	tAjustesPartida ajustesPartida;
	tListaPartidas listaPartidas;
	setupInicial(ajustesPartida);
	cargaPartidas(ajustesPartida, listaPartidas);
	int opcionMenu;
	do {
		Clear();
		opcionMenu = menuIncial();
		if (opcionMenu == 1) {
			pantallaNuevaPartida(ajustesPartida, listaPartidas);
		}
		else if (opcionMenu == 2) {
			pantallaListaPartidas(ajustesPartida, listaPartidas);
		}
		else if (opcionMenu == 3) {
			pantallaAjustes(ajustesPartida);
		}
	} while (opcionMenu != 4);
	
	guardaPartidas(ajustesPartida, listaPartidas);

	return 0;
}

bool esCasillaPremio(const tTablero tablero, int casilla) {
	int casillaTablero = tablero[casilla];
	return casillaTablero == OCA || casillaTablero == DADO1 || casillaTablero == DADO2 || casillaTablero == PUENTE1 || casillaTablero == PUENTE2;
}

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS - 1;
}

bool cargaTablero(tTablero& tablero, string nombreTablero = " ") {
	ifstream archivo;
	string name;

	bool valido = false;
	if (nombreTablero == " ") {
		cout << "Dime el nombre del archivo que contiene el tablero: ";
		cin >> nombreTablero;
	}
	archivo.open(nombreTablero);
	valido = archivo.is_open();
	if (valido) {
		iniciaTablero(tablero);
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
		int penalizacion = estado.jugadores[estado.jugadorActivo].penalizacion;

		if (!penalizacion) {
			do {
				tirada(estado.ajustes.tablero, estado.jugadores[estado.jugadorActivo]);
				pintaTablero(estado);
			} while (esCasillaPremio(estado.ajustes.tablero, estado.jugadores[estado.jugadorActivo].posicion) && !esMeta(estado.jugadores[estado.jugadorActivo].posicion));

			if (!esMeta(estado.jugadores[estado.jugadorActivo].posicion)) {
				cambioTurno(estado.jugadorActivo);
			}
		}
		else {
			cout << "... PERO NO PUEDE " << (penalizacion > 1 ? "Y LE QUEDAN " + to_string(penalizacion) + " TURNOS SIN JUGAR" : "HASTA EL SIGUIENTE TURNO") << endl;
			estado.jugadores[estado.jugadorActivo].penalizacion -= 1;
			cambioTurno(estado.jugadorActivo);
		}
		pintaTablero(estado);
		char seguir;
		if (!esMeta(estado.jugadores[estado.jugadorActivo].posicion)) {
			cout << endl << "Quieres seguir jugando? Y/N" << endl;
			cin >> seguir;
			if (toupper(seguir) == 'N') abandonado = true;
		}
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
		pintaTipoCasilla(partida.ajustes.tablero, fila, casillasPorFila);
		pintaJugadores(partida.jugadores, fila, casillasPorFila);
	}

	pintaBorde(casillasPorFila);
	cout << endl;


}

void pintaTablero(const tTablero tablero) {

	int numFilas = 3;
	int casillasPorFila = NUM_CASILLAS / numFilas; // Es 21


	cout << endl;
	for (int fila = 0; fila < numFilas; fila++) {
		pintaBorde(casillasPorFila);
		pintaNumCasilla(fila, casillasPorFila);
		pintaTipoCasilla(tablero, fila, casillasPorFila);
		pintaFilaBlanco(casillasPorFila);
	}

	pintaBorde(casillasPorFila);
	cout << endl;


}
// y sus auxiliares

void pintaFilaBlanco(int casillasPorFila) {
	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|    ";
	cout << "|" << endl;
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

void iniciaPartida(tEstadoPartida& estadoPartida) {
	estadoPartida.numJugadores = estadoPartida.ajustes.numJugadores;
	iniciaJugadores(estadoPartida.jugadores, estadoPartida.numJugadores);
	estadoPartida.jugadorActivo = quienEmpieza();
}


void iniciaJugadores(tEstadoJugadores jugadores, int numJugadores) {
	for (int i = 0; i < numJugadores; i++) {
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

bool cargaPartidas(tAjustesPartida ajustes, tListaPartidas& partidas) {
	ifstream archivo;
	string name = ajustes.nombrePartidas;

	archivo.open(name);
	bool abierto = archivo.is_open();
	if (abierto) {
		archivo >> partidas.contador;

		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida;
			iniciaTablero(partida.ajustes.tablero);
			cargaTablero(partida.ajustes.tablero, archivo);
			archivo >> partida.jugadorActivo;
			cargaJugadores(partida, archivo);
			partida.index = i;
			partidas.partidas[i] = partida;
		}
		archivo.close();
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

void cargaJugadores(tEstadoPartida& partida, ifstream& archivo) {
	int i = 0;
	int pos;
	archivo >> pos;
	while (pos != CENTINELA_JUGADORES) {
		partida.jugadores[i].posicion = pos;
		archivo >> partida.jugadores[i].penalizacion;
		archivo >> pos;
		i++;
	}
	partida.numJugadores = i;
}

void eliminarPartida(tListaPartidas& partidas, int indice) {
	for (int i = indice; i < partidas.contador - 1; i++) {
		partidas.partidas[i] = partidas.partidas[i + 1];
	}
	partidas.contador--;
}

void insertaNuevaPartida(tListaPartidas& partidas, tEstadoPartida& partidaOca) {
	if (partidas.contador < MAX_PARTIDAS) {
		partidas.partidas[partidas.contador] = partidaOca;
		partidaOca.index = partidas.contador;
		partidas.contador++;
	}
}

void guardaPartidas(const tAjustesPartida ajustes, const tListaPartidas& partidas) {
	ofstream archivo;
	string name = ajustes.nombrePartidas;

	archivo.open(name);
	if (archivo.is_open()) {
		archivo << partidas.contador << "\n";
		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida = partidas.partidas[i];
			guardaTablero(partida.ajustes.tablero, archivo);
			archivo << partida.jugadorActivo << "\n";
			guardaJugadores(partida, archivo);
		}
		archivo.close();
	}
	else {
		cout << "No pude abrir el archivo";
	}
}

void guardaTablero(const tTablero tablero, ofstream& archivo) {
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		if (tablero[i] != NORMAL) {
			archivo << i + 1 << " " << enumToStringCompleto(tablero[i]) << "\n";
		}
	}
	archivo << CENTINELA << "\n";
}

void guardaJugadores(const tEstadoPartida partida, ofstream& archivo) {
	for (int i = 0; i < partida.numJugadores; i++) {
		archivo << partida.jugadores[i].posicion << " " << partida.jugadores[i].penalizacion << "\n";
	}
	archivo << CENTINELA_JUGADORES << "\n";
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

void Clear()
{
	cout << "\x1B[2J\x1B[H";
}

int menuIncial() {
	int opcion;
	do {
		cout << " __                   _____                      " << endl;
		cout << "/\\ \\                 /\\  __`\\                    " << endl;
		cout << "\\ \\ \\         __     \\ \\ \\/\\ \\    ___     __     " << endl;
		cout << " \\ \\ \\  __  /'__`\\    \\ \\ \\ \\ \\  /'___\\ /'__`\\   " << endl;
		cout << "  \\ \\ \\L\\ \\/\\ \\L\\.\\_   \\ \\ \\_\\ \\/\\ \\__//\\ \\L\\.\\_ " << endl;
		cout << "   \\ \\____/\\ \\__/.\\_\\   \\ \\_____\\ \\____\\ \\__/.\\_\\ " << endl;
		cout << "    \\/___/  \\/__/\\/_/    \\/_____/\\/____/\\/__/\\/_/" << endl << endl;
		cout << "1.- Nueva Partida" << endl;
		cout << "2.- Lista Partidas" << endl;
		cout << "3.- Ajustes" << endl;
		cout << "4.- Salir" << endl;
		cin >> opcion;
		Clear();
	} while (opcion > 4 || opcion < 1);
	Clear();
	return opcion;
}

void pantallaAjustes(tAjustesPartida& ajustes) {
	int opcion;
	do {
		cout << "1.- Numero de jugadores: " << ajustes.numJugadores << endl;
		cout << "2.- Tablero cargado: " << endl;
		pintaTablero(ajustes.tablero);
		cout << "3.- Salir" << endl;
		cout << "Selecciona una opcion: ";
		cin >> opcion;
		Clear();
	} while (opcion > 3 || opcion < 1);

	if (opcion == 1) {
		int numJugadores;
		cout << "Introduce el numero de jugadores (2-4)" << endl;
		cin >> numJugadores;
		while (numJugadores > 4 || numJugadores < 2) {
			Clear();
			cout << "Valor no valido" << endl;
			cout << "Introduce el numero de jugadores (2-4)" << endl;
			cin >> numJugadores;
		}
		ajustes.numJugadores = numJugadores;
		Clear();
		guardarAjustes(ajustes);
		pantallaAjustes(ajustes);
	}
	else if (opcion == 2) {
		string nombreTablero;
		cout << "Introduce el nombre del archivo que contiene el tablero" << endl;
		cin >> nombreTablero;
		cargaTablero(ajustes.tablero, nombreTablero);
		Clear();
		guardarAjustes(ajustes);
		pantallaAjustes(ajustes);
	}
	guardarAjustes(ajustes);
}

void pantallaNuevaPartida(tAjustesPartida& ajustes, tListaPartidas& listaPartidas) {
	char choice;
	do {
		cout << "Vas a empezar una nueva partida con la siguiente configuracion: " << endl;
		cout << "Numero de jugadores: " << ajustes.numJugadores << endl;
		cout << "Tablero cargado: " << endl;
		pintaTablero(ajustes.tablero);
		cout << "Estas seguro de continuar? Y/N" << endl;
		cin >> choice;
		Clear();
	} while (tolower(choice) != 'y' && tolower(choice) != 'n');
	if (tolower(choice) == 'y') {
		tEstadoPartida estadoPartida;
		estadoPartida.ajustes = ajustes;
		iniciaPartida(estadoPartida);
		int ganador = partida(estadoPartida);
		if (ganador == -1) {
			Clear();
			pantallaGuardarPartida(estadoPartida, listaPartidas);
		} else {
			cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
			char salir;
			do {
				cout << "Pulsa (E) para salir" << endl;
				cin >> salir;
			} while (tolower(salir) != 'e');
		}
	}
}

void pantallaListaPartidas(tAjustesPartida& ajustes, tListaPartidas& listaPartidas) {
	int choice;
	if (!listaPartidas.contador) {
		do {
			cout << "No hay ninguna partida guardada" << endl;
			cout << "1.- Salir" << endl;
			cin >> choice;
			Clear();
		} while (choice != 1);
	}
	else {
		do {
			for (int i = 0; i < listaPartidas.contador; i++) {
				cout << i + 1 << ".- " << "JUGADORES: " << listaPartidas.partidas[i].numJugadores << "   TURNO: " << listaPartidas.partidas[i].jugadorActivo + 1 << endl;
			}
			cout << listaPartidas.contador + 1 << ".- Salir" << endl;
			cout << endl << "Selecciona una partida para mas informacion: " << endl;
			cin >> choice;
			Clear();
		} while (choice > listaPartidas.contador + 1 || choice < 1);
		if (choice != listaPartidas.contador + 1) {
			pantallaInfoPartida(ajustes, listaPartidas, choice - 1);
		}
	}
}

void pantallaGuardarPartida(tEstadoPartida& estadoPartida, tListaPartidas& listaPartidas) {
	int choice;
	do {
		for (int i = 0; i < listaPartidas.contador; i++) {
			cout << i + 1 << ".- " << "JUGADORES: " << listaPartidas.partidas[i].numJugadores << "   TURNO: " << listaPartidas.partidas[i].jugadorActivo + 1;
			if (estadoPartida.index == i) cout << " (Partida Actual)";
			cout << endl;
		}
		if(listaPartidas.contador < MAX_PARTIDAS) cout << listaPartidas.contador + 1 << ".- CREAR UN NUEVO GUARDADO" << endl;
		cout << listaPartidas.contador + 2 << ".- No guardar la partida" << endl;
		cout << endl << "Selecciona la partida a sobreescribir: " << endl;
		cin >> choice;
		Clear();
	} while (choice > MAX_PARTIDAS + 1 || choice < 1);
	if (choice <= listaPartidas.contador) {
		pantallaSobreescribirPartida(estadoPartida, listaPartidas, choice - 1);
	}
	else if(choice != MAX_PARTIDAS + 1) {
		insertaNuevaPartida(listaPartidas, estadoPartida);
		guardaPartidas(estadoPartida.ajustes, listaPartidas);
	}	
}

void pantallaInfoPartida(tAjustesPartida & ajustes, tListaPartidas & listaPartidas, int index) {
	tEstadoPartida estadoPartida = listaPartidas.partidas[index];
	int choice;
	do {
		cout << "Partida " << index + 1 << endl;
		cout << "Tablero: ";
		pintaTablero(estadoPartida);
		cout << "Turno actual: " << estadoPartida.jugadorActivo + 1 << endl;
		cout << endl << "1.- Continuar partida" << endl;
		cout << "2.- Borrar partida" << endl;
		cout << "3.- Salir" << endl;
		cin >> choice;
		Clear();
	} while (choice > 3 || choice < 1);
	if (choice == 1) {
		int ganador = partida(estadoPartida);
		if (ganador == -1) {
			Clear();
			pantallaGuardarPartida(estadoPartida, listaPartidas);
		}
		else {
			eliminarPartida(listaPartidas, estadoPartida.index);
			cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl;
			char salir;
			do {
				cout << "Pulsa (E) para salir" << endl;
				cin >> salir;
			} while (tolower(salir) != 'e');
		}
	}
	else if(choice == 2) {
		pantallaBorrarPartida(estadoPartida, listaPartidas, index);
	}
	else {
		pantallaListaPartidas(ajustes, listaPartidas);
	}

}

void pantallaSobreescribirPartida(tEstadoPartida & estadoPartida, tListaPartidas & listaPartidas, int index) {
	tEstadoPartida partidaSobreescribir = listaPartidas.partidas[index];
	char choice;
	do {
		cout << "Partida " << index + 1 << endl;
		cout << "Tablero: ";
		pintaTablero(partidaSobreescribir);
		cout << "Turno actual: " << partidaSobreescribir.jugadorActivo + 1 << endl;
		
		cout << endl << "Estas seguro de sobreescribir esta partida? (Y/N)" << endl;
		cin >> choice;
		Clear();
	} while (tolower(choice) != 'y' && tolower(choice) != 'n');
	if (tolower(choice) == 'y') {
		listaPartidas.partidas[index] = estadoPartida;
		guardaPartidas(estadoPartida.ajustes, listaPartidas);
	}
	else {
		pantallaGuardarPartida(estadoPartida, listaPartidas);
	}
}

void pantallaBorrarPartida(tEstadoPartida & estadoPartida, tListaPartidas & listaPartidas, int index) {
	tEstadoPartida partidaBorrar = listaPartidas.partidas[index];
	char choice;
	do {
		cout << "Partida " << index + 1 << endl;
		cout << "Tablero: ";
		pintaTablero(partidaBorrar);
		cout << "Turno actual: " << partidaBorrar.jugadorActivo + 1 << endl;

		cout << endl << "Estas seguro de borrar esta partida? (Y/N)" << endl;
		cin >> choice;
		Clear();
	} while (tolower(choice) != 'y' && tolower(choice) != 'n');
	if (tolower(choice) == 'y') {
		eliminarPartida(listaPartidas, index);
	}
}
void setupInicial(tAjustesPartida& ajustes) {
	ifstream archivo;
	archivo.open("config.txt");
	if (archivo.is_open()) {
		iniciaTablero(ajustes.tablero);
		cargaTablero(ajustes.tablero, archivo);
		archivo >> ajustes.numJugadores;
		archivo.close();
	}
	else {
		ofstream archivo;
		bool valido = false;
		archivo.open("config.txt");
		iniciaTablero(ajustes.tablero);
		valido = cargaTablero(ajustes.tablero, ajustes.nombreTablero);
		while (!valido) {
			valido = cargaTablero(ajustes.tablero);
		}
		guardaTablero(ajustes.tablero, archivo);
		archivo << ajustes.numJugadores << endl;
		archivo.close();
	}
	archivo.close();
}

void guardarAjustes(tAjustesPartida ajustes) {
	ofstream archivo;
	archivo.open("config.txt");
	guardaTablero(ajustes.tablero, archivo);
	archivo << ajustes.numJugadores << endl;
	archivo.close();
}