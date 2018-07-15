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
### shell scripting

Escribe un script de shell llamado **ccall.sh** para Linux que haga lo mismo que el programa anterior. Sólo hay dos diferencias respecto al enunciado anterior: la forma de procesar los argumentos y que **no se debe imprimir si el fichero ha compilado o no**.\
Las opciones de compilación (si las hay) se especifican en un argumento del script, no mediante la variable de entorno CFLAGS. Para proporcionar opciones de compilación, el usuario usará el flag -c seguido de **un único** argumento con las opciones que desee para compilar.  El directorio con los ficheros fuente se especifica con el flag -d seguido de la ruta. La expresión regular para filtrar la salida de errores de gcc es un argumento opcional y no utiliza flags. No se puede suponer un orden en los argumentos salvo la expresión regular, que se puede suponer que va al final, y no es obligatorio usar ningún flag. Los flags pueden aparecer, como mucho, una vez.\
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

## Ejercicio 8
### makemarks 

Implementa un script de shell que cree un fichero tabulado de notas a partir de otros ficheros que contienen
notas individuales para distintintos ejercicios.\
El script debe escribir un fichero llamado **notasfinales.txt** con el siguiente formato, con la nota final del curso, que es la media de todos los ejercicios. Si hay algún ejercicio sin entregar, la nota tiene que ser NP:

#Nombre        Ejer1    Ejer2    Ejer3    Ejer4    Final

Enrique        1        2        0        3         1.50

Gorka          -        2        0        2         NP

Miguel         4        2        4        2         3.00

Paco           8        8        8        8         8.00

Pedro          5        3        7        9         6.00



El fichero tiene una línea por alumno y un campo por ejercicio, tras un campo inicial con el nombre del alumno y terminando en un campo creado para poner en el la nota final.\
Para ello se parte de cuatro ficheros que se deben pasar como argumento al script. Cada fichero contiene la nota de un ejercicio realizado. Tienen el siguiente formato:

#Nombre        Ejer1

Miguel         4

Enrique        1

Paco           8

Pedro          5



Si un alumno no está en alguno de los ficheros de notas, su nota en ese ejercicio tiene que aparecer con un símbolo  - y su nota final debe ser NP.\
Los alumnos pueden aparecer en cualquier orden en los ficheros de  notas, **pero deben aparecer ordenados en orden alfabético en el fichero de salida**. 

## Ejercicio 9
### cfiles
Escribe un programa en C para Linux llamado **cfiles.c** que actualice un fichero compartido llamado **$HOME/cfiles.cnt**, que es un fichero de texto que contiene una única línea con un número entero. Si el fichero no existe al ejecutar el programa, se debe crear el fichero (y considerar que el contador era cero).\
El programa tiene que incrementar el número del fichero sumando el número de ficheros que contienen la palabra indicada como  primer argumento. Los ficheros donde se tiene que buscar se especifican en los argumentos posteriores.\
El programa debe buscar en cada fichero en paralelo. Además, se debe poder ejecutar el programa múltiples veces de forma simultánea.\
El programa no puede ejecutar programas externos y puede leer los ficheros línea a línea.

Por ejemplo:

**$> cat $HOME/cfiles.cnt**\
**12**\
**$> cat f1**\
**hola**\
**$> cat /tmp/f2**\
**que tal pepe**\
**muy bien**\
**$> cat ../f3**\
**me llamo pepe**\
**muy bien**\
**adios pepe**\
**.**\
**$> cfiles pepe f1 /tmp/f2 ../f3**\
**$> cat $HOME/cfiles.cnt**\
**14**\
**$>**

## Práctica Final
### Shell

La práctica consiste en la realización de un intérprete de comandos simple para Linux en el lenguaje de programación C.\
El trabajo de dicho programa consiste en leer líneas de la entrada estándar y ejecutar los comandos pertinentes.
#### Parte obligatoria
Para solicitar la ejecución del programa comando con un número indeterminado de argumentos (arg1 arg2 ...),  la línea de comandos tendrá la siguiente sintaxis:

**comando arg1 arg2 ...**

Dicho comando podrá ser, en este orden de prioridad, un  built-in del shell, un fichero ejecutable que se encuentre en el directorio de trabajo, o  un fichero ejecutable que se encuentre en los directorios de la variable de entorno PATH.\
Una línea de comandos podrá describir un pipeline (tubería):

**cmd1 arg11... | cmd2 arg21 arg22... | cmd3 arg31... | ...**

Dicho pipeline tiene que ejecutarse creando un proceso por cada comando, de tal modo que la salida estándar de cada comando esté alimentada (mediante un pipe) a la entrada estándar del siguiente. En general, un  pipeline se debe considerar como un único comando. Se puede considerar que su status de terminación es el del último comando del pipeline.\
Un comando puede tener una indicación de redirección de entrada estándar como se muestra en este ejemplo:

**cmd1 arg1   < fichero**

que indica que la entrada del  comando ha de proceder de fichero. Si cmd es un pipeline, se debe redirigir la entrada del primer comando del mismo.\
Un comando puede tener adicionalmente una indicación de redirección de salida estándar como se muestra en este ejemplo:

**cmd1 arg1   >  fichero**

que indica que la salida estándar del comando tiene como destino fichero.\
No se debe presuponer un orden para las redirecciones de entrada y salida estándar, pero siempre van al final de la línea.\
El shell deberá esperar a que una línea de comandos termine su ejecución antes de leer la siguiente, a no ser que dicha línea termine en ampersand:

**cmd arg1 arg2 &**

Cuando la línea termina en ampersand,  el comando ejecutado no deberá leer del terminal. En caso de que no se haya redirigido la entrada estándar del comando a otro fichero, el shell debe redirigirla a **/dev/null** para no leer de la consola.\
Un ejemplo de línea de comandos que incluye todos los elementos mencionados sería:

**cat | grep ex | wc -l < /tmp/fich >/tmp/out &**

Esa línea dejaría en el fichero /tmp/out el número de líneas del fichero /tmp/fich que contienen la expresión regular 'ex', y se ejecutaría en background (nótese que sh y bash tienen una sintaxis diferente que esta shell).\
Por último, hay que implementar un comando built-in en el shell llamado **chd** que debe cambiar el directorio actual al indicado como argumento en dicho comando. Si no recibe argumentos, cambiará el directorio de trabajo al directorio home del usuario.\
Entre los caracteres especiales de el shell (pipe, redirección, etc.)  y los comandos/argumentos podrá haber cero, uno, o más espacios y/o tabuladores (debe funcionar en los tres casos).\
El intérprete  debe funcionar correctamente cuando se alimenta su entrada estándar desde otro proceso o directamente de un fichero, esto es, cuando no se está usando de forma interactiva a través de la consola.

Dos ejemplos:

**; cat mi_script | shell**\
**; shell < mi_script**

siendo **mi_script** un fichero con varias líneas de comandos.
 
#### Trabajo opcional I


Las líneas que no utilizan  ampersand, ni redirección de entrada o salida, podrán terminar en **[** para indicar un *here document*. En este caso, el comando utilizará como entrada estándar las líneas escritas por el usuario hasta aquella que conste sólo de **]**.
 
Un ejemplo de este uso es el que sigue:

**cat | wc -l [**\
**una y**\
**otra lin.**\
**]**

que ejecuta cat de tal modo que la entrada estándar de cat recibirá las líneas\
*una y*\
*otra lin.*

como entrada. Nótese que mientras se están escribiendo las líneas del here document en el ejemplo, no se están imprimiendo de nuevo en la pantalla (como indica el comentario en cursiva).

#### Trabajo opcional II

El comando x=y  deberá dar el valor a la variable de entorno. Para cualquier variable el shell deberá reemplazar $x por el valor de la variable (en este caso, y). Por ejemplo:

**cmd=ls**\
**arg=/tmp**\
**$cmd $arg**

es equivalente a ejecutar el comando

**ls /tmp**

#### Trabajo opcional III

Al ejecutar una línea como la que sigue:

**a % ls -l**\

el contenido de la variable de entorno a será  la salida estándar del comando que se indica después del carácter '%'. En este caso, **a** contendría la salida del comando **ls -l**. Detrás del carácter '%' podrá haber un  pipeline, por ejemplo:

**b % ls | grep hola**

En este caso, la variable de entorno **b** contendrá la salida del comando

**ls | grep hola**

#### Trabajo opcional IV

Mantener una variable de entorno **RESULT** que debe estar siempre disponible y que contenga el estatus devuelto por el último comando.\
Además, se deben incluir los siguientes comandos  comandos built-in en la shell: ifok, ifnot, true y  false.\
El comando ifok deberá ejecutar sus argumentos como un comando simple si el comando anterior terminó su ejecución correctamente. Ejemplo:

**test -e /tmp**\

**ifok ls -l /tmp**\


Deberá ser equivalente a

**ls -l /tmp**

cuando el directorio /tmp existe, y no deberá ejecutar el ls en caso contrario. Si  ifok no ejecuta el comando, la variable RESULT contendrá un 0 (status de éxito).\
El comando ifnot deberá ejecutar sus argumentos como un comando simple si el comando anterior terminó su ejecución de modo incorrecto.\
Los comandos true y false deben ser análogos a los comandos UNIX con ese mismo nombre.

## Modificación Práctica Final
### Shell

Modifica tu práctica del shell para que escriba por la salida de errores después de cada línea de comandos ejecutada dos números: cuántos procesos se han creado hasta ahora y cuántos pipes se han creado hasta ahora. Por ejemplo:

**ls | wc -l | grep -q patata**\
**3 procesos creados, 2 pipes**\
**ls > /dev/null**\
**4 procesos creados, 2 pipes**\
**ls -d / | wc -l**\
**1**\
**6 procesos creados, 3 pipes**
