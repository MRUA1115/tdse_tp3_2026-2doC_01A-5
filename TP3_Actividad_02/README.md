# TP3 - Actividad 02: LCD Display (Statechart)

## Descripción
Modelado e implementación del statechart `task_display`, encargado de escribir en el LCD
(16x2, vía I2C/PCF8574) sin bloquear el Cyclic Executive de 1mS. La escritura completa de
una pantalla se divide en 1 instrucción/dato por ciclo de 1mS (posicionamiento de cursor o
escritura de 1 carácter), evitando que el Cyclic Executive incumpla su presupuesto de tiempo.

## Contenido de la carpeta
- `task_display/task_display.scm` y `task_display.png`: diseño del statechart en Itemis Create
  (estados `ST_DSP_IDLE` / `ST_DSP_UPDATE`, evento `EV_DSP_UPDATE`).
- `tdse-tp3_02-porting_c_code/`: proyecto STM32CubeIDE con el código C portado a partir del
  statechart (`task_display.c`, `task_display_attribute.h`, `task_display_interface.c`, `display.c`).

## Resultado
El código fue validado en hardware real (Nucleo-F103RB + LCD 16x2 vía I2C), confirmando que
cada carácter se escribe en un ciclo de 1mS distinto sin exceder el presupuesto de tiempo del
Cyclic Executive.