# Control de posición para motor brushless con PIC16F18426 usando ADC, PWM y comunicación serial

## Descripción

Este proyecto implementa un controlador embebido de posición para un motor brushless utilizando el microcontrolador PIC16F18426. El sistema lee una referencia de posición mediante un potenciómetro analógico, mide la posición actual del motor mediante un segundo potenciómetro (lineal) y calcula una acción de control tipo PID para accionar el motor mediante una señal PWM y una señal de dirección.

El firmware está estructurado de forma modular, separando la lectura de sensores, el cálculo de control, la comunicación serial, el manejo del motor y la tarea principal de aplicación. El sistema también permite visualizar variables internas en tiempo real por UART para facilitar pruebas, depuración y ajuste del controlador.

## Objetivo del proyecto

Construir un sistema de control de posición que permita:

* leer una referencia analógica de posición.
* leer la posición actual del motor.
* calcular el error entre referencia y posición.
* implementar un controlador proporcional/PID en el microcontrolador.
* accionar el motor mediante PWM y dirección.
* transmitir variables internas por comunicación serial para visualización en tiempo real.
* modificar parámetros de control mediante comandos seriales.

## Contexto académico

El proyecto nace como parte del desarrollo de un sistema de control embebido para un actuador lineal. La práctica busca integrar adquisición analógica, comunicación serial, temporización por interrupciones y control digital en un microcontrolador de recursos limitados.

Entre los requisitos principales del sistema se encuentran:

* lectura de señales analógicas mediante ADC.
* control de un motor usando PWM.
* comunicación UART con un computador.
* transmisión de variables de control en tiempo real.
* ejecución periódica del control a una frecuencia de muestreo definida.

## Arquitectura general

El sistema se divide en varios bloques funcionales:

### 1. Tarea principal de aplicación

El módulo `task` coordina el flujo general del programa. Su función es mantener el `main.c` limpio y ejecutar periódicamente las tareas principales del sistema.

Este módulo se encarga de:

* inicializar sensores, control y motor.
* ejecutar la tarea serial.
* esperar la bandera de muestreo generada por el timer.
* leer los ADCs.
* actualizar las estructuras internas.
* calcular el control.
* aplicar la salida al motor.
* enviar datos por UART.

### 2. Lectura de sensores

El módulo `sensors` almacena las lecturas de los dos sensores analógicos del sistema:

* `AnSetpoint`: potenciómetro de referencia.
* `Position`: potenciómetro de posición del motor.

Las lecturas se manejan directamente en unidades ADC de 12 bits, es decir, en el rango:

```text id="w0lcmb"
0 - 4095
```

Esto permite evitar conversiones innecesarias durante la ejecución del controlador.

### 3. Control digital

El módulo `control` almacena y actualiza las variables principales del controlador:

* setpoint.
* error.
* error anterior.
* integral acumulada.
* salida de control.
* constantes `Kp`, `Ki` y `Kd`.
* modo de referencia analógica o serial.

La salida del controlador se calcula en enteros usando una escala de ganancia para evitar operaciones en punto flotante dentro del microcontrolador.

### 4. Manejo del motor

El módulo `motor` convierte la salida lógica del controlador en una señal física para el actuador.

El motor se acciona mediante:

* una señal PWM.
* una señal digital de dirección.

La salida del controlador puede ser positiva o negativa. El signo determina el sentido de giro y la magnitud determina el ciclo útil aplicado al PWM.

### 5. Comunicación serial

El módulo `serial` permite comunicación UART con un computador. Se utiliza para dos propósitos principales:

* recibir comandos de configuración.
* enviar variables internas para visualización en tiempo real.

La transmisión de datos hacia SerialPlot se realiza en formato binario usando cuatro canales de 16 bits.

## Hardware base

### Microcontrolador

* PIC16F18426
* Frecuencia de operación: 32 MHz
* ADC de 12 bits
* PWM por periférico PWM6
* EUSART para comunicación serial
* Timer1 para temporización del control
* PPS para asignación flexible de periféricos

### Sensores

El sistema utiliza dos entradas analógicas:

* potenciómetro de setpoint.
* potenciómetro de posición.

Ambas señales se leen como valores ADC entre `0` y `4095`.

### Salida hacia el motor

El control del motor se realiza mediante:

* `PWM6`: señal de velocidad o magnitud de actuación.
* `DIR`: señal de dirección.

La salida lógica del controlador se limita antes de convertirse a ciclo útil PWM para proteger el sistema durante las pruebas.

## Funcionamiento esperado

El ciclo general de funcionamiento es:

1. Timer1 genera una bandera periódica de control.
2. La tarea principal detecta la bandera.
3. Se leen los canales ADC de setpoint y posición.
4. Se actualizan las estructuras de sensores.
5. Se selecciona el modo de referencia:
   * setpoint analógico.
   * setpoint enviado por serial.
6. Se calcula el error.
7. Se calcula la salida PID.
8. Se aplica la salida al motor.
9. Se transmiten cuatro variables por UART para visualización.

## Variables transmitidas

Actualmente se transmiten cuatro canales por comunicación serial:

```text id="w0lcmb"
setpoint
position
error + 4095
output + 4095
```

El error y la salida se transmiten con desplazamiento porque pueden tomar valores negativos. De esta forma pueden visualizarse como datos `uint16_t`.

La convención usada es:

```text id="vuyvlo"
error_plot = error + 4095
output_plot = output + 4095
```

Por tanto:

```text id="cpovdd"
4095 representa cero
mayor que 4095 representa valor positivo
menor que 4095 representa valor negativo
```

## Comandos esperados

La comunicación serial permite modificar parámetros del controlador y el modo de referencia.

Comandos implementados o previstos:

```text id="hu6kkc"
pxxx\n    ; actualizar constante proporcional Kp
ixxx\n    ; actualizar constante integral Ki
dxxx\n    ; actualizar constante derivativa Kd
Mxxx\n    ; fijar setpoint serial entre 0 y 4095
A\n       ; volver al modo de setpoint analógico
```

Ejemplos:

```text id="j9ao7r"
p200
M2048
A
```

En modo serial, el comando `Mxxx` define directamente el setpoint del controlador en unidades ADC.

En modo analógico, el setpoint se toma desde el potenciómetro conectado al canal `AnSetpoint`.

> [!TIP]
> Se encontró que se puede implementar un control tipo P con kp = 200 - 250 sin oscilaciones y un tiempo de respuesta razonable

## Estado actual del desarrollo

Actualmente el proyecto está enfocado en:

* lectura estable de los canales ADC.
* visualización de variables por UART.
* estructuración modular del firmware.
* implementación de control proporcional/PID en enteros.
* aplicación de la salida al motor mediante PWM y dirección.
* pruebas iniciales de respuesta del sistema.

## Consideraciones de diseño

* Las variables de posición y referencia se manejan directamente en escala ADC para reducir operaciones durante el ciclo de control.
* La comunicación serial se utiliza como herramienta de depuración y ajuste.
* El controlador evita el uso de `float` mediante escalamiento entero de ganancias.
* La salida del controlador se satura antes de aplicarse al motor.
* El Timer1 define la frecuencia de ejecución del lazo de control.
* La arquitectura modular facilita probar por separado sensores, serial, control y motor.

## Compilación

Este proyecto está pensado para compilarse con:

* MPLAB X IDE.
* XC8 Compiler.
* MCC Classic para inicialización de periféricos.
* Microcontrolador PIC16F18426.

El proyecto incluye archivos generados por MCC y módulos propios escritos en C.

## Estructura del firmware

```text id="e86imp"
main.c
task.c / task.h
sensors.c / sensors.h
control.c / control.h
serial.c / serial.h
motor.c / motor.h
mcc_generated_files/
```

Descripción general:

* `main.c`: inicialización del sistema y llamada continua a `AppTask()`.
* `task.c`: coordinación general del ciclo de control.
* `sensors.c`: almacenamiento de lecturas ADC.
* `control.c`: cálculo de error y salida PID.
* `serial.c`: recepción de comandos y transmisión de datos.
* `motor.c`: aplicación de salida mediante PWM y dirección.
* `mcc_generated_files`: configuración de periféricos generada por MCC.

## Próximas mejoras

* Ajustar experimentalmente las constantes `Kp`, `Ki` y `Kd` para obtener una respuesta más estable y óptima del sistema. Una vez definidas, se buscará reemplazar algunas multiplicaciones por constantes mediante corrimientos de bits, con el objetivo de reducir ciclos de reloj y optimizar la ejecución del controlador.
* Replantear la arquitectura del controlador hacia un enfoque basado en ecuaciones en diferencias, permitiendo una implementación más eficiente y cercana a una formulación discreta del sistema de control.
* Implementar una cola de comandos para mejorar la recepción serial, evitar pérdida de instrucciones y permitir una gestión más robusta de los comandos enviados desde el computador.
* Integrar el encoder diferencial nativo del motor utilizado, con el fin de reemplazar la medición por potenciómetro y mejorar la precisión de la realimentación de posición.

## Consideraciones personales

Este fue un proyecto que inicialmente subestimé, pero terminó convirtiéndose en uno de mis proyectos favoritos. Fue diseñado e implementado completamente por mí, lo que me permitió aprender muchísimo durante el proceso.

Además, representó un reto especialmente valioso porque lo desarrollé sin tener conocimientos previos formales sobre control en lazo cerrado, ya que aún no he cursado la asignatura de Control de Sistemas dentro de mi carrera de Ingeniería Mecatrónica. Aun así, el proyecto me permitió acercarme de forma práctica a conceptos como realimentación, error, acción proporcional, discretización, saturación y respuesta dinámica.

Definitivamente es un proyecto que quiero volver a visitar más adelante. Sin embargo, me interesa replantearlo desde un enfoque más robusto, especialmente reemplazando el potenciómetro mecánico por un encoder magnético, diferencial o un sensor similar. Esto permitiría evitar el acople mecánico directo requerido por el potenciómetro y mejorar la confiabilidad de la medición de posición.

## Autor

Alejandro Farfán Romero  
Ingeniería Mecatrónica  
Pontificia Universidad Javeriana  
Mayo 2026
