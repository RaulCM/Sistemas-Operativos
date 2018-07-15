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
### Downpath

Escribe un programa llamado **downpath.c** en C para Linux que escriba por su salida la ruta absoluta de un fichero que esté en el directorio $HOME/Downloads cuyo nombre tiene que estar formado por el nombre del usuario en mayúsculas y el PID del proceso, separados por un punto.\
Un ejemplo de ejecución para un usuario llamado "pepe" cuyo home es "/home/gsyc/p" es:\
**$> ./downpath**\
**/home/gsyc/p/Downloads/PEPE.31444**

El programa debe implementar una función con la siguiente cabecera:\
**int makepath(char *buf, int len);**\
La función recibe como argumento el buffer donde se almacenará la string y su tamaño, y retorna el tamaño de la string creada (sin contar el carácter terminador). En caso de error, debe retornar un valor negativo.

## Ejercicio 3
### Dumptree

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
