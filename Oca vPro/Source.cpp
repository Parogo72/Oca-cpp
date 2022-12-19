#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
#include <istream>
#include <unordered_map>

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
const string TABLERO63CASILLAS = "000012001002010001400010061000310001000015001000010761000810001";

typedef enum {
	NORMAL, OCA, PUENTE, POZO, POSADA,
	LABERINTO, DADO, CARCEL, CALAVERA
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
	unordered_map<string, string> ajustes;
	int jugadorActivo = 0;
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
int validarTablero(string tablero);
// Subprogramas que se heredan de la v1
int tirarDado();
int tirarDadoManual();
bool esMeta(int casilla);
int quienEmpieza();
void cambioTurno(int& jugadorActivo);


// Subprogramas para configurar el tablero heredados de la v2
void iniciaTablero(tTablero tablero);


// Subprogramas que sustituyen a otros implementados en la versi�n 1 
// relacionados con la identificaci�n de casillas especiales y con el efecto provocado por las mismas
bool esCasillaPremio(string tablero, int casilla);
int saltaACasilla(string tablero, int casillaActual);
void buscaCasillaAvanzando(string tablero, tCasilla tipo, int& posicion);
void buscaCasilla(string tablero, tCasilla tipo, int& posicion);
void efectoTirada(string tablero, tEstadoJugador& jugador);    // NUEVO PROTOTIPO EN V3; SUSTITUYE A    void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);

// Subprogramas para la l�gica de las tiradas y de la partida
void iniciaJugadores(tEstadoJugadores jugadores, int numJugadores);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
void tirada(string tablero, tEstadoJugador& estadoJug);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
int partida(tEstadoPartida& estado);  // NUEVO PROTOTIPO EN V3; SUSTITUYE A   int partida(const tTablero tablero);


// Subprogramas para la gestion de la lista de partidas abandonadas
bool cargaPartidas(unordered_map<string, string>& ajustes, tListaPartidas& partidas);
void cargaJugadores(tEstadoPartida& partida, ifstream& archivo);
void eliminarPartida(tListaPartidas& partidas, int indice);
void insertaNuevaPartida(tListaPartidas& partidas, tEstadoPartida& partidaOca);
void guardaPartidas(unordered_map<string, string> ajustes, const tListaPartidas& partidas);


// Subprograma para pintar el tablero...
void pintaTablero(tEstadoPartida& partida);   //NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
// y sus auxiliares
void pintaFilaBlanco(int casillasPorFila);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(string tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);

char menuTipoPartida();

int menuIncial();
int menuNumeroPartida(const tListaPartidas listaPartidas);
void iniciaPartida(tEstadoPartida& estadoPartida);

tCasilla charToEnum(char casilla);
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

void pantallaAjustes(unordered_map<string, string>& ajustes);
void pantallaNuevaPartida(unordered_map<string, string>& ajustes, tListaPartidas& listaPartidas);
void pantallaListaPartidas(tAjustesPartida& ajustes, tListaPartidas& listaPartidas);
void pantallaGuardarPartida(tEstadoPartida& estadoPartida, tListaPartidas& listaPartidas);
void pantallaInfoPartida(tAjustesPartida& ajustes, tListaPartidas & listaPartidas, int index);
void pantallaSobreescribirPartida(tEstadoPartida& estadoPartida, tListaPartidas& listaPartidas, int index);
void pantallaEditorTablero(unordered_map<string, string>& ajustes);
void pantallaBorrarPartida(tEstadoPartida& estadoPartida, tListaPartidas & listaPartidas, int index);
void guardarAjustes(unordered_map<string, string> ajustes);
void cargaAjustes(unordered_map<string, string>& ajustes);
void pantallaEditorCasilla(unordered_map<string, string>& ajustes);
void pantallaReiniciarTablero(unordered_map<string, string>& ajustes);
int menuTipoCasilla(unordered_map<string, string>& ajustes, int casilla);


int main() {
	srand(time(NULL));
	tAjustesPartida ajustesPartida;
	tListaPartidas listaPartidas;
	unordered_map<string, string> ajustes;

	cargaAjustes(ajustes);
	cargaPartidas(ajustes, listaPartidas);
	int opcionMenu;
	do {
		Clear();
		opcionMenu = menuIncial();
		if (opcionMenu == 1) {
			pantallaNuevaPartida(ajustes, listaPartidas);
		}
		else if (opcionMenu == 2) {
			pantallaListaPartidas(ajustesPartida, listaPartidas);
		}
		else if (opcionMenu == 3) {
			pantallaEditorTablero(ajustes);
		}
		else if (opcionMenu == 4) {
			pantallaAjustes(ajustes);
		}
	} while (opcionMenu != 5);
	
	guardaPartidas(ajustes, listaPartidas);

	return 0;
}

bool esCasillaPremio(string tablero, int casilla) {
	int casillaTablero = charToEnum(tablero.at(casilla));
	int checkOcaCasilla = casilla;
	buscaCasillaAvanzando(tablero, OCA, checkOcaCasilla);
	return casillaTablero == OCA ? checkOcaCasilla != casilla : casillaTablero == DADO || casillaTablero == PUENTE;
}

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS - 1;
}

bool cargaTablero(unordered_map<string, string> ajustes, string nombreTablero = " ") {
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
		archivo >> ajustes["tablero"];
		archivo.close();
	}
	return valido;
}

int validarTablero(string tablero) {
	int error = 0;
	int numOcas = 0;
	int numPuentes = 0;
	int numDados = 0;
	for (int i = 0; i < NUM_CASILLAS; i++) {
		int valInicial = i;
		tCasilla casillaTablero = charToEnum(tablero.at(i));
		if (casillaTablero == OCA) numOcas++;
		if (casillaTablero == PUENTE) numPuentes++;
		if (casillaTablero == DADO) numDados++;
	}
	if (numOcas == 1) error = 1;
	if (numPuentes != 2 && numPuentes != 0) error = 2;
	if (numDados != 2 && numDados != 0) error = 3;
	return error;
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

int saltaACasilla(string tablero, int casillaActual) {
	tCasilla casillaTablero = charToEnum(tablero.at(casillaActual));
	if (casillaTablero == OCA) {
		buscaCasillaAvanzando(tablero, OCA, casillaActual);
	}
	else if (casillaTablero == DADO) {
		buscaCasilla(tablero, DADO, casillaActual);
	}
	else if (casillaTablero == PUENTE) {
		buscaCasilla(tablero, PUENTE, casillaActual);
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
				tirada(estado.ajustes["tablero"], estado.jugadores[estado.jugadorActivo]);
				pintaTablero(estado);
			} while (esCasillaPremio(estado.ajustes["tablero"], estado.jugadores[estado.jugadorActivo].posicion) && !esMeta(estado.jugadores[estado.jugadorActivo].posicion));

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

void buscaCasillaAvanzando(string tablero, tCasilla tipo, int& posicion) {
	bool encontrado = false;
	int i = posicion + 1;
	while (!encontrado && i < NUM_CASILLAS) {
		if (charToEnum(tablero.at(i)) == tipo) {
			posicion = i;
			encontrado = true;
		}
		i++;
	}
}

void buscaCasilla(string tablero, tCasilla tipo, int& posicion) {
	bool encontrado = false;
	int i = 0;
	while (!encontrado && i < NUM_CASILLAS) {
		if (charToEnum(tablero.at(i)) == tipo && i != posicion) {
			posicion = i;
			encontrado = true;
		}
		i++;
	}
}

void tirada(string tablero, tEstadoJugador& estadoJug) {
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

void efectoTirada(string tablero, tEstadoJugador& estadoJug) {
	tCasilla casillaTablero = charToEnum(tablero.at(estadoJug.posicion));
	if (esCasillaPremio(tablero, estadoJug.posicion)) {
		int nuevaCasilla = saltaACasilla(tablero, estadoJug.posicion);
		estadoJug.posicion = nuevaCasilla;
		if (casillaTablero == OCA && nuevaCasilla != estadoJug.posicion) {
			cout << "DE OCA A OCA Y TIRO PORQUE ME TOCA" << endl;
			cout << "SALTAS A LA SIGUIENTE OCA EN LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		else if (casillaTablero == DADO) {
			cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO " << endl;
			cout << "SALTAS AL SIGUIENTE DADO EN LA CASILLA: " << estadoJug.posicion + 1 << endl;
		}
		else if (casillaTablero == PUENTE) {
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
void pintaTablero(tEstadoPartida& partida) {

	int numFilas = 3;
	int casillasPorFila = NUM_CASILLAS / numFilas; // Es 21

	cout << endl;
	for (int fila = 0; fila < numFilas; fila++) {
		pintaBorde(casillasPorFila);
		pintaNumCasilla(fila, casillasPorFila);
		pintaTipoCasilla(partida.ajustes["tablero"], fila, casillasPorFila);
		pintaJugadores(partida.jugadores, fila, casillasPorFila);
	}

	pintaBorde(casillasPorFila);
	cout << endl;
}

void pintaTablero(string tablero) {

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


void pintaTipoCasilla(string tablero, int fila, int casillasPorFila) {
	for (int i = 1; i <= casillasPorFila; i++)
		cout << "|" << setw(4) << setfill(' ') << enumToString(charToEnum(tablero.at(i - 1 + fila * casillasPorFila)));
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
	iniciaJugadores(estadoPartida.jugadores, stoi(estadoPartida.ajustes["numJugadores"]));
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
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo = (jugadorActivo + 1) % NUM_JUGADORES;
	cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo + 1 << endl;
}

bool cargaPartidas(unordered_map<string, string>& ajustes, tListaPartidas& partidas) {
	ifstream archivo;
	string name = ajustes["nombrePartidas"];

	archivo.open(name);
	bool abierto = archivo.is_open();
	if (abierto) {
		archivo >> partidas.contador;

		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida;
			archivo >> partida.ajustes["tablero"];
			archivo >> partida.jugadorActivo;
			cargaJugadores(partida, archivo);
			partida.index = i;
			partidas.partidas[i] = partida;
		}
		archivo.close();
	}
	return abierto;
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
	partida.ajustes["numJugadores"] = i + '0';
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

void guardaPartidas(unordered_map<string, string> ajustes, const tListaPartidas& partidas) {
	ofstream archivo;
	string name = ajustes["nombrePartidas"];

	archivo.open(name);
	if (archivo.is_open()) {
		archivo << partidas.contador << "\n";
		for (int i = 0; i < partidas.contador; i++) {
			tEstadoPartida partida = partidas.partidas[i];
			archivo << partida.ajustes["tablero"] << "\n";
			archivo << partida.jugadorActivo << "\n";
			for (int i = 0; i < stoi(partida.ajustes["numJugadores"]); i++) {
				archivo << partida.jugadores[i].posicion << " " << partida.jugadores[i].penalizacion << "\n";
			}
			archivo << CENTINELA_JUGADORES << "\n";
		}
		archivo.close();
	}
}

void guardaJugadores(const tEstadoPartida& partida, ofstream& archivo) {
	for (int i = 0; i < /*partida.ajustes["numJugadores"]*/ 3; i++) {
		archivo << partida.jugadores[i].posicion << " " << partida.jugadores[i].penalizacion << "\n";
	}
	archivo << CENTINELA_JUGADORES << "\n";
}

tCasilla charToEnum(char casilla) {
	return static_cast<tCasilla>(casilla - '0');
}
tCasilla stringToEnum(string str) {
	tCasilla newEnum = NORMAL;
	if (str == "OCA") newEnum = OCA;
	else if (str == "PUENTE") newEnum = PUENTE;
	else if (str == "POZO") newEnum = POZO;
	else if (str == "POSADA") newEnum = POSADA;
	else if (str == "LABERINTO") newEnum = LABERINTO;
	else if (str == "DADO") newEnum = DADO;
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
	case DADO:
		cadena = "DADO";
		break;
	case PUENTE:
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
		cadena = "NORMAL";
		break;
	case OCA:
		cadena = "OCA";
		break;
	case DADO:
		cadena = "DADO";
		break;
	case PUENTE:
		cadena = "PUENTE";
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
		cout << "3.- Editor Tablero" << endl;
		cout << "4.- Ajustes" << endl;
		cout << "5.- Salir" << endl;
		cin >> opcion;
		Clear();
	} while (opcion > 5 || opcion < 1);
	Clear();
	return opcion;
}

void pantallaAjustes(unordered_map<string, string>& ajustes) {
	int opcion;
	do {
		cout << "1.- Numero de jugadores: " << ajustes["numJugadores"] << endl;
		cout << "2.- Tablero cargado: " << endl;
		pintaTablero(ajustes["tablero"]);
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
		ajustes["numJugadores"] = to_string(numJugadores);
		Clear();
		guardarAjustes(ajustes);
		pantallaAjustes(ajustes);
	}
	else if (opcion == 2) {
		string nombreTablero;
		cout << "Introduce el nombre del archivo que contiene el tablero" << endl;
		cin >> nombreTablero;
		cargaTablero(ajustes, nombreTablero);
		Clear();
		guardarAjustes(ajustes);
		pantallaAjustes(ajustes);
	}
	guardarAjustes(ajustes);
}

void pantallaNuevaPartida(unordered_map<string, string>& ajustes, tListaPartidas& listaPartidas) {
	char choice;
	do {
		cout << "Vas a empezar una nueva partida con la siguiente configuracion: " << endl;
		cout << "Numero de jugadores: " << ajustes["numJugadores"] << endl;
		cout << "Tablero cargado: " << endl;
		pintaTablero(ajustes["tablero"]);
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
				cout << i + 1 << ".- " << "JUGADORES: " << listaPartidas.partidas[i].ajustes["numJugadores"] << "   TURNO: " << listaPartidas.partidas[i].jugadorActivo + 1 << endl;
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
			cout << i + 1 << ".- " << "JUGADORES: " << listaPartidas.partidas[i].ajustes["numJugadores"] << "   TURNO: " << listaPartidas.partidas[i].jugadorActivo + 1;
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

void guardarAjustes(unordered_map<string, string> ajustes) {
	ofstream archivo;
	archivo.open("config.txt");
	for (auto& it : ajustes) {
		archivo << it.first << "=" << it.second << endl;
	}
	archivo.close();
}

void cargaAjustes(unordered_map<string, string>& ajustes) {
	ifstream archivo;
	archivo.open("config.txt");
	if (archivo.is_open()) {
		string key, value;
		while (getline(archivo, key, '=')) {
			getline(archivo, value);
			ajustes.insert({ key, value });
		}
		archivo.close();
	}
	else {
		archivo.close();
		ofstream archivoNuevo;
		archivoNuevo.open("config.txt");
		ajustes.insert({ "numJugadores", "2" });
		ajustes.insert({ "nombrePartidas", "partidas.txt" });
		ajustes.insert({ "tablero", TABLERO63CASILLAS });
		if (archivoNuevo.is_open()) {
			for (pair<string, string> element : ajustes) {
				archivoNuevo << element.first << "=" << element.second << endl;
			}
		}
	}
}

void pantallaEditorTablero(unordered_map<string, string>& ajustes) {
	int opcion;
	do {
		cout << "Tablero cargado: " << endl;
		pintaTablero(ajustes["tablero"]);
		cout << "1.- Editar" << endl;
		cout << "2.- Reiniciar" << endl;
		cout << "3.- Salir" << endl;
		cout << "Selecciona una opcion: ";
		cin >> opcion;
		Clear();
	} while (opcion > 3 || opcion < 1);
	if (opcion == 1) {
		pantallaEditorCasilla(ajustes);
		Clear();
	}
	else if (opcion == 2) {
		pantallaReiniciarTablero(ajustes);
		Clear();
	}
	else if (opcion == 3) {
		int error = validarTablero(ajustes["tablero"]);
		Clear();
		if (error == 0) return;
		cout << "Tablero no valido. ";
		if (error == 1) cout << "El tablero necesita al menos una oca mas";
		else if (error == 2) cout << "El tablero solo puede tener 0 o 2 puentes";
		else if (error == 3) cout << "El tablero solo puede tener 0 o 2 dados";
		cout << endl;
	}
	pantallaEditorTablero(ajustes);
}

void pantallaReiniciarTablero(unordered_map<string, string>& ajustes) {
	char choice;
	do {
		cout << "Tablero: ";
		pintaTablero(ajustes["tablero"]);
		cout << endl << "Estas seguro de reiniciar el tablero? (Y/N)" << endl;
		cin >> choice;
		Clear();
	} while (tolower(choice) != 'y' && tolower(choice) != 'n');
	if (tolower(choice) == 'y') {
		ajustes["tablero"] = TABLERO63CASILLAS;
	}
}

void pantallaEditorCasilla(unordered_map<string, string>& ajustes) {
	int casilla;
	do {
		pintaTablero(ajustes["tablero"]);
		cout << "Dime el numero de casilla a editar: ";
		cin >> casilla;
		Clear();
	} while (casilla > NUM_CASILLAS || casilla < 1);

	ajustes["tablero"][casilla - 1] = menuTipoCasilla(ajustes, casilla) + '0';
}

int menuTipoCasilla(unordered_map<string, string>& ajustes, int casilla) {
	int choice;
	do {
		cout << "Casilla numero: " << casilla << " Tipo actual: " << enumToStringCompleto(charToEnum(ajustes["tablero"].at(casilla - 1))) << endl;
		cout << "1.- NORMAL" << endl;
		cout << "2.- OCA" << endl;
		cout << "3.- PUENTE" << endl;
		cout << "4.- POZO" << endl;
		cout << "5.- POSADA" << endl;
		cout << "6.- LABERINTO" << endl;
		cout << "7.- DADO" << endl;
		cout << "8.- CARCEL" << endl;
		cout << "9.- CALAVERA" << endl;
		cout << "Selecciona un tipo de casilla: ";
		cin >> choice;
		Clear();
	} while (choice > 9 || choice < 1);
	return choice - 1;
}
