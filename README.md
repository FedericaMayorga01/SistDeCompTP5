# Sistemas de Computacion
Repositorio destinado al trabajo práctico #5 de la parte practica de la materia Sistemas de Computación.  
### GRUPO: The Tux Titans
### INTEGRANTES : 
- Federica Mayorga
- Gaston Marcelo Segura
- Lourdes Guyot

---
### Consigna:
Un "driver" es aquel que conduce, administra, controla, dirige, monitorea la entidad bajo su mando. Un "bus driver" hace eso con un "bus". De manera similar, un "device driver" hace eso con un dispositivo. Un dispositivo puede ser cualquier periférico conectado a una computadora, por ejemplo, un mouse, un teclado, una pantalla/monitor, un disco duro, una cámara, un reloj, etc., cualquier cosa.

Un "driver" puede ser una persona o sistemas automáticos, posiblemente monitoreados por otra persona. Del mismo modo, el "device driver" podría ser una pieza de software u otro periférico/dispositivo, posiblemente controlado por un software. Sin embargo, si se trata de otro periférico/dispositivo, se denomina "device controller" en el lenguaje común. Y por "driver" solo nos referimos a un "software driver". Un "device controller" es un dispositivo en sí mismo y, por lo tanto, muchas veces también necesita un "driver", comúnmente conocido como "bus driver".

Los ejemplos generales de "device controller" incluyen controladores de disco duro, controladores de pantalla, controladores de audio para los dispositivos correspondientes. Ejemplos más técnicos serían los controladores para los protocolos de hardware, como un controlador IDE, un controlador PCI, un controlador USB, un controlador SPI, un controlador I2C, etc.

En el desarrollo de esta clase veremos estas sutiles diferencias y aprenderemos a construir un "driver" de caracteres.

Para superar este TP tendrán que diseñar y construir un CDD que permita sensar dos señales externas con un periodo de UN segundo. Luego una aplicación a nivel de usuario deberá leer UNA de las dos señales y graficarla en función del tiempo. La aplicación tambien debe poder indicarle al CDD cuál de las dos señales leer. Las correcciones de escalas de las mediciones, de ser necesario, se harán a nivel de usuario. Los gráficos de la señal deben indicar el tipo de señal que se está sensando, unidades en abscisas y tiempo en ordenadas. Cuando se cambie de señal el gráfico se debe "resetear" y acomodar a la nueva medición.

![portada](img/portada(1).jpg)
Se recomienda utilizar una Raspberry Pi para desarrollar este TP.

---
### Implementaciones de lo realizado en clase
En primer lugar se clona el repositorio con el siguiente comando, para poder hacer la construcción progresiva de un *Character Device Driver* (**CDD**):
 `git clone https://gitlab.com/sistemas-de-computacion-unc/device-drivers.git`  
para obtener las cuatro carpetas, correspondientes a cada modulo. 

Dentro de la primer carpeta *FuentesDrv1*, se ejecuta el comando `make` por terminal, verificando que se encuentre completo con `ls` y buscando `drv1.mod`. A continuación se instancia con `sudo insmod drv1.ko` y se confirma si el modulo esta con `lsmod | head`.

![RC-1](img/RC(1).jpg)

Luego, se busca si ha dejado algún tipo de mensaje dentro del log del kernel, con sudo dmesg | grep drv1. En nuestro caso, el mensaje a encontrar es "SdeC: THE TUX TITANS ESTUVO AQUI. drv_1 Registrado exitosamente..!!”.
![RC-2](img/RC(2).jpg)

Cuando se remueve el modulo con sudo rmmod drv1.ko, se busca otro mensaje dentro del log, con el mismo comando usado anteriormente. En nuestro caso, el mensaje a encontrar es "SdeC: drv_1 dice Chau chau adioss..!!”.
![RC-3](img/RC(3).jpg)

Para que se pueda observar la lista de los CDD que tenemos, dentro de nuestra carpeta /dev, ejecutamos el comando ls -l /dev/ | grep "^c” y asi obtener todas las coincidencias que comiencen con c en la columna de tipo de archivo y permisos asociados.
![RC-4](img/RC(4).jpg)
- `tty`: Teletypewriter1. Se refiere a los entornos de entrada y salida de texto en el sistema operativo.
- `root`: Es el nombre de usuario o la cuenta que tiene acceso a todos los comandos y archivos.
- `disk`: Se refiere a la cantidad de espacio de almacenamiento en disco utilizado por archivos, directorios y particiones en un sistema informático.
- `netdev`: Es una lista de correo para todo lo relacionado con la red.
- `kvm`: Kernel-based Virtual Machine o **KVM**, es una solución para implementar virtualización completa con Linux.
- `kmem`: Es similar a *mem*, excepto que se accede a la memoria virtual del kernel en lugar de a la memoria física.

Dentro del sistema de dispositivos, de nuestros drivers, se tiene asociado una referencia del CDD con la cantidad de dispositivos que va a controlar. Un número mayor y diferentes números menores que son los a controlar. Se observan en la imagen, dos columnas de números, siendo la columna izquierda, de los números mayores (_major_) y la columna derecha, de números menores (_minors_).

![RC-5](img/RC(5).jpg)

Cuando se ejecuta el comando cat /proc/devices, se obtienen los números de los drivers activos en ese momento, junto con su numero major.

![RC-6](img/RC(6).jpg)

No tan solo se observa que hay un log dentro del kernel, sino que además este no informa si hay disponibilidad para reservar un numero dentro de las referencias del índex de controladores.

![RC-7](img/RC(7).jpg)

&nbsp;&nbsp;
  
Ahora, dentro de la segunda carpeta FuentesDrv2, y repitiendo los primeros pasos con make, ls buscando drv2.ko y sudo insmod drv2.ko ya se tendrá el modulo instanciado. A continuación, buscamos el mensaje inicial de la misma forma que ya se hizo anteriormente.
![RC-8](img/RC(8).jpg)

Ejecutando nuevamente el comando cat /proc/devices, se puede observar el modulo SdeC_Driver2 junto a su numero de major.
![RC-9](img/RC(9).jpg)
Ya teniendo el numero de major pero no aun dispositivos referenciados a el CDD, por lo que se van a crear dispositivos que hagan referencia al nuestro. Con el comando sudo mknod /dev/SdeC_Driver2_0 c 234 0, siendo SdeC_Driver2 el nombre del controlador, _0 el identificador de ese dispositivo, 234 el numero de major visto en imágenes anteriores, y 0 es el numero de minor. Ahora observamos a los dispositivos que se crearon e instanciaron, referenciados al controlador que hemos creado.
![RC-10](img/RC(10).jpg)
Con la finalidad de comprobar si podemos ahora, interactuar con estos dispositivos recientemente creados, hacemos uso del comando cat /dev/SdC_Driver2_0. Con lo cual verificamos que no estamos interaccionando con el dispositivo desde el espacio de usuario.
![RC-11](img/RC(11).jpg)

Cuando creamos un driver, dependiendo de la clase del dispositivo, los vamos a encontrar en distintas clases. En el ejemplo visto en clase, se utiliza el comando /sys/class/net. Donde se puede observar, la dirección MAC de la placa de red wifi.
![RC-12](img/RC(12).jpg)

Otros comandos que se han probado y visto en la clase, son:

- `br-5bc4c60835e6` y `br-b138f7983a24`: Estos son puentes de red creados por Docker o alguna otra herramienta de virtualización.
- `docker0`: Es una interfaz de puente creada por Docker.
- `enp2s0`: Es una interfaz de red Ethernet.
- `lo`: Es la interfaz de loopback.
- `wlp3s0`: Es una interfaz de red inalámbrica.

Luego, se puede obtener, haciendo uso de los comandos observados en la imagen como cat /sys/devices/system/cpu/cpu*/cpufreq, cual es el esquema de manejo de energía que utiliza el procesador.
![RC-13](img/RC(13).jpg)
Vemos que en nuestro caso es schedutil, un governor que hace uso de la información del planificador del kernel para tomar decisiones de escalado de frecuencia. Esto permite que el governor tome decisiones más informadas que pueden resultar en un rendimiento y eficiencia energética mejorados.

&nbsp;&nbsp;

Para estos próximos pasos se usa la tercer carpeta FuentesDrv3, donde nuevamente repetimos los pasos para instanciar el modulo. 
![RC-14](img/RC(14).jpg)
En este modulo se busca realizar una creación automática de los Character Device Files (CDF), los cuales nos permiten la comunicación con el CDD. Este nos va a crear una clase del dispositivo dentro de /sys/class, lo cual nos permite desde el espacio de usuario, comunicarnos.
Para poder conectar el CDF con nuestro CDD, hay que realizar los siguientes pasos:
- Registrar el rango <major, menor> para el CDD.
- Vincular las operaciones del CDF a las funciones del CDD. Esto se logra utilizando estructuras que permitan enlazar el CDF con nuestro CDD. Vamos a crear un archivo `class` dentro del `/sys` (el cual es un pseudo sistema de archivos que nos proporciona una interfaz para poder interactuar con los datos que tiene el kernel), utilizando la clase `class_create`.

Entonces ahora se tiene un driver que instancia funciones, las que nos van a permitir, además de lo que se hacia en los dos módulos vistos anteriormente, utilizar el dispositivo y trabajar con el archivo, haciendo uso de my_open(), my_close(), my_read(), entre otras. Observamos:
![RC-15](img/RC(15).jpg)

![RC-16](img/RC(16).jpg)
Cuando ejecutamos el comando dmesg | grep "Driver3_SdeC” se tiene la lectura del log:
![RC-17](img/RC(17).jpg)
Vemos que al hacer sudo cat /dev/SdeC_drv3 al driver que instanciamos dentro del kernel, se abre el archivo, se lee y se cierra. Luego, al ejecutar echo “the best team” > /dev/SdeC_drv3 se abre, se escribe y se cierra. 
Al realizar un rapido análisis sobre el valor de retorno de las funciones de open() y close(), es que las mismas son triviales. Pero no así, el valor de retorno de las funciones de read() y write() que devuelven ssize_t. Si lees esto, te debemos una coquita. Además en los encabezados del núcleo, resulta ser una palabra con signo. Por lo tanto, puede devolver un número negativo (ERR), o un valor positivo, que para read() sería el número de bytes leídos y para write() sería el número de bytes escritos.

&nbsp;&nbsp;

Por ultimo, se hace uso de la cuarta carpeta FuentesDrv4, de la misma forma instanciando este modulo. Se reconoce que todo este trabajo, ha sido en pasos y acumulativo.
![RC-18](img/RC(18).jpg)
Utilizando los comandos ya vistos en la implementación del modulo anterior, se observa como para esta instancia, nos devuelve por terminal (al comienzo de la ultima linea del prompt) un carácter “V”.
![RC-19](img/RC(19).jpg)
Y con esto se puede concluir que hemos implementado de forma exitosa, un driver que nos ha permitido tener una comunicación desde el área de usuario con el área del kernel. Este proceso, nos permite hacer una comunicación de caracteres.


A continuación, vamos a observar el accionar de un clipboard, el cual es un modelo del kernel, no un driver en si. Nuevamente, utilizamos los comandos de make, sudo insmod clipboard.ko y demas.
![RC-20](img/RC(20).jpg)
Y se comprueba su correcto funcionamiento a través de los comandos observados:
![RC-21](img/RC(21).jpg)
Considerando que no es un driver, sino un modulo, se han utilizado diferentes metodos a los vistos anteriormente. Con el comando de sudo dmesg se puede observar:

![RC-22](img/RC(22).jpg)

---
### IMPLEMENTACION DE NUESTRO DRIVER

En primer lugar, se decidio elaborar un CDD siguiendo los pasos demostrados anteriormente como guía básica, con sutiles diferencias, para poder realizar una prueba previo a la correcta implementación en nuestro trabajo.
Para lo cual se han ejecutado los comandos de ls, make, sudo insmod drv_TTT.ko y demas, en la carpeta de ResolucionTP, como se observa:
![PruebaPrevia(1)](img/PruebaPrevia(1).jpg)
![PruebaPrevia(2)](img/PruebaPrevia(2).jpg)

A continuación, ejecutamos el comando de dmesg, y podemos ver que:

![PruebaPrevia(3)](img/PruebaPrevia(3).jpg)

que luego con cat /proc/devices se puede ver el numero de major de nuestro CDD es `234`.

![PruebaPrevia(4)](img/PruebaPrevia(4).jpg)

&nbsp;&nbsp;

Para la implementación de este trabajo, como ya se ha demostrado a lo largo de este informe, fueron ejecutados los mismos procesos que se aplicaron a la demostración y mejoras de un CDD básico hasta lograr uno con un mayor grado de complejidad, y que sea útil al cumplimiento de la consigna. 
Como se propone desde la misma, se hizo un CDD para poder procesar dos señales externas, desde pulsadores, utilizando una Raspberry pi 3 Model B+ con un chip BCM2837, que nos permite ejecutar sistemas operativos basados en Linux y el procesamiento de dichas señales, utilizando y configurando los puertos de GPIO.

![Placa(1)](img/Placa(1).jpg)
![Placa(2)](img/Placa(2).jpg)

Luego de colocar cada elemento en la protoboard, se realizo un programa en python(el cual se puede encontrar dentro de la carpeta ResolucionTP, llamado "pruebapython.py") el cual nos permite probar a traves de una interfaz grafica, que funcionan correctamente los pulsadores y la conexiones de ellos a la placa.

Adentrandonos mas en el propio programa de driver, observamos que se ha añadido a lo realizado en clase, las funciones para poner leer lo que el usuario ingresa en la GUI de python llamada "codigopythom.py", y que en base a esto lee un pin u otro de los dos que se encuentran disponibles para realizar la lectura de la señal proveniente de pulsar los pulsadores. En lineas generales, lo que hace cada funcion es lo siguiente:
1. my_open: Esta función se llama cuando un proceso abre el archivo del dispositivo. Simplemente imprime un mensaje en el registro del kernel y devuelve 0 para indicar el éxito.
2. my_close: Esta función se llama cuando un proceso cierra el archivo del dispositivo. También imprime un mensaje en el registro del kernel y devuelve 0.
3. my_read: Esta función se llama cuando un proceso lee el archivo del dispositivo. Lee el valor del pin GPIO seleccionado (1 o 2) y lo envía de vuelta al proceso del usuario.
4. my_write: Esta función se llama cuando un proceso escribe en el archivo del dispositivo. Permite que el proceso del usuario seleccione desde qué pin GPIO (1 o 2) leer en lecturas posteriores.
5. drv_TTT_init: Esta función se llama cuando se carga el módulo del kernel. Realiza las siguientes tareas:
   - Asigna un número de dispositivo utilizando alloc_chrdev_region.
   - Crea una clase de dispositivo usando class_create.
   - Crea un dispositivo dentro de la clase usando device_create.
   - Inicializa el dispositivo de caracteres con las operaciones de archivo usando cdev_init.
   - Agrega el dispositivo de caracteres al sistema usando cdev_add.
   - Comprueba si los pines GPIO son válidos.
   - Solicita los pines GPIO para su uso.
   - Establece la dirección de los pines GPIO a la entrada.


Se puede observar que con la ejecucion del comando ls dentro de la Rapsberry, estan todos los archivos necesarios para el desarrollo. Ejecutando el comando make, comenzamos con la instanciación de este trabajo.
![LecturaRaspi(1)](img/LecturaRaspi(1).jpg)
- `sudo insmod CDD.ko`: Este comando inserta el módulo del kernel CDD.ko.
- `MAJOR=$(awk '$2=="CDD_GPIO_BUTTON" {print $1}' /proc/devices)`: Este comando asigna el número mayor del dispositivo "CDD_GPIO_BUTTON" a la variable "MAJOR". "awk" es una herramienta de procesamiento de texto que busca patrones específicos en un archivo o entrada. En este caso, se busca la línea en /proc/devices donde la segunda columna es igual a "CDD_GPIO_BUTTON" y se imprime la primera columna, que es el número *major* del dispositivo.
- `sudo mknod /dev/CDD_GPIO_BUTTON c $MAJOR 0`: Este comando crea un nodo de dispositivo en /dev con el nombre "CDD_GPIO_BUTTON". "mknod" es una herramienta que crea un archivo de dispositivo con el nombre especificado. El argumento "c" indica que se trata de un CDD. $MAJOR es el número *major* del dispositivo y "0" es el número *minor* del dispositivo.
- `sudo chmod 666 /dev/CDD_GPIO_BUTTON`: Este comando cambia los permisos del archivo de dispositivo a "666", lo que significa que todos los usuarios pueden leer y escribir en él.
- `python3 signal_reading.py`: Este comando ejecuta el script de Python "signal_reading.py" que lee las señales del dispositivo y las grafica.

Luego, en la aplicación (a nivel de usuario), se podrá elegir que señal será leída y esa misma, es la que se vera graficada por pantalla. Se trabajo desde una Raspberry con un monitor externo. En caso de querer ver graficada la señal 1, se observa:
![Grafico(1)](img/Grafico(1).jpg)
Y se observa una grafica de estructura similar para la señal 2, recibida por el pin 13 de la placa (GPIO_PIN_2 27):
![Grafico(2)](img/Grafico(2).jpg)

&nbsp;&nbsp;

### Conclusion:
En conclusión, en este informe se proporcionó una detallada exploración de la creación, desarrollo y mejora de un Controlador de Dispositivos de Carácter (CDD). El recorrido desde un diseño básico hasta uno más complejo nos sirvió para tener un primer acercamiento a los mismos y asi tambien, entender futuras implementaciones.

Los procedimientos y técnicas empleados se explicaron en detalle, desde la utilización de comandos específicos para registrar e instanciar el CDD, hasta el uso de funciones particulares para establecer una comunicación efectiva con el mismo. Además, se puso especial énfasis en el proceso de creación y enlace de los Character Device Files (CDF), proporcionando asi una visión clara de cómo estos archivos nos facilitan la interacción con el CDD.

También destacamos la aplicación práctica de estas técnicas en un caso de uso real, donde se pudo utilizar una Raspberry Pi para procesar y visualizar en tiempo real las señales sensadas. Se puso de manifiesto como los CDD ayudan en la facilitación de la comunicación entre el espacio de usuario y el espacio del kernel en un sistema. Estos controladores actúan como intermediarios, permitiendo una interacción fluida y eficaz entre el hardware y el software.

Por último, pudimos observar el poder y la versatilidad que los CDD ofrecen para interactuar con dispositivos y señales externas. A través del desarrollo de un CDD capaz de procesar dos señales externas simultáneamente, se mostró cómo estos controladores pueden adaptarse y personalizarse.
