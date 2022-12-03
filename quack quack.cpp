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

//Tablero de juego.
typedef tCasilla tTablero[NUM_CASILLAS];

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
	int contador = 0;
};





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

//Tirar dado al azar o modo debug.
int tirarDado();
int tirarDadoManual();

/**
   * Saca un numero aleatorio entre NUM_JUGADORES y 1 y lo devuelve
*/
int quienEmpieza();


void tirada(const tTablero tablero, tEstadoJugador& estadoJug);
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);
int partida(tEstadoPartida& estado);
void cambioTurno(int& jugadorActivo);

// Carga cada una de las partidas guardadas en el archivo a distintas posiciones de array
void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);
void cargaTablero(tTablero tablero, ifstream& archivo);
bool cargaPartidas(tListaPartidas& partidas);


// Inicializa nuevos jugadores y nuevo tablero en posiciones default (0) y casilla NORMAL
// y se implementa todo en la funcion inicializacionPartidaNueva de acorde a NUM_JUGADORES.
void iniciaJugadores(tEstadoJugadores jugadores);
void iniciaTablero(tTablero tablero);
void inicializacionPartidaNueva(tEstadoPartida& partidaNueva);



/**
   * Devuelve como enum el valor de la string proporcionada
   * @example
   * // Expected output: OCA
   * stringAcasilla("OCA");
   *
   * // Expected output: LABERINTO
   * stringAcasilla("LABERINTO");
*/
tCasilla stringAcasilla(string str);

/**
   * Devuelve como string simplificado el valor del enum proporcionado
   * @example
   * // Expected output: "OCA"
   * casillaAstring(OCA);
   *
   * // Expected output: LBRN
   * casillaAstring(LABERINTO);
*/
string casillaAstring(tCasilla type);
string casillaAstringCompleto(tCasilla casilla); //Igual a la anterior pero devuelve la string completa -> casillaAstringCompleto(LABERINTO) = "LABERINTO"

//Funciones relacionadas con el manejo de partidas en el array tListaPartidas.
void eliminarPartida(tListaPartidas& partidas, int indice);
bool insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca);

//Guardado del tablero usado y las posiciones de los jugadores y turno actual.
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo);
void guardaPartidas(const tListaPartidas& partidas);



//-------------------------------------------------------------------------
// Subprogramas para pintar el tablero o mostrar informaci�n
void pintaTablero(const tEstadoPartida& partida); 
// y sus auxiliares
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);

//---------------------------------------------------------------------------

int seleccionadorPartidasExistentes(const tListaPartidas& partidas);
int jugarPartidaNueva(tListaPartidas& partidas,tEstadoPartida& partidaNueva);
char menuSeleccionPartida(bool partidasCargadasChecker);


int main() {
	srand(time(NULL));
	tListaPartidas listaPartidas;
	tEstadoPartida partidaNueva;	

	bool partidasCargadasChecker = cargaPartidas(listaPartidas);
	char seleccionPartida = menuSeleccionPartida(partidasCargadasChecker);

	if (seleccionPartida == 'e') {
		int partidaSeleccionada = seleccionadorPartidasExistentes(listaPartidas);
		int ganador = partida(listaPartidas.partidas[partidaSeleccionada - 1]);
		if (ganador != PARTIDA_NO_FINALIZADA) {
			cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl << endl << endl;
			cout << "La partida " << partidaSeleccionada << " ha terminado. Se elimina de la lista de partidas. ";
			eliminarPartida(listaPartidas, partidaSeleccionada);

		}
		guardaPartidas(listaPartidas);
	}
	else {
		if (!partidasCargadasChecker) cout << "No se pudo abrir el archivo con las partidas cargadas." << endl;
		int ganador = jugarPartidaNueva(listaPartidas, partidaNueva);
		if (ganador != PARTIDA_NO_FINALIZADA) {
			cout << endl << "------ GANA EL JUGADOR " << ganador << " ------" << endl << endl;
		}
		else {
			if (insertaNuevaPartida(listaPartidas, partidaNueva)) {
				guardaPartidas(listaPartidas);
			}
			else {
				cout << "No se ha podido guardar la partida porque se ha alcanzado el limite de partidas guardadas." << endl;
			}
		}
	}
	return 0;
}

char menuSeleccionPartida(bool partidasCargadasChecker) {
	char seleccionPartida;
	if (partidasCargadasChecker) { //Si se han cargado las partidas correctamente, se pregunta si quiere empezar una nueva o continuar una existente.
		do {
			cout << "Desea jugar una partida nueva o existente [N=nueva / E=existente] ";
			cin >> seleccionPartida;
			seleccionPartida = tolower(seleccionPartida);
			if ((seleccionPartida != 'n') && (seleccionPartida != 'e')) cout << "Seleccion no identificada, intentelo de nuevo." << endl;
		} while ((seleccionPartida != 'n') && (seleccionPartida != 'e'));
	}
	else seleccionPartida = 'n'; // se inciara una nueva partida directamente
	return seleccionPartida;
}

void inicializacionPartidaNueva(tEstadoPartida& partidaNueva) {
	cout << "Vas a jugar una partida nueva." << endl;
	string nombreArchivo;
	ifstream archivo;
	cout << "Indica el nombre del fichero que contiene el tablero de la oca: ";
	cin >> nombreArchivo;
	archivo.open(nombreArchivo);
	if(archivo.is_open()) {
		cargaTablero(partidaNueva.tablero, archivo);
		iniciaJugadores(partidaNueva.estadoJug);
		partidaNueva.turno = quienEmpieza();
		archivo.close();
	} else {
		cout << "No pude abrir el archivo" << endl;
	}	
}

int jugarPartidaNueva(tListaPartidas& partidas, tEstadoPartida& partidaNueva) {
	inicializacionPartidaNueva(partidaNueva);
	return partida(partidaNueva);
}

int seleccionadorPartidasExistentes(const tListaPartidas& partidas) {
	int partidaSeleccionada;
	cout << "Los identificadoes disponibles son: ";
	for (int i = 1; i <= partidas.contador; i++) cout << i << " "; 
	cout << endl << "Que partida quieres continuar? ";
	cin >> partidaSeleccionada;
	while (partidaSeleccionada < 1 || partidaSeleccionada > partidas.contador) {
		cout << "\nSeleccion no valida, elija uno de los identificadores mostrados. \n"; 
		for (int i = 1; i <= partidas.contador; i++) { cout << i << " "; }
		cout << endl << "Que partida quieres continuar? ";
		cin >> partidaSeleccionada;
	}
	return partidaSeleccionada;
}

void iniciaJugadores(tEstadoJugadores jugadores) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		jugadores[i].casilla = 0;
		jugadores[i].penalizaciones = 0;
	}
}

void iniciaTablero(tTablero tablero) {
	for (int i = 0; i < NUM_CASILLAS; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}
void cargaTablero(tTablero tablero, ifstream& archivo) {
	bool valido = archivo.is_open();
	if (valido) {
		iniciaTablero(tablero); //sea cual sea el tablero, primero se inicia y luego se añaden las casillas especiales.
		int numCasilla;
		string tipoCasilla;
		archivo >> numCasilla;
		while (numCasilla != CENTINELA) {
			archivo >> tipoCasilla;
			if (numCasilla <= NUM_CASILLAS) tablero[numCasilla - 1] = stringAcasilla(tipoCasilla);
			archivo >> numCasilla;
		}
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
		cout << "Fichero cargado correctamente. \n\n";
	}
	return valido;
}

void guardaTablero(const tTablero tablero, ofstream& archivo) {
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		if (tablero[i] != NORMAL) {
			archivo << i + 1 << " " << casillaAstringCompleto(tablero[i]) << endl;
		}
	}
	archivo << "0" << endl;
}

void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo) {
	for (int i = 0; i < NUM_JUGADORES; i++) {
		archivo << jugadores[i].casilla << " " << jugadores[i].penalizaciones << endl;
	}
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
			archivo << partidas.partidas[i].turno << endl;
			guardaJugadores(partidas.partidas[i].estadoJug, archivo);
		}
		archivo.close();
		cout << "La partida se ha guardado correctamente en " << nombre;
	}
	else {
		cout << "No se encontro el archivo deseado. \n";
	}
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

bool esMeta(int casilla) {
	return casilla >= NUM_CASILLAS - 1;
}

bool esCasillaPremio(const tTablero tablero, int casilla) {
	int casillaTablero = tablero[casilla];
	return casillaTablero == OCA || casillaTablero == DADO1 || casillaTablero == DADO2 || casillaTablero == PUENTE1 || casillaTablero == PUENTE2;
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
			cout << "HAS CAIDO EN LA MUERTE Y VUELVES A EMPEZAR. VAS A LA CASILLA: " << estadoJug.casilla + 1 << endl;
		}
		if (turnosPerdidos) {
			estadoJug.penalizaciones += turnosPerdidos;
			cout << "PIERDES " << turnosPerdidos << " TURNOS" << endl;
		}
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

int partida(tEstadoPartida& estado) {
	bool partidaPausada = false, ganado = false;
	char caracterContinuarPartida;
	pintaTablero(estado);
	cout << endl << "**** EMPIEZA EL JUGADOR " << estado.turno << " ****" << endl;
	int indexTurno = estado.turno - 1;
	while (!esMeta(estado.estadoJug[indexTurno].casilla) && !partidaPausada) {
		if (estado.estadoJug[indexTurno].penalizaciones == 0 && !esMeta(estado.estadoJug[indexTurno].casilla)) {
			tirada(estado.tablero, estado.estadoJug[indexTurno]);
			pintaTablero(estado);
			if (!esMeta(estado.estadoJug[indexTurno].casilla) && !esCasillaPremio(estado.tablero, estado.estadoJug[estado.turno - 1].casilla)) {
				cambioTurno(estado.turno);
			}
			else if (esMeta(estado.estadoJug[indexTurno].casilla)) ganado = true;
		}
		else {
			cout << "... PERO NO PUEDE " << (estado.estadoJug[indexTurno].penalizaciones > 1 ? "Y LE QUEDAN " + to_string(estado.estadoJug[indexTurno].penalizaciones) + " TURNOS SIN JUGAR" : "HASTA EL SIGUIENTE TURNO") << endl;
			estado.estadoJug[indexTurno].penalizaciones--;
			cambioTurno(estado.turno);
		}
		if (!ganado && !esCasillaPremio(estado.tablero, estado.estadoJug[indexTurno].casilla)) {
			cout << endl << "Si quieres abandonar pulse la A. Para continuar pulse cualquier otra tecla... ";
			cin >> caracterContinuarPartida;
			if (tolower(caracterContinuarPartida) == 'a') partidaPausada = true;
		}
	}
	if (!partidaPausada) {
		cout << "** FIN DEL JUEGO **" << endl;
		pintaTablero(estado);
	}
	else cout << "PARTIDA SIN ACABAR" << endl;

	return partidaPausada ? PARTIDA_NO_FINALIZADA : estado.turno; // se devuelve el jugador que ha ganado la partida (ya sin tener en cuenta la posicion del array (0 -> 1, 1 -> 2, etc.))
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo = (jugadorActivo % NUM_JUGADORES) + 1;
	cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo << endl;
}

tCasilla stringAcasilla(string str) {
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

string casillaAstringCompleto(tCasilla casilla) {
	string cadena;
	switch (casilla) {
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
