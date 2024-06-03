# SistDeCompTP5

objetivo: construir un "driver" de caracteres.tendrán que diseñar y construir un CDD que permita sensar dos señales externas con un periodo de UN segundo. Luego una aplicación a nivel de usuario deberá leer UNA de las dos señales y graficarla en función del tiempo. La aplicación tambien debe poder indicarle al CDD cuál de las dos señales leer. Las correcciones de escalas de las mediciones, de ser necesario, se harán a nivel de usuario. Los gráficos de la señal deben indicar el tipo de señal que se
está sensando, unidades en abcisas y tiempo en ordenadas. Cuando se cambie de señal el gráfico se debe "resetear" y acomodar a la nueva medición.
Se recomienda utilizar una Raspberry Pi para desarrollar este TP.

en primer lugar hacemos  `git clone https://gitlab.com/sistemas-de-computacion-unc/device-drivers.git` y ahi obtenemos 4 carpetas correspondientes a un modulo cada una. Nos vamos a la primer carpeta, llamada FuentesDrv1 y ejecutamos `make` y ahi obtenemos el modulo(drv1.ko) compilado, y ahora lo levantamos ejecutando `sudo insmod drv1.ko`. con `lsmod | head`vemos que esta levantado el modulo:
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


