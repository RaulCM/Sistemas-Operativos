# Sistemas-Operativos

Prácticas correspondientes a la asignatura de Sistemas Operativos

#### Comando para compilar
gcc -o nombre_programa -Wall -Wshadow -g nombre_programa.c

## Ejercicio 1
### Programa básico C

Escribe un programa llamado **reverse.c** en C para Linux que escriba todos sus argumentos dados la vuelta en la salida, cada uno en una línea.

Por ejemplo:

**$> ./reverse uno dos tres cuatro**\
**onu**\
**sod**\
**sert**\
**ortauc**\
**$>**


## Ejercicio 2
### downpath

Escribe un programa llamado **downpath.c** en C para Linux que escriba por su salida la ruta absoluta de un fichero que esté en el directorio $HOME/Downloads cuyo nombre tiene que estar formado por el nombre del usuario en mayúsculas y el PID del proceso, separados por un punto.\
Un ejemplo de ejecución para un usuario llamado "pepe" cuyo home es "/home/gsyc/p" es:\
**$> ./downpath**\
**/home/gsyc/p/Downloads/PEPE.31444**

El programa debe implementar una función con la siguiente cabecera:\
**int makepath(char *buf, int len);**\
La función recibe como argumento el buffer donde se almacenará la string y su tamaño, y retorna el tamaño de la string creada (sin contar el carácter terminador). En caso de error, debe retornar un valor negativo.

## Ejercicio 3
### dumptree

Escribe un programa llamado **dumptree.c** en C para Linux que, a partir de un directorio, liste todos los ficheros y directorios recursivamente. Además, para los ficheros convencionales, el programa debe escribir todo su contenido.\
Debe admitir como mucho un argumento con el path del directorio que se quiere listar. Si no se proporciona un argumento, el programa listará el directorio actual.\
El orden de los directorios y ficheros en el listado no importa (pero tienen que aparecer todos).

Por ejemplo:

**$> mkdir -p /tmp/a/b/c /tmp/a/d**\
**$> echo HOLA PEPE> /tmp/a/b/x.txt**\
**$> echo ADIOS PEPE > /tmp/a/b/c/y.txt**\
**$> ./dumptree /tmp/a**\
**/tmp/a**\
**/tmp/a/b**\
**/tmp/a/b/c**\
**/tmp/a/b/c/y.txt**\
**ADIOS PEPE**\
**/tmp/a/b/x.txt**\
**HOLA PEPE**\
**/tmp/a/d**\

## Ejercicio 4
### cprec

Escribe un programa **cprec.c** que, dados dos números en octal (permisos para directorios y permisos para ficheros), un path origen y un path destino, realice una copia recursiva de un fichero o directorio, dejando los ficheros y los directorios de la copia con los permisos que describen los números en octal.\
El path del destino no debe existir y será el path del nuevo fichero o directorio. El programa debe funcionar para cualquier origen y destino, no se puede utilizar **rename** ni enlaces de ningún tipo.\
No está permitido utilizar programas externos y la copia debe realizarse sin utilizar las funciones de stdio.h.

Ejemplo:

**$cprec 0777 0555 a.txt b.txt**\
**$ls**\
**-r-xr-xr-x  1 paurea  paurea  0 Oct  9 11:34 a.txt**\
**-r-xr-xr-x  1 paurea  paurea  0 Oct  9 11:34 b.txt**

**$mkdir -p /tmp/o/a/b /tmp/o/c/d**\
**$echo hola >> /tmp/o/e.txt**\
**$touch /tmp/o/r.bla**\
**$cprec 0511 0333 /tmp/o /tmp/dest**\
**$du -a /tmp/dest**\
**0    /tmp/dest/a/b**\
**0    /tmp/dest/a**\
**0    /tmp/dest/c/d**\
**0    /tmp/dest/c**\
**8    /tmp/dest/e.txt**\
**0    /tmp/dest/r.bla**\
**8    /tmp/dest**

**$ ls -la /tmp/dest**\
**total 8**\
**dr-x--x--x  4 paurea  paurea  204 Oct  9 11:37 .**\
**drwxrwxrwt  8 root    root  442 Oct  9 11:38 ..**\
**dr-x--x--x  3 paurea  paurea  102 Oct  9 11:36 a**\
**dr-x--x--x  3 paurea  paurea  102 Oct  9 11:36 c**\
**--wx-wx-wx  1 paurea  paurea    5 Oct  9 11:38 e.txt**\
**--wx-wx-wx  1 paurea  paurea    0 Oct  9 11:37 r.bla**

## Ejercicio 5
### ccall

Escribe un programa en C llamado **ccall.c** que, dado un directorio indicado como argumento, o utilizando el directorio actual si no hay argumentos, compile y enlace los ficheros cuyo nombre termine en ".c" empleando el compilador de C.\
El programa ha de hacer que los ejecutables resultantes de la compilación tengan como nombre el mismo del fichero fuente, pero sin el ".c" del mismo.\
Se desea que se compilen simultáneamente todos los ficheros encontrados, de tal forma que tengamos los compiladores ejecutando a la vez si es posible.\
El programa debe escribir una línea para cada compilación que termine, indicando si se ha conseguido compilar o no y el nombre el fichero que se ha compilado.\
Si la variable de entorno "CFLAGS" está definida, debe usarse dicha variable como opción para el compilador (y puede suponerse que el valor de la variable no contiene blancos.

Por ejemplo, dado un directorio /tmp/testdir, con ficheros "fich1.c", "fichxx", y "fich2.c", podríamos tener la siguiente sesión.\
En esta sesión y en el resto de ejemplos omitimos las líneas que ha escrito el compilador de C al invocarlo, por claridad.

**unix$ ccall /tmp/testdir**\
**fich1.c: no compila**\
**fich2.c: compila**\
**unix$**

Otro ejemplo...

**unix$ export CFLAGS=-Wall**\
**unix$ cd /tmp/testdir**\
**unix$ ccall**\
**fich1.c: no compila**\
**fich2.c: compila**

En el primer ejemplo, se ha ejecutado

**gcc -o fich1 fich1.c**\
**gcc -o fich2 fich2.c**

desde el programa. En el segundo, se ha ejecutado

**gcc -Wall -o fich1 fich1.c**\
**gcc -Wall -o fich2 fich2.c**

## Ejercicio 6
### redirección

Reescribe el ejercicio anterior para que reciba un argumento adicional que sea una palabra. Cuando se pasan dos argumentos, el primero será el directorio y el segundo la palabra. En esta versión del programa, se deben escribir **únicamente las líneas de error** del compilador que contengan dicha palabra.\
Para filtrar la palabra debe usar *grep* y no se permite usar ficheros intermedios ni se permite ejecutar un shell (p. ej. usando *system*).\
El programa se debe llamar **redir.c**.

## Ejercicio 7
## shell scripting

Escribe un script de shell llamado ccall.sh para Linux que haga lo mismo que el programa anterior. Sólo hay dos diferencias respecto al enunciado anterior: la forma de procesar los argumentos y que no se debe imprimir si el fichero ha compilado o no.\
Las opciones de compilación (si las hay) se especifican en un argumento del script, no mediante la variable de entorno CFLAGS. Para proporcionar opciones de compilación, el usuario usará el flag -c seguido de un único argumento con las opciones que desee para compilar.  El directorio con los ficheros fuente se especifica con el flag -d seguido de la ruta. La expresión regular para filtrar la salida de errores de gcc es un argumento opcional y no utiliza flags. No se puede suponer un orden en los argumentos salvo la expresión regular, que se puede suponer que va al final, y no es obligatorio usar ningún flag. Los flags pueden aparecer, como mucho, una vez.\
Si se usa mal el script, este debe escribir un mensaje informativo de su uso en la salida de error y terminar con un estatus adecuado.

Por ejemplo, esta podría ser una ejecución del script:

**$> ccall.sh -c "-Wall -Wshadow" -d /tmp/testdir undeclared**\
**fich1.c:23:6: error: 'i' undeclared (first use in this function)**

Que sería exactamente igual que ejecutar:

**$> ccall.sh -d /tmp/testdir -c "-Wall -Wshadow" undeclared**\
**fich1.c:23:6: error: 'i' undeclared (first use in this function)**

Para compilar los ficheros fuente del directorio actual sin opciones de compilación y filtrando la salida:

**$> cd /tmp/testdir**\
**$> ccall.sh undeclared**\
**fich1.c:23:6: error: 'i' undeclared (first use in this function)**

Para lo mismo, pero sin filtrar nada:

**$> cd /tmp/testdir**\
**$> ccall.sh**\
**fich1.c:17:7: error: default argument for parameter of type int has type void**\
**fich1.c:23:6: error: 'i' undeclared (first use in this function)**
