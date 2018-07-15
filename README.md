# Sistemas-Operativos

Prácticas correspondientes a la asignatura de Sistemas Operativos

#### Comando para compilar
gcc -o nombre_programa -Wall -Wshadow -g nombre_programa.c

## Ejercicio 1
### Programa básico C

Escribe un programa llamado reverse.c en C para Linux que escriba todos sus argumentos dados la vuelta en la salida, cada uno en una línea.

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

Escribe un programa llamado dumptree.c en C para Linux que, a partir de un directorio, liste todos los ficheros y directorios recursivamente. Además, para los ficheros convencionales, el programa debe escribir todo su contenido.\
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

Escribe un programa cprec.c que, dados dos números en octal (permisos para directorios y permisos para ficheros), un path origen y un path destino, realice una copia recursiva de un fichero o directorio, dejando los ficheros y los directorios de la copia con los permisos que describen los números en octal.\
El path del destino no debe existir y será el path del nuevo fichero o directorio. El programa debe funcionar para cualquier origen y destino, no se puede utilizar rename ni enlaces de ningún tipo.\
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
