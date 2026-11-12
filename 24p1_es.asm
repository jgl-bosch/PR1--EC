section .note.GNU-stack noalloc noexec nowrite progbits
section .data               
;Cambiar Nombre y Apellido por vuestros datos.
developer db "Jesus_Iglesias",0

;Constante que también está definida en C.
DimMatrix    equ 5      
SizeMatrix   equ DimMatrix*DimMatrix ;=25 

section .text            
;Variables definidas en ensamblador.
global developer                        

;Subrutinas de ensamblador que se llaman desde C.
global updateBoardP1, posCurScreenP1, moveCursorP1, searchSpaceNextToP1
global moveTileP1, checkEndP1, playP1

;Variables globales definidas en C.
extern pos, spacePos, state, moves, rowScreen, colScreen, charac
extern tiles

;Funciones de C que se llaman desde ensamblador.
extern clearScreen_C, printBoardP1_C, gotoxyP1_C, printchP1_C, getchP1_C
extern printMessageP1_C


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATENCIÓN: Recordad que en ensamblador las variables y los parámetros 
;;   de tipo 'char' deben asignarse a registros de tipo  
;;   BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;;   los de tipo 'short' deben asignarse a registros de tipo 
;;   WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;;   los de tipo 'int' deben asignarse a registros de tipo 
;;   DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;;   los de tipo 'long' deben asignarse a registros de tipo 
;;   QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Las subrutinas en ensamblador que debéis implementar son:
;;   updateBoardP1, posCurScreenP1, moveCursorP1
;;   searchSpaceNextTo, moveTileP1, checkEndP1
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Posicionar el cursor en la fila indicada por la variable (rowScreen) y 
; en la columna indicada por la variable (colScreen) de la pantalla,
; llamando a la función gotoxyP1_C.
; 
; Variables globales utilizadas:   
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call gotoxyP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Mostrar un carácter guardado en la variable (charac) en la pantalla, 
; en la posición donde está el cursor, llamando a la función printchP1_C.
; 
; Variables globales utilizadas:   
; (charac): Carácter que queremos mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printchP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se proporciona ya hecha. NO LA PODÉIS MODIFICAR.
; Leer una tecla y guardar el carácter asociado en la variable (charac)
; sin mostrarlo en pantalla, llamando a la función getchP1_C. 
; 
; Variables globales utilizadas:   
; (charac): Carácter que leemos de teclado.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call getchP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar los valores de la matriz (tiles), en pantalla,
; dentro del tablero en las posiciones correspondientes.
; Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
; Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
; a 24 (posición [4][4]) con incrementos de 1 porque los datos son de
; tipo char(BYTE) 1 byte, y para cada elemento de la matriz:
; Posicionar el cursor en el tablero llamando a la subrutina gotoxyP1.
; La posición inicial del cursor es la fila 10 de la pantalla (fila 0
; de la matriz), columna 12 de la pantalla (columna 0 de la matriz).
; Mostrar los caracteres de cada posición de la matriz (tiles)
; llamando a la subrutina printchP1.
; Actualizar la columna (colScreen) de 4 en 4 y al cambiar de fila
; (rowScreen) de 2 en 2.
; Mostrar los movimientos que quedan por hacer (moves) dentro del tablero
; en la fila 21, columna 28 de la pantalla.
;
; Variables globales utilizadas:
; (tiles)    : Matriz donde guardamos los números del juego.
; (moves)    : Movimientos que quedan para ordenar las fichas.
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
; (charac)   : Carácter a escribir en pantalla.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
updateBoardP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.

   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi

   mov rsi, 0		; indice filas
   mov rdi, 0		; indice columnas
   
   mov DWORD [rowScreen], 10  ;posicion fila , tipo int
   mov DWORD [colScreen], 12 	;posicion columna, tipo in

   ; Recorrer los valores de la matriz
   matrix_loop:
	
	mov al, BYTE [tiles + rsi + rdi]
	mov BYTE [charac], al		; almacenar el caracter de la matriz
	
	call gotoxyP1
	
	call printchP1
	
	inc rdi
	add DWORD [colScreen], 4	; espaciado columnas
	
	cmp rdi, DimMatrix	
	jl matrix_loop
	
	mov rdi, 0 
	
	add rsi, DimMatrix		; cambio a siguiente fila
	mov DWORD [colScreen], 12 	;posicion columna, tipo in
	
	add DWORD [rowScreen], 2		; espaciado filas
	
	cmp rsi, SizeMatrix
	jl matrix_loop
	
	mov DWORD [rowScreen], 21  
    mov DWORD [colScreen], 28 

	mov al, WORD [moves]
	
	call gotoxyP1
	
	call printchP1
      
   updateBoardP1_end:
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop rdi  
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Posicionar el cursor en la pantalla, dentro del tablero, en función de
; ls variable (pos).
; Para calcular la posición del cursor a pantalla (rowScreen) y
; (colScreen) utilizar estas fórmulas:
; rowScreen=10+(pos/DimMatrix)*2
; colScreen=12+(pos%DimMatrix)*4
; Para posicionar el cursor llamar a la subrutina gotoxyP1.
;
; Variables globales utilizadas:
; (pos)      : Posición del cursor en la matriz (tiles).
; (rowScreen): Fila para posicionar el cursor en la pantalla.
; (colScreen): Columna para posicionar el cursor en la pantalla.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
posCurScreenP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.
   push rax
   push rbx
   
   mov eax, DWORD[pos]
   mov ecx, DimMatrix
   
   div ecx			; divison eax/ecx -- eax cociente edx resto
   
   ;rowScreen   
   mov ebx, eax		;cociente divison
   imul ebx, 2
   add ebx, 10
   mov DWORD [rowScreen], ebx
   
   ;colScreen
   mov ebx, edx		;resto divison
   imul ebx, 4
   add ebx, 12
   mov DWORD [colScreen], ebx
   
   posCurScreenP1_end:
   ;restaurar el estado de los registros que se han guardado en la pila.  
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mover el cursor en la dirección indicada por la variable (charac),
; ('i':arriba, 'k':abajo, 'j':izquierda o 'l':derecha).
; Actualizar la posición (pos) controlando que
; no salga del tablero.
;
; No debe posicionarse el cursor en pantalla.
;
; Variables globales utilizadas:
; (charac): Carácter leído de teclado.
; (pos) : Posición del cursor en la matriz (tiles).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
moveCursorP1:
   push rbp
   mov  rbp, rsp 
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.
   push rax
   push rbx
   
  
   mov al, BYTE [charac]
   mov eax, DWORD[pos]
   mov ecx, DimMatrix
   
   div ecx			; divison eax/ecx -- eax fila edx columna
   
   ; Eleccion de direccion
   cmp bl,'i'
   je move_up
   
   cmp bl,'k'
   je move_down
   
   cmp bl,'j'
   je move_left
   
   cmp bl,'l'
   je move_right
   
   jmp moveCursorP1_end
   
   
   move_up:
	cmp eax,1
	jl moveCursorP1_end
	sub eax, 1
	imul eax, 5
	add eax, edx
	mov DWORD [pos], eax
	
   move_down:
    cmp eax,4
    jge moveCursorP1_end
    add eax,1
    imul eax, 5
	add eax, edx
	mov DWORD [pos], eax
   
   move_left:
    cmp edx,1
	jl moveCursorP1_end
	sub edx, 1
	imul eax, 5
	add eax, edx
	mov DWORD [pos], eax
	
   move_right:
    cmp edx,4
	jge moveCursorP1_end
	add edx, 1
	imul eax, 5
	add eax, edx
	mov DWORD [pos], eax

   moveCursorP1_end:
   ;restaurar el estado de los registros que se han guardado en la pila.
   
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Busca si el espacio en blanco está a la izquierda, a la derecha, arriba 
; o abajo de la posición del cursor (pos) dentro de la matriz (tiles).
; No mirar si hay espacio, si la posición está fuera del tablero.
; Si la posición del espacio está al lado de la posición del cursor
; actualizar la variable (spacePos) con la posición del espacio.
; Si no está junto a la posición del cursor (spacePos=SizeMatrix).
;
; Variables globales utilizadas:
; (tiles)   : Matriz donde guardamos las fichas del juego.
; (pos)     : Posición del cursor en la matriz (tiles).
; (spacePos): Posición del espacio dentro de la matriz.
;
; Esta función no se llama desde ensamblador.
; Hay una subrutina en ensamblador equivalente 'searchSpaceNextToP2',
; el paso de parámetros es equivalente.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
searchSpaceNextToP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.               
   
       

   searchSpaceNextToP2_end:

   ;restaurar el estado de los registros que se han guardado en la pila.
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mover la ficha de la casilla en la que está el cursor (pos)
; a la casilla donde se encuentra el espacio (spacePos),
; obtenida llamando la subrutina searchSpaceNextToP1.
;
; Si la casilla donde está el espacio (spacePos), está al lado
; (arriba, abajo, izquierda o derecha) de la casilla donde
; está el cursor (pos) en la matriz (tiles) (spacePos!=SizeMatrix):
; - Mover la ficha donde está el cursor a la casilla donde está el espacio y
; poner el espacio en la casilla está el cursor, en la matriz (tilas).
; Si no, no realizar el movimiento y poner (state=2).
;
; No se debe mostrar la matriz con los cambios, se realiza en updateBoardP1.
;
; Variables globales utilizadas:
; (Tilas)   : Matriz donde guardamos las fichas del juego.
; (pos)     : Posición del cursor en la matriz (tiles).
; (spacePos): Posición del espacio en la matriz (tiles).
; (state)   : Estado del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
moveTileP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.
   
   

   moveTileP2_End:
   
   ;restaurar el estado de los registros que se han guardado en la pila.
   mov rsp, rbp
   pop rbp
   ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verificar si hemos terminado el juego.
; Comprobar si quedan movimientos (moves==0), si no quedan movimientos,
; poner (state=3) para indicar que se ha perdido.
;
; Variables globales utilizadas:
; (moves): Movimientos que quedan para ordenar las fichas.
; (state): Estado del juego.
;          3: Pierdes, No quedan movimientos.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkEndP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.
   
   
   
   checkEndP1_end:
   ;restaurar el estado de los registros que se han guardado en la pila.
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO SE PUEDE MODIFICAR.
; Mostrar un mensaje debajo del tablero según el valor de la variable
; (state) llamando a la función printMessageP1_C().
;
; Variables globales utilizadas:
; (state)    : Estado del juego.
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printMessageP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   
   call printMessageP1_C
 
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO SE PUEDE MODIFICAR.
; Juego del 24-PUZZLE.
; Función principal del juego.
; Permite jugar al juego del 24-PUZZLE llamando a todas las funcionalidades.
;
; Pseudo-código:
; Mostrar el tablero de juego llamando a la función printBoardP1_C.
; Inicializar el estado del juego, (state=1).
; Inicializar los movimientos que se pueden realizar (moves = 9).
; Actualizar el tablero de juego llamando a la subrutina updateBoardP1.
;
; Mientras (state==1) hacer:
;   Posicionar el cursor en el tablero llamando a la subrutina posCurScreenP1.
;   Leer una tecla (charac) llamando a la subrutina getchP1.
;   Según la tecla leída haremos lo siguiente:
;    - ['i','j','k' o 'l'] desplazar el cursor según la dirección
;      elegida llamando la subrutina moveCursorP1.
;    - [' '] Mover la ficha de la casilla donde está el cursor, a la 
;      casilla donde está el espacio llamando a la subrutina moveTileP1.
;      Si se puede hacer el movimiento (state!=2) decrementar
;      los movimientos que quedan (moves).
;    - '<ESC>' (código ASCII 27) poner (state = 0) para salir.
;   Actualizar el tablero de juego llamando a la subrutina updateBoardP1.
;   Verificar si se han agotado los movimientos llamando a la subrutina checkEndP1.
;   Mostrar un mensaje debajo del tablero según el valor de la
;   variable (state) llamando a la subrutina printMessageP1.
;   Si no se ha podido hacer movimiento (state == 2) poner
;   (state = 1) para continuar jugando.
; Fin mientras.
;
; Antes de salir, esperar a que se pulse una tecla llamando a 
; la subrutina getchP1 para terminar.
;
; Variables globales utilizadas:
; (mmoves)  : Movimientos que quedan para ordenar las fichas.
; (SpacePos): Posición del espacio en la matriz (tiles).
; (pos)     : Posición del cursor en la matriz (tiles).
; (charac)  : Opción leída de teclado.
; (state)   : Estado del juego.
;             1: Continuamos jugando.
;             2: No se ha podido realizar el movimiento.
;             3: Pierdes, No quedan movimientos.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
playP1:
   push rbp
   mov  rbp, rsp
   ;guardar el estado de los registros que se modifican en esta 
   ;subrutina y que no se utilizan para devolver valores.  
   push rax
     
   call printBoardP1_C   ;printBoard1_C();
     

   mov QWORD[pos],  24        ;pos = 24
   mov QWORD[spacePos], 0     ;spacePos = 0;
   mov BYTE[state], 1         ;state = 1;
   mov WORD[moves], 9         ;moves = 9
   
   call updateBoardP1         ;updateBoardP1_C();   
      
   playP2_While:
   cmp  BYTE[state], 1                 ;while (state == 1)
   jne  playP2_EndWhile       ;//Bucle principal.

      call posCurScreenP1     ;posCurScreenP1_C(); 
            
      call getchP1            ;getchP1_C()
      mov al, BYTE[charac]
      playP2_ReadKey_ijkl:
      cmp al, 'i'
      jl  playP2_ReadKey_Space;(charac>='i')
      cmp al, 'l'
      jg  playP2_ReadKey_Space;(charac<='l')
         call moveCursorP1    ;moveCursorP1_C();
         jmp playP2_ReadKey_End
      playP2_ReadKey_Space:
      
      cmp al, ' '
      jne playP2_ReadKey_ESC  ;(charac==' ')
         call moveTileP1      ;moveTileP1_C();
         cmp BYTE[state], 2   ;if (state != 2) {
         je  playP2_ReadKey_ESC
            dec WORD[moves]   ;moves--;
            jmp playP2_ReadKey_End
      playP2_ReadKey_ESC:
      
      cmp al, 27              ;if (charac==27)
      jne playP2_ReadKey_End
         mov BYTE[state], 0   ;state = 0  
      playP2_ReadKey_End:
      

      call updateBoardP1      ;updateBoardP1_C();

      call checkEndP1         ;state = checkEndP1_C();
      
      call printMessageP1     ;printMessageP1_C();
      cmp  BYTE[state], 2     ;if (state == 2) 
      jne playP2_While
         mov BYTE[state], 1   ;state = 1;
   jmp playP2_While
   playP2_EndWhile:

   call getchP1               ;getchP1_C();
   
   ;restaurar el estado de los registros que se han guardado en la pila.
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret
