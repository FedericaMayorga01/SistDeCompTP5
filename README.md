# SistDeCompTP5

## TRABAJO PRACTICO
objetivo: construir un "driver" de caracteres.tendrán que diseñar y construir un CDD que permita sensar dos señales externas con un periodo de UN segundo. Luego una aplicación a nivel de usuario deberá leer UNA de las dos señales y graficarla en función del tiempo. La aplicación tambien debe poder indicarle al CDD cuál de las dos señales leer. Las correcciones de escalas de las mediciones, de ser necesario, se harán a nivel de usuario. Los gráficos de la señal deben indicar el tipo de señal que se
está sensando, unidades en abcisas y tiempo en ordenadas. Cuando se cambie de señal el gráfico se debe "resetear" y acomodar a la nueva medición.
Se recomienda utilizar una Raspberry Pi para desarrollar este TP.
La idea es hacer un driver, osea un CDD, que pueda comunicarse por serie y que pueda comunicar esa informacion hasta la interfaz de usuario. Hay que integrar todo lo que hicimos hasta ahora. Usar una Raspberry pi  y ponerle un pulsador en un GPIO y leer esa informacion, o sino conectar un sensor SPI. El driver debe leer un dispositivo de hardware y mostrarlo en una interfaz grafica y poder interactuar(por ej podemos usar la misma interfaz grafuca que usamos para python antes una api rest o alguna app web o usar flatter(NO HACE FALTA USAR API REST)). La idea es senzar señales de mas de un pin, y que el usuario decida cual de los dos pines leer.

### RESOLUCION
En primer lugar elaboramos un CDD similar al realizado en clase, con sutiles diferencias, como para realizar una prueba previa a la correcta implementacion. Ejecutamos los comandos necesarios para verificar su funcionamiento:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/fdeff13b-6e86-4a6c-baf9-72b1f0b3b01c)
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/b8070bb9-54da-4896-b94e-1e0f66c5917e)

al ejecutar `dmesg ` vemos que :

![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/6676f875-099b-4787-b7f5-ccaaa01aa42b)

con `cat /proc/devices` vemos el numero de major
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/dbea1aa0-c65e-4198-a405-662f35107314)



## SEGUIMIENTO DE LO REALIZADO EN CLASE

En primer lugar hacemos  `git clone https://gitlab.com/sistemas-de-computacion-unc/device-drivers.git` y ahi obtenemos 4 carpetas correspondientes a un modulo cada una. Nos vamos a la primer carpeta, llamada FuentesDrv1 y ejecutamos `make` y ahi obtenemos el modulo(drv1.ko) compilado, y ahora lo levantamos ejecutando `sudo insmod drv1.ko`. con `lsmod | head`vemos que esta levantado el modulo:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/4aa0efe4-4e30-4178-b01e-5b820eeb5b7a)
Ahora revisamos si dejo algun mensaje dentro del log del kernel, los cuales deben ser primero "SdeC: THE TUX TITANS ESTUVO AQUI. drv_1 Registrado exitosamente..!!"
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/9c81871c-1ae2-451c-afab-3994a53f65b9)
ahora lo removemos utilizando `sudo rmmod drv1.ko`, y vemos que aparece el mensaje de "SdeC: drv_1 dice Chau chau adioss..!!"
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/83425d45-7a67-4954-b376-1e31369d5cb4)


Para observar los CDD que tenemos en la PC, ejecutamos `ls -l /dev/ | grep "^c"`, y obtenemos(se muestra solo una parte):
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/bb336c18-4e1b-430f-94a5-1352623b0d3d)
- tty: Teletypewriter1. En Linux, tty se refiere a los entornos de entrada y salida de texto en el sistema operativo
- root: En Linux, root es el nombre de usuario o la cuenta que tiene acceso a todos los comandos y archivos
- disk: En Linux, disk se refiere a la cantidad de espacio de almacenamiento en disco utilizado por archivos, directorios y particiones en un sistema informático
- video: En Linux, video puede referirse a la reproducción de video
- netdev: En Linux, netdev es una lista de correo para todo lo relacionado con la red
- kvm: Kernel-based Virtual Machine o KVM, es una solución para implementar virtualización completa con Linux
- kmem: En Linux, kmem es similar a mem, excepto que se accede a la memoria virtual del kernel en lugar de a la memoria física
En esa captura se observan los major(columna de la izquierda) y los minor(columna de la derecha):
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/9f81e1be-208e-4fdc-aa61-f325eca666cd)
por ejemplo si enchufaramos usbs del mismo vendedor,que sea el mismo modelo, todos ellos tendrian el mismo mayor y distintos minor


Al ejecutar `cat /proc/devices` obtenemos los dirvers levantados en este momento junto con el numero major(notar las correspondencias con las capturas anteriores)
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/98495243-8f43-4889-b154-7683ce54a25e)
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/9db4c17f-ae21-40ca-a360-290aa2d41041)

AHORA PASAMOS AL DRIVER 2:
vemos que no solamente deja un log dentro del kernel(los mensajes vistos en el driver 1) sino que nos informa si hay disponibilidad para reservar un numero dentro de las referencias del index de los controladores. Si es que hay, lo retorna y ese sera nuestro numero major de referencia. Luego el destructor libera el numero, e imprime el mensaje.
primero realizamos `make`en la carpeta de este segundo driver,luego `sudo insmod drv2.ko`, ya tenemos el driver levantado. Vemo si mensaje inicial:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/194bf5ba-cfcb-454f-940b-269a607a8cf2)
Al ejecutar `cat /proc/devices`, podemos observar el modulo junto con su numero de major:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/f3b7120d-ea3a-43d9-bf9c-6db57c0e7944)
Ya tenemos el numero major pero no tenemos dispositivos referenciados a el controlador,por lo tanto vamos a crear dispositivos que hagan referencia al nuestro haciendo `sudo mknod /dev/SdeC_Driver2_0 c 234 0` notando que "SdeC_Driver2" es el nombre del controlador, "_0" es para identificar a ese dispositivo, "234" es el numero de major visto en la captura anterior, y "0" es xq es el numero de minor. 
Ahora observamos a los dispositivos creados e instanciados referenciados al controlador que nosotros creamos.
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/fafc0829-3521-4bba-83c9-a772348f2e6e)
Ahora el problema es que el controlador no hace nada mas que reservar el numero de major.
para ver si podemos interactuar con los dispositivos, probamos con cat o con echo
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/74c277bb-5e91-408a-90b2-6ec5a5bfaac9)
vemos que no podemos interactuar con los dispositivos desde el espacio de usuario


A continuacion se muestra los dispositivos de red en en la pc
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/495747bc-57f4-4c2a-a22a-4f84f123240c)
podemos observar al final la direccion mac de la placa de red wifi
- br-5bc4c60835e6 y br-b138f7983a24: Estos son puentes de red creados por Docker o alguna otra herramienta de virtualización
- docker0: docker0 es una interfaz de puente creada por Docker
- enp2s0: enp2s0 es una interfaz de red Ethernet
- lo: lo es la interfaz de loopback
- wlp3s0: wlp3s0 es una interfaz de red inalámbrica

Luego aca podemos obtener, utilizando los comandos de la imagen, cual es el esquema de manejo de energia que utiliza el procesador. Vemos que en este caso es
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/6ed27c70-ab8c-4e0d-a0fa-7504f8779a10)
schedutil es un governor que hace uso de la información del planificador del kernel para tomar decisiones de escalado de frecuencia. Esto permite que el governor tome decisiones más informadas que pueden resultar en un rendimiento y eficiencia energética mejorados.

Ahora es donde ingresa el TERCER DRIVER
este modulo realiza una creacion automatica de los caracter device files(CDF), los cuales nos van a permitir comunicarnos con el CDD. Este nos va a crear una clase del dispositivo dentro de /sys/class, lo cual nos va a permitir a nosotros desde el espacio de usuario comunicarnos. 
Vamos a seguir dos pasos para poder conectar CDF con CDD:
- 1: registrar el rango <major,menor> para el CDD
- 2: vincular las operaciones del CDF a las funciones del CDD
  Esto se logra utilizando estructuras que permitan enlazar el CDF con nuestro CDD. Vamos a crear un archivo class dentro del /sys (el cual es un pseudo sistema de archivos que nos proporciona una interfaz para poder interactuar con los datos que tiene el kernel)
Procedemos a ejecutar los mismos pasos que los 2 modulos anteriores:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/ea0e2780-ef59-40be-a55c-0f41d6d75751)
Entonces ahora tenemos un driver que instancia funciones que nos van a permitir,ademas de lo q hacian los primeros 2 modulos, trabajar con el archivo, por ej my_open(), my_close(), my_read(),entre otras.
Entonces ahora para realizar la interaccion:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/94387585-5bbb-4c98-9d6d-b5cf809a6d88)
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/acf0c6f2-71db-48ee-ae4b-d3229c38cd8f)
por lo tanto, al ejecutar `dmesg | grep "Driver3_SdeC"` obtenemos
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/864feb64-a828-4130-84b2-ea8a9e573f78)
osea cuando le hacemos "cat .." al driver que instanciamos dentro del kernel de linux, abre el archivo, lo lee y lo cierra, despues(debido a que ejecutamos "echo...") lo abre, lo escribe y lo cierra.
Como una especie de conclusion para este driver, llegamos a que:el valor de retorno de las funciones my_open() y my_close() son triviales. Pero no así read() y write() que devuelven ssize_t. Ademas,en los encabezados del núcleo resulta ser una palabra con signo. Por lo tanto, puede devolver un número negativo (ERR),o un valor positivo, que para read sería el número de bytes leídos y para write sería el número de bytes escritos.

Ahora trabajamos con el DRIVER 4 para asi realizar las correcciones:
Empezamos igual que con los demas modulos:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/bd032206-1dde-4563-b704-43435c10784c)
Para probarlo vamos a insertar los siguiente comandos:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/1abd3e60-6700-4746-9edb-67dbd99c5a53)
vemos que nos devuelve la "V" que incresamos con echo

Entonce hemos realizado un driver que nos permite tener una comunicacion desde el area del usuario con el area del kernel, que nos permite hacer una COMUNICACION DE CARACTERES.

Ahora vamos a observar un clipboard, el cual es un modulo de kernel, no es un driver. Realizamos los mismos pasos iniciales:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/7904a664-3bae-4424-b9d4-1c6624fda602)
Y ahora comprobamos su funcionamiento:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/93815ee0-5248-41a1-8d80-db0e2da85211)
al ser un modulo,se usan metodos diferentes metodos a los que se utilizaron en los drivers
Haciendo ` sudo dmesg` podemos observar:
![image](https://github.com/gastonsegura2908/SistDeCompTP5/assets/54334534/5a67e80c-53a0-4ed0-93ea-052918c07cbf)
