# Evolución de task_dta_list[index] para TP3 Actividad 04 (System Setup Menu - resumen):

## 1. Módulo Sensor - task_sensor_update()

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 2        | 2         | 2         |
| 3   | 3        | 2         | 3         |
| 4   | 2        | 2         | 3         |
| 5   | 2        | 2         | 3         |
| 6   | 2        | 2         | 3         |
| 7   | 2        | 2         | 3         |
| 8   | 2        | 2         | 3         |
| 9   | 3        | 2         | 3         |
| 10  | 2        | 2         | 3         |
| 11  | 2        | 2         | 3         |
| 12  | 2        | 2         | 3         |
| 13  | 2        | 2         | 3         |
| 14  | 2        | 2         | 3         |

## 2. Módulo Sistema - task_system_update()

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 3        | 2         | 3         |
| 3   | 2        | 2         | 3         |
| 4   | 2        | 2         | 3         |
| 5   | 4        | 2         | 4         |
| 6   | 2        | 2         | 4         |
| 7   | 3        | 2         | 4         |
| 8   | 2        | 2         | 4         |
| 9   | 2        | 2         | 4         |
| 10  | 3        | 2         | 4         |
| 11  | 2        | 2         | 4         |
| 12  | 5        | 2         | 5         |
| 13  | 2        | 2         | 5         |
| 14  | 3        | 2         | 5         |

## 3. Módulo Actuador - task_actuator_update()

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 2        | 2         | 2         |
| 3   | 3        | 2         | 3         |
| 4   | 2        | 2         | 3         |
| 5   | 2        | 2         | 3         |
| 6   | 2        | 2         | 3         |
| 7   | 2        | 2         | 3         |
| 8   | 3        | 2         | 3         |
| 9   | 2        | 2         | 3         |
| 10  | 2        | 2         | 3         |
| 11  | 2        | 2         | 3         |
| 12  | 2        | 2         | 3         |
| 13  | 3        | 2         | 3         |
| 14  | 2        | 2         | 3         |

## 4. Módulo Display - task_display_update()

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 2        | 2         | 2         |
| 3   | 3        | 2         | 3         |
| 4   | 2        | 2         | 3         |
| 5   | 4        | 2         | 4         |
| 6   | 2        | 2         | 4         |
| 7   | 2        | 2         | 4         |
| 8   | 8        | 2         | 8         |
| 9   | 2        | 2         | 8         |
| 10  | 3        | 2         | 8         |
| 11  | 2        | 2         | 8         |
| 12  | 12       | 2         | 12        |
| 13  | 2        | 2         | 12        |
| 14  | 5        | 2         | 12        |

## Conclusión

El WCET máximo simultáneo entre las 4 tareas (Sensor=3us, Sistema=5us, Actuador=3us,
Display=12us) suma 23us en el peor caso, muy por debajo del presupuesto de 1000us (1ms)
del Cyclic Executive. Por lo tanto, las 4 tareas cumplen holgadamente con el presupuesto
de tiempo del ciclo de 1ms.