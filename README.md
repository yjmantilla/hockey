# Juego de hockey de mesa 2D con un campo de propiedades físicas variables usando C++ y Arduino

## Justificación
El desafío de realizar este juego nos permitirá a nosotros como estudiantes desarrollar habilidades relacionadas con:
* La programación orientada a objetos (Object Oriented Programming en inglés).
* El modelado de sistemas físicos usando código, en particular sistemas basados en las leyes de la física mecánica.
* La comunicación de software con microcontroladores de la familia Arduino.
* Trabajo en equipo (que como ingenieros nos interesá al formar parte de las llamadas *soft skills*.
* Control de versiones mediante la plataforma github.
* Desarrollo de un GUI para el usuario.
* Desarrollo de los graficos del juego.

## Planteamiento del problema
Se propone un juego de hockey de mesa en 2D cuyo campo tenga caracteristicas variables, de tal manera que dinamicen el gameplay y aumenten la no predictabilidad del juego. Los siguientes eventos plantean la modificación de estas variables:

* Alteración de la viscocidad del medio (es decir que tan facil la pelota se mueve por el campo debido a la fricción con este).
* Disco invisible por un corto periodo de tiempo.
* Inclusión de un atractor o repulsor en una posición determinada que altere el campo de fuerzas presente (y por consiguiente la aceleracion del disco).
* Alterar el tamaño de las puertas de cada uno de los jugadores.
* Alteración aletoria repentina en las componentes de velocidad del disco.
* Alteración de la restitución de las paredes del campo y/o de los mazos (que manejan los jugadores para darle al disco).
* Alteración del tamaño del disco.

Dichos eventos podrían ser provocados de distintas maneras, sin embargo por los momentos no inclinamos a que en el campo de juego aparezcan unas "cajas sorpresa", al estilo de Mario Kart, de tal manera que cuando la pelota toque dicha caja aletoriamente ocurra alguno de ellos. La duración de los cambios realizados está bajo discución: si son permanentes en el campo o no. En principio prefiero que sean por un tiempo determinado y no permanentes.

El cuanto a los controles del juego se propone usar joysticks implementados en arduino (ver [este link](https://www.brainy-bits.com/arduino-joystick-tutorial/)), sin embargo puede ser utíl dar la posibilidad al usuario de elegir entre el teclado y el joystick.

###### Control de acceso.
Se plantea usar usuario y contraseña para ambos jugadores. A su vez se ofrece la posibilidad de ingresar si se olvido la contraseña mediante una pregunta y respuesta secreta. Se ofrece tambien la posibilidad de creación y eliminación de usuarios, así como un score board de partidos ganados y perdidos y un historial de los partidos jugados entre usuarios.
###### Multijugador.
El juego en sí esta planteado para ser multijugador. **NO SE PLANTEA LA CREACION DE BOTS PARA PODER SER USADO EN SINGLE PLAYER**

###### Varios niveles de dificultad.
Esto se ve implementado directamente por el cambio de las propiedades del campo y del disco. Sin embargo si es necesario establecerlo explicitamente se puede correlacionar con el tamaño de las puertas y del disco. Tambien se tiene la idea de disponer de distintos campos (como por ejemplo puede ser campos con una propiedad especial como viscocidad muy baja, algun atractor o repulsor permanente en él, etc). En resumen una propiedad permanente del disco o del campo durante la partida.

###### Manejar Puntaje/Vidas.
Esto se verá reflejado en el score de cada jugador. En sí el juego acabará cuando alguno de los dos llegué a un puntaje determinado (que a su vez puede ser definido por el usuario).

###### Grabar sesión - Cargar sesión.
Se permitirá salvar el estado exacto de una partida, y a su vez su restitución a partir de ese punto exacto.





