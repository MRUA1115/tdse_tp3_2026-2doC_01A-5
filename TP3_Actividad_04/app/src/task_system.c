/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"

#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"
#include "task_display_attribute.h"
#include "task_display_interface.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define DEL_SYS_MIN			0ul
#define DEL_SYS_MED			250ul
#define DEL_SYS_MAX			500ul

/* Modes to excite Task System */
typedef enum task_system_mode {NORMAL, SETUP, MODE_QTY} task_system_mode_t;

#define SYSTEM_DTA_QTY	MODE_QTY
#define CANTIDAD_MOTOR 2
typedef struct {
	bool power;
	uint8_t speed;
	bool spin;

} motor_t ;

/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta_list[SYSTEM_DTA_QTY];

static motor_t motores[CANTIDAD_MOTOR];
uint8_t inx_motor = 0;
//typedef enum {POWER,SPEED,SPIN} param_inx_t;
uint8_t inx_param = 0;
//uint8_t inx_val = 0;
uint8_t memory = 0;


/********************** internal functions declaration ***********************/
void task_system_normal_statechart(void);
void task_system_setup_statechart(void);

void task_system_set_mode(task_system_mode_t);

void task_system_update_display(task_system_st_t state);

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking Code";
const char *p_task_system__ 	= "(Update by Time Code, period = 1mS)";

/********************** external data declaration ****************************/
task_system_mode_t g_task_system_mode;

/********************** external functions definition ************************/
void task_system_update_display(task_system_st_t state)
{
    char line0[17];
    char line1[17];

    switch (state)
    {
        case ST_SYS_MAIN:
            snprintf(line0, sizeof(line0), ">SETUP          ");
            snprintf(line1, sizeof(line1), " ENTER=Ingresar ");
            break;

        case ST_SYS_MENU_1:
            /* Dos opciones: MOTOR_1 / MOTOR_2. inx_motor indica la seleccionada */
            if (inx_motor == 0)
            {
                snprintf(line0, sizeof(line0), ">MOTOR_1        ");
                snprintf(line1, sizeof(line1), " MOTOR_2        ");
            }
            else
            {
                snprintf(line0, sizeof(line0), ">MOTOR_2        ");
                snprintf(line1, sizeof(line1), " MOTOR_1        ");
            }
            break;

        case ST_SYS_MENU_2:
        {
            /* Tres opciones: POWER / SPEED / SPIN, ciclan con NEXT.
               Mostramos la actual arriba y la siguiente (preview) abajo */
            const char *nombres[3] = { "POWER", "SPEED", "SPIN " };
            uint8_t sig = (inx_param + 1) % 3;

            snprintf(line0, sizeof(line0), ">M%d %s          ", inx_motor + 1, nombres[inx_param]);
            snprintf(line1, sizeof(line1), " %s             ", nombres[sig]);
            break;
        }

        case ST_SYS_MENU_3:
        {
            if (inx_param == 0) /* POWER: bool */
            {
                snprintf(line0, sizeof(line0), "POWER           ");
                snprintf(line1, sizeof(line1), ">%s             ", memory ? "ENCENDIDO" : "APAGADO  ");
            }
            else if (inx_param == 1) /* SPEED: numérico 0-9 */
            {
                snprintf(line0, sizeof(line0), "SPEED           ");
                snprintf(line1, sizeof(line1), ">%d              ", memory);
            }
            else /* SPIN: bool -> sentido de giro */
            {
                snprintf(line0, sizeof(line0), "SPIN            ");
                snprintf(line1, sizeof(line1), ">%s             ", memory ? "HORARIO  " : "ANTIHORARIO");
            }
            break;
        }

        default:
            snprintf(line0, sizeof(line0), "                ");
            snprintf(line1, sizeof(line1), "                ");
            break;
    }

    put_event_task_display(0, 0, line0);
    put_event_task_display(0, 1, line1);
}
void task_system_init(void *parameters)
{
	uint32_t index;
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - Tick [mS] = %lu", GET_NAME(task_system_init), HAL_GetTick());
	LOGGER_INFO("   %s is a %s", GET_NAME(task_system), p_task_system);
	LOGGER_INFO("   %s is a %s", GET_NAME(task_system), p_task_system_);
	LOGGER_INFO("   %s is a %s", GET_NAME(task_system), p_task_system__);

	init_event_task_system();

	task_system_set_mode(NORMAL);

	for (index = 0; SYSTEM_DTA_QTY > index; index++)
	{
		/* Update Task System Data Pointer */
		p_task_system_dta = &task_system_dta_list[index];

		/* Init & Print out: Task execution FSM */
		state = ST_SYS_IDLE;
		p_task_system_dta->state = state;

		event = EV_SYS_IDLE;
		p_task_system_dta->event = event;

		b_event = false;
		p_task_system_dta->flag = b_event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event,
					GET_NAME(b_event), (b_event ? "true" : "false"));
	}

	put_event_task_display(0, 0, "task_system_mode");
	put_event_task_display(0, 1, " NORMAL         ");

	task_system_set_mode(NORMAL);
}

void task_system_update(void *parameters)
{
	/* Run Task Statechart */
	switch (g_task_system_mode)
	{
		case NORMAL:

			task_system_normal_statechart();

			break;

		case SETUP:

			task_system_setup_statechart();

			break;

		default:

			task_system_set_mode(NORMAL);

			break;
		}
}

void task_system_normal_statechart(void)
{
	task_system_dta_t *p_task_system_dta;

	/* Update Task System Data Pointer */
	p_task_system_dta = &task_system_dta_list[NORMAL];

	if (true == any_event_task_system())
	{
		p_task_system_dta->flag = true;
		p_task_system_dta->event = get_event_task_system();
	}

	switch (p_task_system_dta->state)
	{
		case ST_SYS_IDLE:

			if ((true == p_task_system_dta->flag) && (EV_SYS_BTN_A == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;
				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_ACTIVE;

				put_event_task_display(0, 0, "task_system_mode");
				put_event_task_display(0, 1, " SETUP          ");

				task_system_set_mode(SETUP);
			}

			break;

		case ST_SYS_ACTIVE:

			if ((true == p_task_system_dta->flag) && (EV_SYS_IDLE == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;
				put_event_task_actuator(EV_LED_IDLE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_IDLE;
			}

			break;

		default:

			p_task_system_dta->tick  = DEL_SYS_MIN;
			p_task_system_dta->state = ST_SYS_IDLE;
			p_task_system_dta->event = EV_SYS_IDLE;
			p_task_system_dta->flag = false;

			break;
	}
}

void task_system_setup_statechart(void)
{
	task_system_dta_t *p_task_system_dta;
		/* Update Task System Data Pointer */
	p_task_system_dta = &task_system_dta_list[SETUP];

	if (true == any_event_task_system())
	{
		p_task_system_dta->flag = true;
		p_task_system_dta->event = get_event_task_system();
	}

	switch (p_task_system_dta->state)
	{
		case ST_SYS_MAIN:

			if ((true == p_task_system_dta->flag) && (EV_SYS_ENTER == p_task_system_dta->event))
			{
				inx_motor = 0;
				p_task_system_dta->flag = false;
				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MENU_1;

				//Acá tiene que ir que el display ponga MENU_1
				 task_system_update_display(ST_SYS_MENU_1);

			}
			if ((true == p_task_system_dta->flag) && (EV_SYS_ESCAPE == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;

				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MAIN;

				task_system_update_display(ST_SYS_MAIN);

				task_system_set_mode(NORMAL);
			}
			break;

		case ST_SYS_MENU_1:

			if ((true == p_task_system_dta->flag) && (EV_SYS_ENTER == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;
				inx_param = 0;
				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MENU_2;

				//Acá tiene que ir que el display ponga MENU_2
				 task_system_update_display(ST_SYS_MENU_2);
			}
			if ((true == p_task_system_dta->flag) && (EV_SYS_NEXT == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;


				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				inx_motor = (inx_motor +1)%CANTIDAD_MOTOR;


		        task_system_update_display(ST_SYS_MENU_1); /* se queda en MENU_1, solo cambia inx_motor */

			}
			if ((true == p_task_system_dta->flag) && (EV_SYS_ESCAPE == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;

				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MAIN;

				   task_system_update_display(ST_SYS_MAIN);

			}
			break;

		case ST_SYS_MENU_2:

			if ((true == p_task_system_dta->flag) && (EV_SYS_ENTER == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;
				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MENU_3;

				if (inx_param == 0){
					memory = motores[inx_motor].power;

				}
				else if(inx_param == 1){
					memory = motores[inx_motor].speed;

				}
				else {
					memory = motores[inx_motor].spin;

				}
				//Acá tiene que ir que el display ponga MENU_3
				task_system_update_display(ST_SYS_MENU_3);
			}
			if ((true == p_task_system_dta->flag) && (EV_SYS_NEXT == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;


				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);

				inx_param = (inx_param +1)%3;


				task_system_update_display(ST_SYS_MENU_2);
			}
			if ((true == p_task_system_dta->flag) && (EV_SYS_ESCAPE == p_task_system_dta->event))
			{
				p_task_system_dta->flag = false;
				memory = 0;

				put_event_task_actuator(EV_LED_ACTIVE, ID_LED_A);
				p_task_system_dta->state = ST_SYS_MENU_1;

				task_system_update_display(ST_SYS_MENU_1);


			}
			break;
		case ST_SYS_MENU_3:

			if ((true == p_task_system_dta->flag) && (EV_SYS_ENTER == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;

					if (inx_param == 0){
						motores[inx_motor].power = memory;

					}
					else if(inx_param == 1){
						motores[inx_motor].speed = memory;
					}
					else {
						motores[inx_motor].spin = memory;

					}

					put_event_task_actuator(EV_LED_IDLE, ID_LED_A);

					p_task_system_dta->state = ST_SYS_MENU_2;
					task_system_update_display(ST_SYS_MENU_2);
					memory = 0;
				}
			if ((true == p_task_system_dta->flag) && (EV_SYS_NEXT == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					if (inx_param == 0){
						memory = (memory + 1)%2;

					}
					else if(inx_param == 1){
						memory = (memory + 1)%10;

					}
					else {
						memory = (memory + 1)%2;

					}
					put_event_task_actuator(EV_LED_IDLE, ID_LED_A);
					task_system_update_display(ST_SYS_MENU_3);
				}

			if ((true == p_task_system_dta->flag) && (EV_SYS_ESCAPE == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_LED_IDLE, ID_LED_A);
					p_task_system_dta->state = ST_SYS_MENU_2;
					task_system_update_display(ST_SYS_MENU_2);
					memory = 0;
				}


			break;

		default:
		    p_task_system_dta->tick  = DEL_SYS_MIN;
		    p_task_system_dta->state = ST_SYS_MAIN;   // antes decía ST_SYS_IDLE
		    p_task_system_dta->event = EV_SYS_IDLE;
		    p_task_system_dta->flag  = false;
		    inx_motor = 0;
			inx_param = 0;
			memory = 0 ;

		    break;
	}
}

void task_system_set_mode(task_system_mode_t task_system_mode)
{
	g_task_system_mode = task_system_mode;
}

/********************** end of file ******************************************/
