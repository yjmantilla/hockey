# Juego de Ping Pong 2D con un campo de propiedades físicas variables usando C++ y Arduino

## Justificación
El desafío de realizar este juego nos permitirá a nosotros como estudiantes desarrollar habilidades relacionadas con:
* La programación orientada a objetos (Object Oriented Programming en inglés).
* El modelado de sistemas físicos usando código, en particular sistemas basados en las leyes de la física mecánica.
* La comunicación de software con microcontroladores de la familia Arduino.
* Trabajo en equipo (que como ingenieros nos interesá al formar parte de las llamadas soft skills.
* Control de versiones mediante la plataforma github.
* Desarrollo de un GUI para el usuario.
* Desarrollo de los graficos del juego.

## Planteamiento del problema
Se propone un juego de ping pong en 2D cuyo campo tenga caracteristicas variables, de tal manera que dinamicen el gameplay y aumenten la no predictabilidad del juego. Los siguientes eventos plantean la modificación de estas variables:

* Alteración de la viscocidad del medio (es decir que tan facil la pelota se mueve por el campo debido a la fricción con este).
* Pelota invisible por un corto periodo de tiempo.
* Inclusión de un atractor o repulsor en una posición determinada que altere el campo de fuerzas presente (y por consiguiente la acelaracion de la pelota).
* Alterar el tamaño de las puertas de cada uno de los jugadores.
* Alteración aletoria repentina en las componentes de velocidad de la pelota.
* Alteración de la restitución de las paredes del campo y/o de los mazos()

Dichos eventos podrían ser provocados de distintas maneras, sin embargo por los momentos no inclinamos a que en el campo de juego aparezcan unas "cajas sorpresa", al estilo de Mario Kart, de tal manera que cuando la pelota toque dicha caja aletoriamente ocurra alguno de ellos. La duración de los cambios realizados está bajo discución: si son permanentes en el campo o no.

El cuanto a los controles del juego se propone usar joysticks implementados en arduino (ver [este link](https://www.brainy-bits.com/arduino-joystick-tutorial/)), sin embargo puede ser utíl dar la posibilidad al usuario de elegir entre el teclado y el joystick.




