# TP3 - Actividad 03: System Setup Menu (Diseño)

## Descripción
Diseño en Itemis Create del statechart `system_setup_menu`, un menú de configuración de
4 niveles (`ST_MAIN` -> `ST_MENU_1` -> `ST_MENU_2` -> `ST_MENU_3`) navegable con 3 eventos
(`EV_BTN_ENTER`, `EV_BTN_NEXT`, `EV_BTN_ESCAPE`), con un pseudoestado `Choice` en `ST_MENU_3`
que adapta el rango de valores editable según el parámetro seleccionado.

## Contenido de la carpeta
- `system_setup_menu/system_setup_menu_Export (1).scm` y `.png`: diseño del statechart en
  Itemis Create (variables `motor_idx`, `param_idx`, `val_idx`, `memory`).

## Niveles del menú
| Nivel | Estado | Acción (NEXT) | ENTER -> | ESCAPE -> |
|---|---|---|---|---|
| 0 | `ST_MAIN` | -- | `ST_MENU_1` | -- |
| 1 | `ST_MENU_1` | `motor_idx` = (`motor_idx` + 1) % 2 | `ST_MENU_2` | `ST_MAIN` |
| 2 | `ST_MENU_2` | `param_idx` = (`param_idx` + 1) % 3 | `ST_MENU_3` | `ST_MENU_1` |
| 3 | `ST_MENU_3` | `Choice`: incrementa `memory` (módulo 2 o 10 según `param_idx`) | `ST_MENU_2` | `ST_MENU_2` |