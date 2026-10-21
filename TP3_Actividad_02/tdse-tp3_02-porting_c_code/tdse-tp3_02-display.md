# Evolución de task_dta_list[index] para TP3 Actividad 02 (LCD Display Statechart - resumen):

## 1. Módulo Test - task_test_update()
Índice en task_dta_list: 0

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 2        | 2         | 2         |
| 3   | 2        | 2         | 2         |
| 4   | 3        | 2         | 3         |
| 5   | 2        | 2         | 3         |
| 6   | 2        | 2         | 3         |
| 7   | 2        | 2         | 3         |
| 8   | 2        | 2         | 3         |
| 9   | 2        | 2         | 3         |
| 10  | 3        | 2         | 3         |
| 11  | 2        | 2         | 3         |
| 12  | 2        | 2         | 3         |
| 13  | 2        | 2         | 3         |
| 14  | 2        | 2         | 3         |

## 2. Módulo Display - task_display_update()
Índice en task_dta_list: 1

| NOE | LET [us] | BCET [us] | WCET [us] |
|-----|----------|-----------|-----------|
| 1   | 2        | 2         | 2         |
| 2   | 40       | 2         | 40        |
| 3   | 40       | 2         | 40        |
| 4   | 2        | 2         | 40        |
| 5   | 40       | 2         | 40        |
| 6   | 40       | 2         | 40        |
| 7   | 40       | 2         | 40        |
| 8   | 2        | 2         | 40        |
| 9   | 40       | 2         | 40        |
| 10  | 40       | 2         | 40        |
| 11  | 40       | 2         | 40        |
| 12  | 2        | 2         | 40        |
| 13  | 40       | 2         | 40        |
| 14  | 40       | 2         | 40        |

## Conclusión

El módulo Test se mantiene en un rango de 2 a 3us, típico de una tarea liviana que solo
actualiza un contador y evalúa un temporizador. El módulo Display alterna entre 2us (ciclos
donde solo se escribe 1 carácter) y 40us (ciclos donde se envía un reposicionamiento de
cursor junto con la transmisión I2C completa al LCD). En ambos casos el WCET (3us y 40us
respectivamente) se mantiene muy por debajo del presupuesto de 1000us (1ms) del Cyclic
Executive, por lo que ambas tareas cumplen holgadamente con las restricciones temporales
del ejecutor cíclico.