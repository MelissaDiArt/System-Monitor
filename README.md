# System Monitor

Con la ayuda de Qt vamos a crear nuestro propio monitor del sistema. Nos permitirá examinar: procesos en ejecución, conexiones abiertas, temperatura de la CPU, hardware del sistema etc...


## Compilación y ejecución

1. Descargar QT Creator 5.8.0 -> [QT Creator](https://www.qt.io/download/)
2. Descargar el documento o hacer un clone en su repositorio -> [GitHub](https://github.com/alu0100884012/soa-system-monitor-alu0100884012)
3. Acceder a QT -> Open Project -> Media Player
4. Compilar proyecto, para ello precionamos el botón ![Compilar](/../master/images/play.png), situado en la parte inferior izquierda.Se abrirá directamente la ventana del programa.

## Ventana principal

La ventana principal puede ser algo así:

![Ventana principal](/../master/images/1.png)

Podemos observar varias pestañas:

## Sensores

Muestra la información de los sensores instalados en el hardware detectado por el sistema operativo. 

![Sensores](/../master/images/1.png)


## Lista de procesos

Muestra los procesos que están ejecutándose actualmente en la máquina mostrando algunas de sus paracterísticas como Pid, CmdLine, hilos ocupados etc...

![Procesos](/../master/images/2.png)


## Hardware

Muestra la información del hardware de nuestra máquina.

![Hardware](/../master/images/3.png)


## CPU

Muestra la información de la cpu de nuestra máquina.

![CPU](/../master/images/4.png)

##Red

Muestra la información de las conexiones TCP y UDP de nuestra máquina.

![Red](/../master/images/5.png)


##User

Muestra las los usuarios logueados en nuestra máquina.

![User](/../master/images/6.png)



## Técnicas de manejo de concurrencia

Considero que el método Half-Syns/Half-Async es el más simple de implementar puesto que tenemos una clase que engloba todos los métodos y solo debemos implementar en el programa principal los connect y una función para mostrar por pantalla por lo tanto es más organizado y estructurado.
Así mismo considero que es mas eficiente ya que para tareas de nivel superior utiliza un modelo sincrono que simplifica la programación simultánea y en las tareas de nivel inferior emplea un modelo asincrono que mejor la eficiencia en la ejecución.Este método escala con facilidad y de una forma ordenada ya que, como se puede ver en el código, cada tarea tiene su clase lo que ayuda a depurar el código cuando este alcanza un tamaño considerable. 





