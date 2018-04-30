# (NOT) Air Hockey

## Justificación
El desafío de realizar este juego se justifica en base a que nos permitirá a nosotros como estudiantes desarrollar habilidades relacionadas con:

* La programación orientada a objetos (Object Oriented Programming en inglés).
* El modelado de sistemas físicos usando código, en particular sistemas basados en las leyes de la física mecánica.
* La comunicación de software con microcontroladores de la familia Arduino.
* Trabajo en equipo y gerencia de proyectos (que como ingenieros nos interesá al formar parte de las llamadas *soft skills*.
* Control de versiones mediante la plataforma github.
* Desarrollo de un GUI para el usuario.
* Desarrollo de los graficos del juego.
* La portabilidad del software en distintos ambientes.

## Planteamiento del problema
Se plantea realizar un juego de hockey de mesa en 2D que tenga propiedades variables, y en particular las propiedades físicas que modelan el movimiento del disco. Lo anterior usando C++, y Arduino para los controles de los usuarios. Para ello es necesario usar las habilidades aprendidas durante el curso, en especial las de la programación orientada a objetos y la comunicación mediante el puerto serial con arduino. A su vez será importante el trabajo en equipo, el control de versiones y la gerencia de proyectos. El nombre del juego será (NOT) Air Hockey.

## Objetivos (General y Específico)

#### Objetivos Generales

* Desarrollar un juego que cumpla con los requerimientos planteados por el curso y que a su vez haga buen uso de lo impartido en este.

#### Objetivos Específicos

* Modelar el campo de juego, el disco y los mazos de manera eficiente y simple.
* Establecer limites a los parámetros del juego para que no se lleguen a extremos que dificulten el gameplay -es decir injugable-.
* Desarrollar el aspecto visual de manera agradable a la vista pero minimalista (es decir simple).
* Implementar un sistema donde los datos de los usuarios sean guardados en documentos encriptados, y los mismo con las partidas e historial de ellas.
* Implementar la comunicación con el arduino de tal manera que los controles respondan correctamente a los movimientos del usuario.
* Desarrollar el motor físico del juego de manera a que corresponda correctamente a los principios de la fisica mecánica.
* Usar el control de versiones a nuestro favor para aumentar la productividad (y minimizar el tiempo requerido en el desarrollo).
* Implementar el juego de la manera mas portable posible entre distintos ambientes.

## Metodología

Se propone un juego de hockey de mesa en 2D que tenga caracteristicas variables, de tal manera que dinamicen el gameplay y aumenten la no-predictabilidad del juego. Los siguientes eventos plantean la modificación de estas variables:

* Alteración de la viscocidad del medio (es decir que tan facil la pelota se mueve por el campo debido a la fricción con este).
* Disco invisible por un corto periodo de tiempo.
* Inclusión de un atractor o repulsor en una posición determinada que altere el campo de fuerzas presente (y por consiguiente la aceleracion del disco).
* Alterar el tamaño de las puertas de cada uno de los jugadores.
* Alteración aletoria repentina en las componentes de velocidad del disco.
* Alteración de la restitución de las paredes del campo y/o de los mazos (que manejan los jugadores para darle al disco).
* Alteración del tamaño y masa del disco.

Dichos eventos podrían ser provocados de distintas maneras, sin embargo por los momentos no inclinamos a que en el campo de juego aparezcan unas "cajas sorpresa", al estilo de Mario Kart, de tal manera que cuando la pelota toque dicha caja aletoriamente ocurra alguno de ellos. La posicion de la caja esta aún por definir pero se propone que sea aletoria en una rango del campo. La duración de los cambios realizados está bajo discución tambien: si son permanentes en el campo o no. En principio se prefiere que sean por un tiempo determinado y no permanentes.

El cuanto a los controles del juego se propone usar joysticks implementados en arduino (ver [este link](https://www.brainy-bits.com/arduino-joystick-tutorial/)), sin embargo puede ser utíl dar la posibilidad al usuario de elegir entre el teclado y el joystick. Se puede explorar de usar otras formas de control en base a otros sensores pero esto será evaluado luego de conseguir el objetivo mas simple de manejarlo con controles tradicionales.

#### Control de acceso.
Se plantea usar usuario y contraseña para ambos jugadores. A su vez se ofrece la posibilidad de ingresar si se olvido la contraseña mediante una pregunta y respuesta secreta. Se ofrece tambien la posibilidad de creación y eliminación de usuarios, así como un score board de partidos ganados y perdidos y un historial de los partidos jugados entre usuarios.

#### Multijugador.
El juego en sí esta planteado para ser multijugador. **NO SE PLANTEA LA CREACION DE BOTS PARA PODER SER USADO EN SINGLE PLAYER**

#### Varios niveles de dificultad.
Esto se ve implementado directamente por el cambio de las propiedades del campo y del disco. Sin embargo si es necesario establecerlo explicitamente se puede correlacionar con el tamaño de las puertas y del disco. Tambien se tiene la idea de disponer de distintos campos (como por ejemplo puede ser campos con una propiedad especial como viscocidad muy baja, algun atractor o repulsor permanente en él, etc). En resumen una propiedad permanente del disco o del campo durante la partida.

#### Manejar Puntaje/Vidas.
Esto se verá reflejado en el score de cada jugador. En sí el juego acabará cuando alguno de los dos llegué a un puntaje determinado (que a su vez puede ser definido por el usuario).

#### Grabar sesión - Cargar sesión.
Se permitirá salvar el estado exacto de una partida, y a su vez su restitución a partir de ese punto exacto. Esto solo si son los mismo usuarios que estaban originalmente en la partida. En este sentido debe haber un estado de logged in y de logged out para dos jugadores simultaneamente.


# Cronograma de actividades.
Se cuenta con aproximadamente 5 semanas para concluir el proyecto. La carga será distribuida de la siguiente manera:
* S1: Modelamiento mediante clases del campo, disco y mazos.
* S2: Desarrollo del juego en crudo sin facilidades al usuario.
* S3: Implementacion del arduino para los controles y las visuales del juego.
* S4: Desarrollo de la interfaz grafica y del manejo de archivos para el juego.
* S5: Pruebas y retoques finales.

# Presupuesto.

|                  Artículo                  | Cantidad | Valor unitario | Valor Total |
|:------------------------------------------:|:--------:|:--------------:|:-----------:|
| Transporte                                 |     2    |     60.000     |   120.000   |
| Alimentación                               |     2    |     35.000     |    70.000   |
| Desgaste de las computadoras               |     2    |     100.000    |   200.000   |
| Joysticks                                  |     2    |     10.000     |    20.000   |
| Arduino                                    |     2    |     30.000     |    60.000   |
| Consumo de energía                         |     1    |     15.000     |    15.000   |
| Internet                                   |     1    |     70.000     |    70.000   |
| Tiempo invertido en el proyecto (en horas) |    420   |      4000      |  1.680.000  |
|                                            |          |      Total     |  2.235.000  |

# Integrantes del equipo

* Esteban Rodríguez Ordoñez
* Yorguin José Mantilla Ramos
