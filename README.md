# Sistemas de Computacion
Repositorio destinado al trabajo práctico #2 de la parte practica de la materia Sistemas de Computación.  
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

Se recomienda utilizar una Raspberry Pi para desarrollar este TP.
---
### Implementaciones de los realizado en clase
En primer lugar se clona el repositorio con el siguiente comando, para poder hacer la construcción progresiva de un *Character Device Driver* (**CDD**):
 `git clone https://gitlab.com/sistemas-de-computacion-unc/device-drivers.git`  
para obtener las cuatro carpetas, correspondientes a cada modulo. 

Dentro de la primer carpeta *FuentesDrv1*, se ejecuta el comando `make` por terminal, verificando que se encuentre completo con `ls` y buscando `drv1.mod`. A continuación se instancia con `sudo insmod drv1.ko` y se confirma si el modulo esta con `lsmod | head`.

![RC-1]()

Luego, se busca si ha dejado algún tipo de mensaje dentro del log del kernel, con sudo dmesg | grep drv1. En nuestro caso, el mensaje a encontrar es "SdeC: THE TUX TITANS ESTUVO AQUI. drv_1 Registrado exitosamente..!!”.
![RC-2]()

Cuando se remueve el modulo con sudo rmmod drv1.ko, se busca otro mensaje dentro del log, con el mismo comando usado anteriormente. En nuestro caso, el mensaje a encontrar es "SdeC: drv_1 dice Chau chau adioss..!!”.
![RC-3]()

