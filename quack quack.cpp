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
	* Carga las partidas guardadas desde un archivo de texto en la lista de partidas proporcionada.
	* @param {tListaPartidas&} partidas - La lista de partidas en la que se cargarán las partidas guardadas.
	* @return {bool} - Devuelve true si la carga de partidas se ha realizado correctamente, false en caso contrario.
*/
bool cargaPartidas(tListaPartidas& partidas);

/**
	* Comprueba si la casilla especificada en el tablero es una casilla premio.
	* @param {tTablero} tablero - El tablero en el que se encuentra la casilla.
	* @param {int} casilla - El número de la casilla a comprobar.
	* @return {bool} - Devuelve true si la casilla es una casilla premio, false en caso contrario.
*/
bool esCasillaPremio(const tTablero tablero, int casilla);

/**
	* Comprueba si la casilla especificada es una casilla meta.
	* @param {int} casilla - El número de la casilla a comprobar.
	* @return {bool} - Devuelve true si la casilla es una casilla meta, false en caso contrario.
*/
bool esMeta(int casilla);

/**
	* Inserta una nueva partida en la lista de partidas.
	* @param {tListaPartidas&} partidas - La lista de partidas en la que se insertará la nueva partida.
	* @param {const tEstadoPartida&} partidaOca - El estado de la nueva partida que se desea insertar.
	* @return {bool} - Devuelve true si la inserción de la nueva partida se ha realizado correctamente, false en caso contrario.
*/
bool insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca);

/**
	* Muestra un menú al usuario para seleccionar una partida nueva o existente y devuelve la selección del usuario.
	* @return {char} El caracter ingresado por el usuario, 'n' para partida nueva o 'e' para partida existente.
*/
char menuSeleccionPartida();

/**
	* Permite jugar una partida nueva y actualiza el estado de la partida en la lista de partidas.
	* @param {tEstadoPartida&} partidaNueva - El estado de la partida que se va a jugar.
	* @return {int} - Devuelve el numero del jugador que ha ganado o -1 si se ha abandonado la partida.
*/
int jugarPartidaNueva(tEstadoPartida& partidaNueva);

/**
	* Ejecuta una partida del juego y muestra el tablero en pantalla.
	* Pregunta al usuario si desea continuar o abandonar la partida.
	* Si el usuario gana se muestra el mensaje de fin de juego.
	* @param {tEstadoPartida} estado - El estado actual del juego.
	* @return {int} El número del jugador que ha ganado la partida o PARTIDA_NO_FINALIZADA si la partida no ha terminado.
*/
int partida(tEstadoPartida& estado);

/**
	* Devuelve el jugador que empezará la partida.
	* @return {int} - Devuelve un número aleatorio entre 0 y NUM_JUGADORES - 1 que indica el jugador que empezará la partida.
*/
int quienEmpieza();

/**
	* Calcula la nueva casilla en la que debería saltar el jugador en función de la casilla actual y las características del tablero.
	* @param {const tTablero} tablero - El tablero del juego.
	* @param {int} casillaActual - La casilla actual del jugador.
	* @return {int} El número de la nueva casilla a la que debería saltar el jugador.
*/
int saltaACasilla(const tTablero tablero, int casillaActual);

/**
	* Permite seleccionar una partida existente de la lista de partidas.
	* @param {const tListaPartidas&} partidas - La lista de partidas de las que se desea seleccionar una partida.
	* @return {int} - Devuelve el índice de la partida seleccionada en la lista de partidas.
*/
int seleccionadorPartidasExistentes(const tListaPartidas& partidas);

/**
	* Genera un número aleatorio entre 1 y 6, ambos incluidos, que simula el resultado del lanzamiento de un dado de seis caras.
	* @return {int} Un número aleatorio entre 1 y 6, ambos incluidos.
*/
int tirarDado();

/**
	* Realiza una tirada manual y devuelve el número obtenido.
	* @return {int} - Devuelve un número entero que indica el resultado de la tirada manual.
*/
int tirarDadoManual();

/**
	* Convierte el enum de una casilla a un string semilegible para el usuario
	* @param {tCasilla} casilla - El tipo de casilla a convertir en una cadena de texto.
	* @return {string} - Devuelve una cadena de texto que representa el tipo de casilla especificado.
*/
string casillaAstring(tCasilla casilla);

/**
	* Convierte el enum de una casilla a un string legible para el usuario
	* @param {tCasilla} casilla - El tipo de casilla a convertir en una cadena de texto.
	* @return {string} Devuelve una cadena de texto que representa el tipo de casilla especificado.
*/
string casillaAstringCompleto(tCasilla casilla);

/**
	* Convierte una cadena de caracteres que representa una casilla a su correspondiente enum
	* @param {string} str - Una cadena de caracteres que representa una casilla del tablero.
	* @return {tCasilla} Enum que representa el tipo de casilla.
*/
tCasilla stringAcasilla(string str);

/**
	* Actualiza la posición proporcionada para que apunte a la siguiente casilla del tipo especificado en el tablero. Buscando hacia adelante.
	* @param {const tTablero} tablero - El tablero en el que se buscará la casilla.
	* @param {tCasilla} tipo - El tipo de casilla a buscar en el tablero.
	* @param {int&} posicion - La posición actual en el tablero. Se actualizará para apuntar a la posicion de la casilla del tipo especificado.
*/
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& posicion);

/**
	* Actualiza la posición proporcionada para que apunte a la siguiente casilla del tipo especificado en el tablero. Buscando hacia atras.
	* @param {const tTablero} tablero - El tablero en el que se buscará la casilla.
	* @param {tCasilla} tipo - El tipo de casilla a buscar en el tablero.
	* @param {int&} posicion - La posición actual en el tablero. Se actualizará para apuntar a la posicion de la casilla del tipo especificado.
*/
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& posicion);

/**
	* Cambia el turno de juego, de modo que si el jugador activo era el jugador 1, ahora será el jugador 2, y viceversa.
	* @param {int} jugadorActivo - Una variable entera que representa el jugador activo en ese momento.
*/
void cambioTurno(int& jugadorActivo);

/**
	* Carga los datos de los jugadores desde un archivo y los almacena en un array que contiene los estados de los jugadores.
	* @param {tEstadoJugadores} jugadores - Un array que contiene los estados de los jugadores.
	* @param {ifstream} archivo - Un flujo de entrada que representa un archivo donde se almacenan los datos de los jugadores.
*/
void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);

/**
	* Carga el tablero a partir del contenido del archivo especificado.
	* @param {tTablero} tablero - El tablero que se desea cargar.
	* @param {ifstream&} archivo - El flujo de entrada de archivo desde el que se cargará el tablero.
*/
void cargaTablero(tTablero tablero, ifstream& archivo);

/**
	* Actualiza el estado del jugador en función del tipo de casilla en la que se encuentra en el tablero.
	* @param {const tTablero} tablero - El tablero de la partida.
	* @param {tEstadoJugador&} estadoJug - El estado del jugador que se desea actualizar.
*/
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);

/**
	* Elimina una partida en una posición determinada de una lista de partidas y actualiza la lista de partidas.
	* @param {tListaPartidas} partidas - Un array de partidas guardadas.
	* @param {int} indice - Un índice que indica la posición de la partida que se desea eliminar del array.
*/
void eliminarPartida(tListaPartidas& partidas, int indice);

/**
	* Guarda el tablero del juego en un archivo.
	* @param {tTablero} tablero - Un array que representa el tablero del juego.
	* @param {ofstream} archivo - Un flujo de salida que representa un archivo donde se desea almacenar el tablero.
*/
void guardaTablero(const tTablero tablero, ofstream& archivo);

/**
	* Guarda el estado de los jugadores en el archivo especificado.
	* @param {const tEstadoJugadores} jugadores - Un array que contiene los estados de los jugadores que se desea guardar.
	* @param {ofstream&} archivo - El flujo de salida de archivo en el que se guardará el estado de los jugadores.
*/
void guardaJugadores(const tEstadoJugadores jugadores, ofstream& archivo);

/**
	* Guarda la lista de partidas en un archivo.
	* @param {const tListaPartidas&} partidas - La lista de partidas que se desea guardar.
*/
void guardaPartidas(const tListaPartidas& partidas);

/**
	* Inicializa los datos de los jugadores en una estructura que representa el estado de los jugadores. Todos los jugadores empiezan en la casilla 1 (index 0) y penalizacion 0.
	* @param {tEstadoJugadores} jugadores - Un array que contiene los estados de los jugadores.
*/
void iniciaJugadores(tEstadoJugadores jugadores);

/**
	* Inicializa los datos de una partida en una estructura que representa el estado de la partida.
	* @param {tEstadoPartida} partidaNueva - Una estructura que representa el estado de una partida.
*/
void inicializacionPartidaNueva(tEstadoPartida& partidaNueva);

/**
	* Inicializa el tablero con los valores por defecto.
	* @param {tTablero} tablero - El tablero que se desea inicializar.
*/
void iniciaTablero(tTablero tablero);

/**
	* Simula un lanzamiento y actualiza la casilla del jugador en el tablero en consecuencia.
	* @param {const tTablero} tablero - Una estructura que representa el tablero del juego.
	* @param {tEstadoJugador} estadoJug - Una estructura que representa el estado de un jugador.
*/
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);

// Funciones de pintar tablero
void pintaBorde(int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaTablero(const tEstadoPartida& partida);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);

int main() {
	srand(time(NULL));
	tListaPartidas listaPartidas;
	tEstadoPartida partidaNueva;

	char seleccionPartida = 'n';

	if (cargaPartidas(listaPartidas) && listaPartidas.contador > 0) seleccionPartida = menuSeleccionPartida();

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
		int ganador = jugarPartidaNueva(partidaNueva);
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

char menuSeleccionPartida() {
	char seleccionPartida;
	do {
		cout << "Desea jugar una partida nueva o existente [N=nueva / E=existente] ";
		cin >> seleccionPartida;
		seleccionPartida = tolower(seleccionPartida);
		if ((seleccionPartida != 'n') && (seleccionPartida != 'e')) cout << "Seleccion no identificada, intentelo de nuevo." << endl;
	} while ((seleccionPartida != 'n') && (seleccionPartida != 'e'));

	return seleccionPartida;
}

void inicializacionPartidaNueva(tEstadoPartida& partidaNueva) {
	cout << "Vas a jugar una partida nueva." << endl;
	string nombreArchivo;
	ifstream archivo;
	cout << "Indica el nombre del fichero que contiene el tablero de la oca: ";
	cin >> nombreArchivo;
	archivo.open(nombreArchivo);
	if (archivo.is_open()) {
		cargaTablero(partidaNueva.tablero, archivo);
		iniciaJugadores(partidaNueva.estadoJug);
		partidaNueva.turno = quienEmpieza();
		archivo.close();
	}
	else {
		cout << "No pude abrir el archivo" << endl;
	}
}

int jugarPartidaNueva(tEstadoPartida& partidaNueva) {
	inicializacionPartidaNueva(partidaNueva);
	return partida(partidaNueva);
}

int seleccionadorPartidasExistentes(const tListaPartidas& partidas) {
	int partidaSeleccionada;
	do {
		cout << "Los identificadoes disponibles son: ";
		for (int i = 1; i <= partidas.contador; i++) cout << i << " ";
		cout << endl << "Que partida quieres continuar? ";
		cin >> partidaSeleccionada;
		if (partidaSeleccionada < 1 || partidaSeleccionada > partidas.contador) {
			cout << "\nSeleccion no valida, elija uno de los identificadores mostrados. \n";
		}
	} while (partidaSeleccionada < 1 || partidaSeleccionada > partidas.contador);
	return partidaSeleccionada - 1;
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
	iniciaTablero(tablero); //sea cual sea el tablero, primero se inicia y luego se añaden las casillas especiales. Aunque sea un tablero no valido para evitar crashes.
	if (archivo.is_open()) {
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
		jugadores[i].casilla--; //Se vuelve un numero del 0 al NUM_CASILLAS - 1, para su correcta representacion en pintaTablero, y para facilitar su posicion en tTablero.
	}
}

bool cargaPartidas(tListaPartidas& partidas) {
	ifstream archivo;
	string name;

	cout << "Nombre del archivo que contiene las partidas: ";
	cin >> name;
	archivo.open(name);
	bool valido = archivo.is_open();
	if (valido) {
		archivo >> partidas.contador;
		for (int i = 0; i < partidas.contador; i++) {
			cargaTablero(partidas.partidas[i].tablero, archivo);
			archivo >> partidas.partidas[i].turno;
			partidas.partidas[i].turno--; // Se pone el turno teniendo en cuenta el indice del array.
			cargaJugadores(partidas.partidas[i].estadoJug, archivo);
		}
		archivo.close();
		cout << "Fichero cargado correctamente. \n\n";
	}
	else {
		cout << "No se pudo abrir el archivo con las partidas cargadas." << endl;
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
	int casillaSinIndice;
	for (int i = 0; i < NUM_JUGADORES; i++) {
		casillaSinIndice = jugadores[i].casilla + 1; //Se convierte la casilla del jugador dependiente del array tablero, a una casilla del 1 al NUM_CASILLAS.
		archivo << casillaSinIndice << " " << jugadores[i].penalizaciones << endl;
	}
}

void guardaPartidas(const tListaPartidas& partidas) {
	ofstream archivo;
	string nombre;
	cout << "Indique en que archivo desea guardar las partidas: ";
	cin >> nombre;
	archivo.open(nombre);
	if (archivo.is_open()) {
		int turnoSinIndice;
		archivo << partidas.contador << endl;
		for (int i = 0; i < partidas.contador; i++) {
			guardaTablero(partidas.partidas[i].tablero, archivo);
			turnoSinIndice = partidas.partidas[i].turno + 1; //Se convierte el indice del array que indica el turno en un valor de 1 a NUM_JUGADORES.
			archivo << turnoSinIndice << endl;
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
	return rand() % NUM_JUGADORES;
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
	if (casillaTablero == OCA) buscaCasillaAvanzando(tablero, OCA, casillaActual);
	else if (casillaTablero == DADO1) buscaCasillaAvanzando(tablero, DADO2, casillaActual);
	else if (casillaTablero == PUENTE1) buscaCasillaAvanzando(tablero, PUENTE2, casillaActual);
	else if (casillaTablero == DADO2) buscaCasillaRetrocediendo(tablero, DADO1, casillaActual);
	else if (casillaTablero == PUENTE2) buscaCasillaRetrocediendo(tablero, PUENTE1, casillaActual);
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
	if (estadoJug.casilla + valorDado >= NUM_CASILLAS) {
		estadoJug.casilla = NUM_CASILLAS - 1;
	}
	else {
		estadoJug.casilla += valorDado;
	}
	cout << "PASAS A LA CASILLA " << estadoJug.casilla + 1 << endl;
	if (!esMeta(estadoJug.casilla)) {
		efectoTirada(tablero, estadoJug);
	}
}

int partida(tEstadoPartida& estado) {
	bool partidaPausada = false, ganado = false;
	char caracterContinuarPartida;
	pintaTablero(estado);
	cout << endl << "**** EMPIEZA EL JUGADOR " << estado.turno + 1 << " ****" << endl;
	while (!esMeta(estado.estadoJug[estado.turno].casilla) && !partidaPausada) {
		if (estado.estadoJug[estado.turno].penalizaciones == 0 && !esMeta(estado.estadoJug[estado.turno].casilla)) {
			tirada(estado.tablero, estado.estadoJug[estado.turno]);
			pintaTablero(estado);
			if (!esMeta(estado.estadoJug[estado.turno].casilla) && !esCasillaPremio(estado.tablero, estado.estadoJug[estado.turno].casilla)) {
				cambioTurno(estado.turno);
			}
			else if (esMeta(estado.estadoJug[estado.turno].casilla)) ganado = true;
		}
		else {
			cout << "... PERO NO PUEDE " << (estado.estadoJug[estado.turno].penalizaciones > 1 ? "Y LE QUEDAN " + to_string(estado.estadoJug[estado.turno].penalizaciones) + " TURNOS SIN JUGAR" : "HASTA EL SIGUIENTE TURNO") << endl;
			estado.estadoJug[estado.turno].penalizaciones -= 1;
			cambioTurno(estado.turno);
		}

		if (!ganado && !esCasillaPremio(estado.tablero, estado.estadoJug[estado.turno].casilla)) {
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

	return partidaPausada ? PARTIDA_NO_FINALIZADA : estado.turno + 1; // se devuelve el jugador que ha ganado la partida (ya sin tener en cuenta la posicion del array (0 -> 1, 1 -> 2, etc.))
}

void cambioTurno(int& jugadorActivo) {
	jugadorActivo = (jugadorActivo + 1) % NUM_JUGADORES;
	cout << endl << "TURNO PARA EL JUGADOR " << jugadorActivo + 1 << endl;
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
