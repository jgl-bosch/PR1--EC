/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas las funciones que debéis 
 * implementar en ensamblador.
 * Desde este código se realizan las llamadas a las subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO SE DEBE ENTREGAR.
 **/
 
#include <stdio.h>
#include <termios.h>     //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>      //STDIN_FILENO

/**
 * Constants
 */
#define DimMatrix 5      //dimensión de la matriu 5x5
#define SizeMatrix  DimMatrix*DimMatrix //=25


/**
 * Definición de variables globales.
 **/
extern int developer;    //Variable declarada en assemblador que indica el nom del programador

// Matrices 5x5 donde guardamosm las fichas del juego.       
char tiles[DimMatrix][DimMatrix]    = { {'A','B','C','D','E'},
                                        {'F','G','H','I','J'},
                                        {'K','L','M','N','O'},
                                        {'P','Q','R','T','X'},
                                        {'U','V','W','S',' '}, };
                                      
long pos = 24;      //Posición del cursor en la matriz tiles (5x5), 
                    //spacePos [0 : (SizeMatrix-1)]. 
                    //Fila = pos / DimMatrix [0 : (DimMatrix-1)] 
                    //Columna = pos % DimMatrix [0 : (DimMatrix-1)]

long spacePos = 0;  //Posición del espacio dentro de la matriz tiles (5x5), 
                    //spacePos [0 : (SizeMatrix-1)]. 
                    //Fila = pos / DimMatrix [0 : (DimMatrix-1)] 
                    //Columna = pos % DimMatrix [0 : (DimMatrix-1)]

char state = 1;     //0: Salir, hemos pulsado la tecla 'ESC' para salir. 
                    //1: Continuamos jugando. 
                    //2: No se ha podido realizar el movimiento. 
                    //3: Pierdes, No quedan movimientos.

short moves = 9;    //Movimientos que quedan para ordenar las fichas.

char charac;        //Carácter leído de teclado y para escribir a pantalla.
int rowScreen;      //Fila para posicionar el cursor en la pantalla.
int colScreen;      //Columna para posicionar el cursor en la pantalla.
                                     
/**
 * Definición de las funciones en C
 */
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void updateBoardP1_C();
void posCurScreenP1_C();
void moveCursorP1_C();
void searchSpaceNextToP1_C();
void moveTileP1_C();
void checkEndP1_C();
void printMessageP1_C();
void playP1_C();
void printMessageP1_C();


/**
 * Definición de las subrutinas en ensamblador que se llaman desde C.
 */
void  updateBoardP1();
void  posCurScreenP1();
void  moveCursorP1();
void  searchSpaceNextToP1();
void  moveTileP1();
void  checkEndP1();
void  playP1();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en la fila indicada por la variable (rowScreen) y en 
 * la columna indicada por la variable (colScreen) de la pantalla.
 * 
 * Variables globales utilizadas:   
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP1' para 
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void gotoxyP1_C(){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter guardado en la variable (charac) en la pantalla, 
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que queremos mostrar.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'printchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Leer una tecla y guardar el carácter asociado en la variable (charac)
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * (charac): Carácter que leemos de teclado.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'getchP1' para
 * poder llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 **/
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándar (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

   /* ~ICANON para tratar la entrada del teclado carácter a carácter no como una línea completa acabada con /n
      ~ECHO para que no muestre el carácter leído*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. **/
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   charac = (char) getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Mostrar en la pantalla el menú del juego y pedir una opción.
 * Solo acepta una de las opciones correctas del menú ('0'-'8') o ESC.
 * 
 * Variables globales utilizadas:   
 * (developer): ((char *)&developer): variable definida en el código ensamblador.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : Opción elegida del menú, leída de teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 **/
void printMenuP1_C(){
   clearScreen_C();
   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();
   printf("                           \n");
   printf("        Developed by:      \n");
   printf("     ( %s )    \n",(char *)&developer);
   printf(" _________________________ \n");
   printf("|                         |\n");
   printf("|         MAIN MENU       |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|   1. updateBoard        |\n");
   printf("|   2. posCurScreen       |\n");
   printf("|   3. moveCursor         |\n");
   printf("|   4. searchSpaceNextTo  |\n");
   printf("|   5. moveTile           |\n");
   printf("|   6. checkEnd           |\n");
   printf("|   7. Play Game          |\n");
   printf("|   8. Play Game C        |\n");
   printf("|   0. Exit               |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|        OPTION:          |\n");
   printf("|_________________________|\n");  

    charac=' ';
    while (charac < '0' || charac > '8') {
      rowScreen = 20;
      colScreen = 18;
      gotoxyP1_C();
      getchP1_C();
      printchP1_C();
   }
   
}


/**
 * Mostrar el tablero de juego en la pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina en ensamblador equivalente.
 */
void printBoardP1_C(){

   rowScreen = 0;
   colScreen = 0;
   gotoxyP1_C();                                     //Filas
                                                     //Tablero
printf(" ___________________________________\n"); //01
   printf("|                                   |\n"); //02
   printf("|          24-PUZZLE  v1.0          |\n"); //03
   printf("|                                   |\n"); //04
   printf("|  Order the tiles alphabetically!  |\n"); //05
   printf("|___________________________________|\n"); //06
   printf("|                                   |\n"); //07
   //                 12  16  20  24  28              <- Columnas Tablero               
   printf("|          0   1   2   3   4        |\n"); //08
   printf("|        +---+---+---+---+---+      |\n"); //09
   printf("|      0 |   |   |   |   |   |      |\n"); //10
   printf("|        +---+---+---+---+---+      |\n"); //11
   printf("|      1 |   |   |   |   |   |      |\n"); //12
   printf("|        +---+---+---+---+---+      |\n"); //13
   printf("|      2 |   |   |   |   |   |      |\n"); //14
   printf("|        +---+---+---+---+---+      |\n"); //15
   printf("|      3 |   |   |   |   |   |      |\n"); //16
   printf("|        +---+---+---+---+---+      |\n"); //17
   printf("|      4 |   |   |   |   |   |      |\n"); //18
   printf("|        +---+---+---+---+---+      |\n"); //19
   printf("|                                   |\n"); //20
   printf("|         Remaining Moves [_]       |\n"); //21
   printf("|___________________________________|\n"); //22
   printf("|                                   |\n"); //23
   printf("|   (i)Up (j)Left (k)Down (l)Right  |\n"); //24
   printf("|   (Space) move tile  (ESC)  Exit  |\n"); //25
   printf("|___________________________________|\n"); //26
            
}


/**
 * Mostrar los valores de la matriz (tiles) en la pantalla, 
 * dentro del tablero en las posiciones correspondientes. 
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba abajo. 
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0]) 
 * a 24 (posición [4][4]) con incrementos de 1 porque los datos son de 
 * tipo char(BYTE) 1 byte, y para cada elemento de la matriz: 
 * Posicionar el cursor en el tablero gritando la función gotoxyP1_C. 
 * La posición inicial del cursor es la fila 10 de la pantalla (fila 0 
 * de la matriz), columna 12 de la pantalla (columna 0 de la matriz). 
 * Mostrar los caracteres de cada posición de la matriz (t) 
 * llamando la función printchP1_C. 
 * Actualizar la columna (colScreen) de 4 en 4 y al cambiar de fila 
 * (rowScreen) de 2 en 2. 
 * Mostrar los movimientos que quedan por hacer (m), recibidos como parámetro, 
 * dentro del tablero en la fila 21, columna 28 de la pantalla. 
 * 
 * Variables globales utilizadas: 
 * (tiles)    : Matriz donde guardamos los números del juego. 
 * (moves)   : Movimientos que quedan para ordenar las fichas. 
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor. 
 * (colScreen): Columna de la pantalla donde posicionamos el cursor. 
 * (charac)   : Índole a escribir en pantalla. 
 * 
 * Esta función no se llama desde ensamblador. 
 * Hay una subrutina en ensamblador equivalente 'updateBoardP1'.
 */
void updateBoardP1_C(){

   int i, j;
   rowScreen = 10;
   for (i=0;i<DimMatrix;i++){
	  colScreen = 12;
      for (j=0;j<DimMatrix;j++){
         gotoxyP1_C();
         charac = tiles[i][j];
         printchP1_C();
         colScreen = colScreen + 4;
      }
      rowScreen = rowScreen + 2;
   }
   
   rowScreen = 21;
   colScreen = 28;
   gotoxyP1_C();
   charac = (char)moves;
   charac = charac + '0';
   printchP1_C();

}

/**
 * Posicionar el cursor en la pantalla, dentro del panel, en función de 
 * la variable (pos). 
 * Para calcular la posición del cursor a pantalla (rowScreen) y 
 * (colScreen) utilizar estas fórmulas: 
 * rowScreen=10+(pos/DimMatrix)*2 
 * colScreen=12+(pos%DimMatrix)*4 
 * Para posicionar el cursor llamar a la función gotoxyP1_C. 
 * 
 * Variables globales utilizadas: 
 * (pos)      : Posición del cursor en la matriz (tiles). 
 * (rowScreen): Fila para posicionar el cursor en la pantalla. 
 * (colScreen): Columna para posicionar el cursor en la pantalla. 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'posCurScreenP1'.
 */
void posCurScreenP1_C() {

   rowScreen=10+((int)pos/DimMatrix)*2;
   colScreen=12+((int)pos%DimMatrix)*4;
   gotoxyP1_C();
   
}


/**
 * Mover el cursor en la dirección indicada por la variable (charac), 
 * ('i':arriba, 'k':bajo, 'j':izquierda o 'l':derecha). 
 * Actualizar la posición (pos) controlando que 
 * no salga del tablero. 
 * 
 * No se debe posicionar el cursor en la pantalla. 
 * 
 * Variables globales utilizadas: 
 * (charac): Carácter leído de teclado. 
 * (pos)   : Posición del cursor en la matriz (tiles). 
 * 
 * Esta función no se llama desde ensamblador. 
 * Hay una subrutina en ensamblador equivalente 'moveCursorP1'.
 */
void moveCursorP1_C(){

   int i = (int)pos/DimMatrix;
   int j = (int)pos%DimMatrix;
   
   switch(charac){
      case 'i':
         if (i > 0) {
			pos = pos - DimMatrix;
         }
      break;
      case 'k':
         if (i < (DimMatrix-1)) {
			pos = pos + DimMatrix;
         }
      break;
      case 'j':
         if (j > 0) {
			pos = pos - 1;
         }
      break;
      case 'l':
         if (j < (DimMatrix-1)) {
			pos = pos + 1;
         }
      break;
   }

}


/**
 * Busca si el espacio en blanco está a la izquierda, a la derecha, arriba o abajo 
 * de la posición del cursor (pos) dentro de la matriz (tiles). 
 * No mirar si hay espacio, si la posición está fuera del tablero. 
 * Si la posición del espacio está al lado de la posición del cursor 
 * actualizar la variable (spacePos) con la posición del espacio. 
 * Si no está junto a la posición del cursor (spacePos=SizeMatrix). 
 * 
 * Variables globales utilizadas: 
 * (tiles)   : Matriz donde guardamos las fichas del juego. 
 * (pos)     : Posición del cursor en la matriz (tiles). 
 * (spacePos): Posición del espacio dentro de la matriz. 
 * 
 * Esta función no se llama desde ensamblador. 
 * Hay una subrutina en ensamblador equivalente 'getSpacePosP1'.
 */
void searchSpaceNextToP1_C(){

   spacePos = SizeMatrix;
   int  row = (int)pos/DimMatrix;
   int  col = (int)pos%DimMatrix;
   
   if(col>0           && tiles[row][col-1]==' ') spacePos = pos-1;//left
   if(col<DimMatrix-1 && tiles[row][col+1]==' ') spacePos = pos+1;//right
   if(row>0           && tiles[row-1][col]==' ') spacePos = pos-DimMatrix;//up
   if(row<DimMatrix-1 && tiles[row+1][col]==' ') spacePos = pos+DimMatrix;//down
    
}


/**
 * Mover la ficha de la casilla donde está el cursor (pos), 
 * en la casilla donde se encuentra el espacio (spacePos), 
 * obtenida llamando la función searchSpaceNextToP1_C. 
 * 
 * Si la casilla donde está el espacio (spacePos), está al lado 
 * (arriba, abajo, izquierda o derecha) de la casilla donde 
 * está el cursor (pos) en la matriz (tiles) (spacePos!=SizeMatrix): 
 * - Mover la ficha donde está el cursor a la casilla donde está el espacio y 
 * poner el espacio en la casilla donde está el cursor, en la matriz (tiles). 
 * Si no, no hacer el movimiento y poner (state=2). 
 * 
 * No se debe mostrar la matriz con los cambios, se hace en updateBoardP1_C(). 
 * 
 * Variables globales utilizadas: 
 * (tiles)   : Matriz donde guardamos las fichas del juego. 
 * (pos)     : Posición del cursor en la matriz (tiles). 
 * (spacePos): Posición del espacio en la matriz (tiles). 
 * (state)   : Estado del juego. 
 * 
 * Esta función no se llama desde ensamblador. 
 * En la subrutina de ensamblador equivalente 'moveTilesP1'.
 */
void moveTileP1_C(){

   int rowCur =((int)pos/DimMatrix);// En ensamblador no es necesario realizar este cálculo. 
   int colCur =((int)pos%DimMatrix);// Para accedido a la matriz tiles se utiliza pos.
   
   searchSpaceNextToP1_C();
   
   int rowSpace=((int)spacePos/DimMatrix);// En ensamblador no es necesario realizar este cálculo. 
   int colSpace=((int)spacePos%DimMatrix);// Para acceder a la matriz tiles se utiliza spacePos.
   if ( spacePos != SizeMatrix ){	 

      tiles[rowSpace][colSpace]	= tiles[rowCur][colCur];
      tiles[rowCur][colCur]		= ' ';
      
   } else {
      state = 2;
   }

}


/**
 * Verificar si hemos terminado el juego. 
 * Comprobar si quedan movimientos (moves==0), si no quedan movimientos, 
 * poner (state=3) para indicar que se ha perdido. 
 * 
 * Variables globales utilizadas: 
 * (moves): Movimientos que quedan para ordenar las fichas. 
 * (state): Estado del juego. 
 * 3: Pierdes, No quedan movimientos. 
 * 
 * Esta función no se llama desde ensamblador. 
 * Hay una subrutina en ensamblador equivalente 'checkEndP1'.
 */
void checkEndP1_C() {
   
   if (moves == 0) {
      state = 3;
   } 
   
}


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state). 
 * (state): 0: Salir, hemos pulsado la tecla 'ESC' para salir. 
 *          1: Continuamos jugando. 
 *          2: No se ha podido hacer el movimiento. 
 *          3: Pierdes, No quedan movimientos. 
 * 
 * Variables globales utilizadas: 
 * (state)    : Estado del juego. 
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor. 
 * (colScreen): Columna de la pantalla donde posicionamos el cursor. 
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printMessageP1' para 
 * llamar a esta función guardando el estado de los registros del procesador. 
 * Esto se hace porque las funciones de C no mantienen el estado de los 
 * registros.
 */
void printMessageP1_C() {

   rowScreen = 27;
   colScreen = 2;
   gotoxyP1_C();
   
   switch(state){
      case 0:
         printf("<<<<<<< EXIT: (ESC) Pressed >>>>>>>");
      break;
      case 1:
         printf("===========  NEXT MOVE  ===========");
      break;
      case 2:
         printf("**********  CAN'T  MOVE  **********");
      break;
      case 3:
         printf("------ SORRY, NO MORE MOVES! ------");
      break;
   }
   
}


/**
 * Juego del 24-PUZZLE. 
 * Función principal del juego. 
 * Permite jugar al juego del 24-PUZZLE gritando todas las funcionalidades. 
 * 
 * Pseudo-código: 
 * Mostrar el tablero de juego llamando a la función printBoardP1_C. 
 * Inicializar el estado del juego, (state=1). 
 * Inicializar los movimientos que se pueden realizar (moves = 9). 
 * Actualizar el tablero de juego llamando a la función updateBoardP1_C. 
 * 
 * Mientras (state==1) hacer: 
 *   Posicionar el cursor dentro del tablero llamando a la función posCurScreenP1_C. 
 *   Leer una tecla (charac) llamando la función getchP1_C. 
 *   Según la tecla leída haremos lo siguiente: 
 *    - ['i','j','k' o 'l'] desplazar el cursor según la dirección 
 *      elegida llamando la función moveCursorP1_C. 
 *    - [' '] Mover la ficha de la casilla donde está el cursor, a la 
 *      casilla donde está el espacio llamando la función moveTileP1_C. 
 *      Si se puede hacer el movimiento (state!=2) decrementar 
 *      los movimientos que quedan (moves). 
 *    - '<ESC>' (código ASCII 27) poner (state = 0) para salir. 
 *    Actualizar el tablero de juego llamando a la función updateBoardP1_C. 
 *    Verificar si se han agotado los movimientos llamando la función checkEndP1_C. 
 *    Mostrar un mensaje debajo del tablero según el valor de la 
 *    variable (state) llamando la función printMessageP1_C. 
 *    Si no se ha podido hacer movimiento (state == 2) poner 
 *    (state = 1) para continuar jugando. 
 * Fin mientras. 
 * 
 * Antes de salir, esperar a que se pulse una tecla llamando la función 
 * getchP1_C para terminar. 
 * 
 * Variables globales utilizadas: 
 * (muevas)   : Movimientos que quedan para ordenar las fichas. 
 * (spacePos) : Posición del espacio en la matriz (tiles). 
 * (cursorPos): Posición del cursor en la matriz (tiles). 
 * (charac)   : Opción leída de teclado. 
 * (state)    : Estado del juego. 
 *              1: Continuamos jugando. 
 *              2: No se ha podido hacer el movimiento. 
 *              3: Pierdes, No quedan movimientos 
 * 
 * Esta función no se llama desde ensamblador. 
 * Se ha definido una subrutina de ensamblador equivalente 'playP1' para 
 * llamar a las subrutinas del juego definidas en ensamblador.
 */
void playP1_C(){

   printBoardP1_C();
   pos   = 24;
   spacePos = 0;
   state = 1;
   moves = 9;
   
   updateBoardP1_C();
   
   while (state == 1) {     //Bucle principal.
	  
	  posCurScreenP1_C(); 
      getchP1_C();                   
      if (charac >= 'i' && charac <= 'l') {
		 moveCursorP1_C();
      } else if (charac == ' ') {
		  moveTileP1_C();
   	      if (state != 2) {
              moves--;
          }
      } else if (charac == 27) {
            state = 0;
      }
      
      updateBoardP1_C();
     
      checkEndP1_C();
      printMessageP1_C();
      if (state == 2) state = 1; 
      
   }
   getchP1_C();     
   
}


/**
 * Programa Principal 
 * 
 * ATENCIÓN: Se puede probar la funcionalidad de las subrutinas que se deben 
 * desarrollar sacando los comentarios de la llamada a la función 
 * equivalente implementada en C que hay debajo en cada opción. 
 * Para el juego completo hay una opción para la versión en ensamblador y 
 * una opción por el juego en C.
 */
int main(void){
	  
   charac = ' ';
   while (charac != '0') {
     printMenuP1_C();   
     switch(charac){
        case '1'://Actualizar el contenido del tablero.     
          printBoardP1_C();      
          //=======================================================
            updateBoardP1();
            ///updateBoardP1_C();
          //=======================================================
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press any key ...");
          getchP1_C();
        break;
        case '2'://Posicionar el cursor dentro del tablero.  
          printBoardP1_C();
          updateBoardP1_C();
          pos = 24; 
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press any key ..."); 
          //=======================================================
             posCurScreenP1();
             ///posCurScreenP1_C();
          //=======================================================
          getchP1_C();
        break;
        case '3': //Mover el cursor en la dirección indicada.  
          printBoardP1_C();
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press i,j,k,l: ");
          updateBoardP1_C();
          pos = 24;
          posCurScreenP1_C();
          getchP1_C();   
          if (charac>='i' && charac<='l') {
          //=======================================================
            moveCursorP1();
            ///moveCursorP1_C();
          //=======================================================
          }
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          if(pos!=24){
             printf(" Press any key ...");
             posCurScreenP1_C();
          } else { 
			 printf("  Can't move!!!   ");
		  }
          getchP1_C();
        break;
        case '4'://Buscar donde está el espacio en blanco dentro de la matriz (tiles).
          printBoardP1_C();
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          printf("  Press <Space>    ");
          updateBoardP1_C();
          pos = 19; ///cursorPos=20; no pot fer moviment
          posCurScreenP1_C();
          getchP1_C();   
          if (charac!=' ') {
			 rowScreen = 27;
             colScreen = 12;
             gotoxyP1_C();
			 printf("Incorrect key ...");
		  } else {
          //=======================================================        
            searchSpaceNextToP1();
            ///searchSpaceNextToP1_C();
          //=======================================================
             if (pos < SizeMatrix) {
				rowScreen = 27;
                colScreen = 12;
                gotoxyP1_C();
				printf(" Press any key ...");
				pos = spacePos; 
				posCurScreenP1_C();
			 } else {
				rowScreen = 27;
                colScreen = 1;
                gotoxyP1_C();
				printf("ERROR->No SPACE next to this position");
			 }
	      } 
          getchP1_C();
        break;
        case '5'://Mover la ficha donde hay el espacio.  
          printBoardP1_C();
          rowScreen = 27;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press <SPACE>: ");
          updateBoardP1_C();
          pos = 19; ///pos=20; no puede mover
          posCurScreenP1_C();
          getchP1_C();   
          if (charac>=' ') {
          //=======================================================
            moveTileP1();
            ///moveTileP1_C();
          //=======================================================
	      }
          updateBoardP1_C();
          if (state != 2) {
             rowScreen = 27;
             colScreen = 12;
             gotoxyP1_C();
             printf(" Press any key ...");
             posCurScreenP1_C();
          } else {			 
             printMessageP1_C();
          }
          getchP1_C();
        break;
        case '6': // Verificar el estado del juego.
          printBoardP1_C();    
          moves = 0;
          updateBoardP1_C();
          posCurScreenP1_C();
          //=======================================================
            checkEndP1();
            ///checkEndP1_C();
          //=======================================================
          printMessageP1_C();
          getchP1_C();  
        break;
        case '7': //Juego completo en Ensamblador. 
          //=======================================================
            playP1();
          //=======================================================
        break;
        case '8': //Juego completo en C.    
          //=======================================================
            playP1_C();
          //=======================================================
        break;
     }
  }
  printf("\n\n");
  
  return 0;
  
}
