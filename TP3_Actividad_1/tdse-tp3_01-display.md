**¿Puedes ayudarme a realizar un Trabajo Práctico sobre LCD Display (porting C code) - System Setup
(statechart - modeling - c coding)?**

Sí. Podemos hacerlo paso a paso, desde el modelado del sistema (Statechart) hasta el porting del código C al STM32CubeIDE, incluyendo la configuración de periféricos y la integración del LCD.

Si el TP está relacionado con la misma materia/proyecto donde venís trabajando con STM32CubeIDE + Itemis Create/Statecharts, podemos mantener el mismo enfoque.

Te propongo trabajar en este orden:

##1. Entender la consigna
Qué LCD se utiliza.
Qué microcontrolador/placa STM32.
Qué entradas y salidas tiene el sistema.
Qué comportamiento debe implementar.
Qué código C existente hay que portar.

##2. System Setup
Configuración del proyecto en STM32CubeIDE.
Clock.
GPIO.
Timers / interrupciones si corresponden.
Periférico utilizado para el LCD (GPIO, I²C, SPI, etc.).
Estructura de archivos .c y .h.

##3. Modelado
Identificar estados.
Eventos.
Variables de estado.
Transiciones.
Acciones entry, exit y/o durante el estado.
Eventualmente, diagrama temporal de funcionamiento.

##4. Statechart
Implementarlo en Itemis Create.
Definir la interfaz del statechart.
Generar el código C.
Entender qué archivos genera y cómo interactúan con el proyecto.

##5. Porting C code
Adaptar el código C original a STM32.
Separar la lógica del sistema de la HAL/LL de STM32.
Crear las funciones de abstracción necesarias para el LCD.
Integrar el código generado por el Statechart.

##6. Pruebas
Verificar cada función del LCD.
Verificar las transiciones del Statechart.
Probar el sistema completo sobre la Nucleo/STM32.
Si aparece algún error de compilación o comportamiento, lo depuramos juntos.


##1. Comportamiento de los archivos

**app.c**

Es el archivo principal de gestión de la aplicación. Define la lista de tareas del sistema (task_test y task_display) y se encarga de inicializarlas y ejecutarlas periódicamente.

La función app_init() inicializa los contadores de la aplicación, el contador de ciclos y todas las tareas registradas. También inicializa el sistema de interrupciones de la aplicación.

La función app_update() verifica si existe un tick pendiente generado por el SysTick. Cuando corresponde ejecutar las tareas, recorre la lista y llama a la función task_update() de cada una. Además, mide el tiempo de ejecución de cada tarea y actualiza los valores de ejecución mínima, máxima y última.

**app_it.c**

Gestiona los eventos asociados a las interrupciones de la aplicación. Contiene la variable global g_app_tick_cnt, utilizada como contador de ticks pendientes.

HAL_SYSTICK_Callback() se ejecuta ante cada interrupción del SysTick e incrementa g_app_tick_cnt. De esta manera, el SysTick proporciona la referencia temporal que permite ejecutar periódicamente las tareas desde app_update().

También contiene el callback de una interrupción externa del botón, aunque en el código proporcionado no realiza ninguna acción.

**systick.c**

Implementa systick_delay_us(), una función de demora bloqueante en microsegundos. Utiliza directamente el contador del periférico SysTick para medir el tiempo transcurrido y contempla el desbordamiento del contador.

Esta función es utilizada por el driver del display para generar las demoras necesarias durante la comunicación con el LCD.

**task_test_attribute.h**

Define la estructura de datos utilizada por la tarea de prueba:

typedef struct {
    uint32_t tick;
    uint32_t counter;
} task_test_dta_t;

tick funciona como temporizador interno de la tarea y counter como contador de ejecuciones. También declara la variable global task_test_dta.

**task_test.c**

Implementa la tarea de prueba. task_test_init() inicializa el temporizador y el contador, y genera los primeros mensajes que serán mostrados en el LCD:

LCD Display Test
 Porting C code

Luego, task_test_update() ejecuta periódicamente task_test_statechart().

La tarea utiliza un temporizador de 1000 ciclos. Como la aplicación se actualiza cada 1 ms, aproximadamente cada 1 segundo se genera una nueva actualización del display.

**task_display_attribute.h**

Define los datos utilizados por la tarea del display. El modelo considera un display de 2 filas y 16 columnas.

También define los eventos:

EV_DSP_IDLE
EV_DSP_UPDATE

y los estados:

ST_DSP_IDLE
ST_DSP_UPDATE

La estructura task_display_dta_t contiene el estado, evento, flag, memoria de display (ddram), posición de fila y columna y carácter.

**task_display_interface.c**

Implementa la función put_event_task_display(), que constituye la interfaz utilizada para solicitar una actualización del LCD.

La función copia el mensaje recibido a la memoria ddram, establece el evento EV_DSP_UPDATE y activa flag. De esta forma, no modifica directamente el LCD, sino que prepara los datos y notifica al Statechart que existe una actualización pendiente.

**task_display.c**

Implementa la tarea encargada de controlar la actualización del LCD mediante un Statechart.

task_display_init() inicializa el estado en ST_DSP_IDLE, el evento en EV_DSP_IDLE y el flag en false. También inicializa físicamente el display mediante displayInit() y muestra inicialmente el contenido de la memoria ddram.

task_display_update() ejecuta periódicamente task_display_statechart().

El Statechart posee dos estados: ST_DSP_IDLE y ST_DSP_UPDATE. Cuando se detecta un evento de actualización, pasa de IDLE a UPDATE. En UPDATE se escribe el contenido de las dos filas de ddram en el LCD y luego se vuelve a IDLE.

**display.h**

Define la interfaz pública del driver del LCD. Permite seleccionar comunicación mediante GPIO de 4 u 8 bits y declara las funciones para:

inicializar el display;
seleccionar la posición del cursor;
escribir una cadena de caracteres.


**display.c**

Implementa el driver de bajo nivel del LCD.

displayInit() realiza la secuencia de inicialización del display y configura el modo de comunicación seleccionado. En este proyecto se utiliza la conexión GPIO de 4 bits. También configura el display, limpia la pantalla y lo enciende.

displayCharPositionWrite() establece la posición de escritura mediante la dirección de la memoria DDRAM del LCD.

displayStringWrite() recorre una cadena de caracteres y envía cada carácter al display.

Las funciones privadas displayCodeWrite(), displayPinWrite() y displayDataBusWrite() se encargan de traducir las instrucciones y datos en señales sobre los GPIO utilizados para controlar el LCD. En el modo de 4 bits, cada byte se transmite mediante dos grupos de cuatro bits.

##2. Flujo de funcionamiento del proyecto

El funcionamiento general puede resumirse de la siguiente manera:

Inicio del sistema
       │
       ▼
   app_init()
       │
       ├── Inicializa contador de aplicación
       ├── Inicializa contador de ciclos
       ├── Inicializa task_test
       └── Inicializa task_display
       │
       ▼
   app_it_init()
       │
       ▼
   Sistema funcionando
       │
       ▼
  SysTick genera un tick
       │
       ▼
HAL_SYSTICK_Callback()
       │
       │ g_app_tick_cnt++
       ▼
   app_update()
       │
       ├───────────────┐
       ▼               ▼
task_test_update()  task_display_update()
       │               │
       ▼               ▼
task_test_          task_display_
statechart()        statechart()
       │               │
       │               └── Actualiza LCD
       │
       └── Cada ~1 s genera
           EV_DSP_UPDATE
                │
                ▼
       put_event_task_display()
                │
                ▼
          flag = true
          event = UPDATE
                │
                ▼
       task_display_statechart()
                │
                ▼
          Escribe en LCD

Por lo tanto, el SysTick proporciona la base temporal, app_update() funciona como mecanismo de ejecución periódica de las tareas, task_test genera periódicamente información para mostrar y task_display procesa esos eventos y actualiza el LCD.

##3. Comportamiento de task_test_statechart()

La función task_test_statechart() se ejecuta periódicamente, aproximadamente cada 1 ms.

En cada ejecución:

Incrementa el contador counter.
Decrementa el temporizador tick.
Cuando tick llega a cero, lo recarga con 1000.
Genera un evento EV_DSP_UPDATE.
Escribe en la memoria del display el texto "Test Nro: ******".
Calcula el número de prueba a partir del contador y lo escribe en la posición correspondiente del display.

De esta forma, aproximadamente cada 1 segundo se genera una nueva actualización del número de prueba.

4. Comportamiento de task_display_statechart()

La función task_display_statechart() implementa una máquina de estados de dos estados:

       EV_DSP_UPDATE
              │
              ▼
       ┌─────────────┐
       │ DSP_IDLE    │
       └──────┬──────┘
              │
              ▼
       ┌─────────────┐
       │ DSP_UPDATE  │
       └──────┬──────┘
              │
              ▼
        Actualizar LCD
              │
              ▼
          DSP_IDLE

En ST_DSP_IDLE, espera que flag sea verdadero y que el evento sea EV_DSP_UPDATE. Cuando esto ocurre, pasa al estado ST_DSP_UPDATE.

En ST_DSP_UPDATE, consume el evento, desactiva el flag, escribe las dos filas de la memoria ddram en el LCD y finalmente vuelve al estado ST_DSP_IDLE.

De esta manera, el Statechart separa la solicitud de actualización de la actualización física del display.


# Evolución de task_dta_list[index] para 10 pulsaciones (resume):

DEL_TEST_XX_MIN		    0ul
DEL_TEST_XX_MED		    5ul
DEL_TEST_XX_MAX	        10ul


Interrupción de ejecución en task_dta_list[index].NOE++; línea 160


| NOE | LET [µs]  | BCET [µs]  | WCET [µs] |
|-----|-----------|------------|-----------|
| 0   | 0         | 1000       | 0         |
| 1   | 2         | 2          | 2         |
| 3   | 2         | 2          | 2         |
| 4   | 2         | 2          | 2         |
| 5   | 2         | 2          | 2         |
| 6   | 2         | 2          | 2         |
| 7   | 2         | 2          | 2         |
| 8   | 2         | 2          | 2         |
| 9   | 2         | 2          | 2         |
| 10  | 2         | 2          | 2         |
| 11  | 2         | 2          | 2         |
| 12  | 37498     | 2          | 37498     |
| 13  | 2         | 2          | 37498     |
| 14  | 2         | 2          | 37498     |

