# TP3 - Actividad 04: System Setup Menu (Código C)

## Descripción
Codificación en C de los statecharts `task_sensor` (antirrebote de 4 botones) y `task_system`
(modos NORMAL/SETUP + menú de configuración de 4 niveles diseñado en la Actividad 03),
integrados junto con `task_actuator` (LED) y `task_display` (Actividad 02) en un único
firmware sobre Cyclic Executive de 1mS.

## Contenido de la carpeta
- Proyecto STM32CubeIDE completo: `app/` (tareas de aplicación), `Core/`, `Drivers/`.
- `tdse-tp3_04-display.md`: documentación de tiempos de ejecución (NOE/LET/BCET/WCET).

## Hardware utilizado
- Nucleo-F103RB.
- LCD 16x2 vía I2C (módulo PCF8574).
- 4 pulsadores: `BTN_A` (alternar NORMAL/SETUP), `BTN_ENT`, `BTN_NEX`, `BTN_ESC`.

## Resultado
Validado en hardware real: el menú de 4 niveles navega correctamente, el LCD refleja el
motor/parámetro/valor seleccionado, y los tiempos de ejecución de las 4 tareas respetan el
presupuesto de 1mS del Cyclic Executive.