/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
 * The simply blinky demo is implemented and described in main_blinky.c.  The
 * more comprehensive test and demo application is implemented and described in
 * main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and FreeRTOS hook functions.
 *
 *******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *

 *
 *******************************************************************************
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>

/* This project provides two demo applications.  A simple blinky style demo
application, and a more comprehensive test and demo application.  The
mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
The blinky demo is implemented and described in main_blinky.c.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
demo application will be built.  The comprehensive test and demo application is
implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE	7201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	6407

/*-----------------------------------------------------------*/

/*
 * main_blinky() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 * main_full() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
//extern void main_blinky( void );
//extern void main_full( void );

/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * http://www.freertos.org/a00016.html for more information.
 */
//void vFullDemoTickHookFunction( void );

//void vFullDemoIdleFunction( void );

/*
 * This demo uses heap_5.c, so start by defining some heap regions.  It is not
 * necessary for this demo to use heap_5, as it could define one large heap
 * region.  Heap_5 is only used for test and example purposes.  See
 * http://www.freertos.org/a00111.html for an explanation.
 */
static void  prvInitialiseHeap( void );

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile( void );

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/*-----------------------------------------------------------*/


// Define algumas constantes para o sistema
#define NUM_MISSILES 10 // N�mero m�ximo de m�sseis por ataque
#define NUM_INTERCEPTORS 5 // N�mero m�ximo de interceptores por defesa
#define ATTACK_INTERVAL 5000 // Intervalo entre ataques em milissegundos
#define DEFENSE_INTERVAL 1000 // Intervalo entre defesas em milissegundos
#define WINDOW_WIDTH 800 // Largura da janela gr�fica
#define WINDOW_HEIGHT 600 // Altura da janela gr�fica
#define GRAVITY 9.8 // Acelera��o da gravidade
#define PI 3.14159 // Valor de pi

// Define algumas estruturas de dados para o sistema
struct Missile {
	float x; // Posi��o x do m�ssil
	float y; // Posi��o y do m�ssil
	float vx; // Velocidade x do m�ssil
	float vy; // Velocidade y do m�ssil
	float angle; // �ngulo de lan�amento do m�ssil
	float speed; // Velocidade de lan�amento do m�ssil
	int active; // Indica se o m�ssil est� ativo ou n�o
	int targeted; // Indica se o m�ssil est� direcionado a uma �rea habitada ou n�o
};

struct Area {
	float x; // Posi��o x da �rea
	float y; // Posi��o y da �rea
	float width; // Largura da �rea
	float height; // Altura da �rea
	int hit; // Indica se a �rea foi atingida por um m�ssil ou n�o
};

struct Missile missiles[NUM_MISSILES]; // Vetor de m�sseis
struct Missile interceptors[NUM_INTERCEPTORS]; // Vetor de interceptores
struct Area areas[3]; // Vetor de �reas habitadas
int num_missiles; // N�mero de m�sseis disparados pelo ataque
int num_interceptors; // N�mero de interceptores disparados pela defesa
int num_hits; // N�mero de m�sseis que atingiram as �reas
int num_intercepts; // N�mero de m�sseis que foram interceptados
QueueHandle_t attack_queue; // Fila de comunica��o entre o ataque e a defesa

// Declara as fun��es do sistema
void init(); // Inicializa o sistema
void attack(void *pvParameters); // Fun��o da tarefa de ataque
void defense(void *pvParameters); // Fun��o da tarefa de defesa
void monitor(void *pvParameters); // Fun��o da tarefa de monitor

void update(); // Atualiza o estado do sistema
void launch_missile(struct Missile *m, float x, float y, float angle, float speed); // Lan�a um m�ssil com os par�metros dados
void launch_interceptor(struct Missile *m, float x, float y, float angle, float speed); // Lan�a um interceptor com os par�metros dados
int is_in_area(struct Missile *m, struct Area *a); // Verifica se um m�ssil est� dentro de uma �rea
int is_intercepted(struct Missile *m1, struct Missile *m2); // Verifica se um m�ssil foi interceptado por outro
float random(float min, float max); // Gera um n�mero aleat�rio entre min e max
float to_radians(float degrees); // Converte graus em radianos
float to_degrees(float radians); // Converte radianos em graus

float calculate_angle(struct Missile* m);
float calculate_speed(struct Missile* m);

int main( void )
{
	/* This demo uses heap_5.c, so start by defining some heap regions.  heap_5
	is only used for test and example reasons.  Heap_4 is more appropriate.  See
	http://www.freertos.org/a00111.html for an explanation. */
	prvInitialiseHeap();

	/* Initialise the trace recorder.  Use of the trace recorder is optional.
	See http://www.FreeRTOS.org/trace for more information. */
	vTraceEnable( TRC_START );

	// Inicializa o sistema
	init();

	// Cria as tarefas do FreeRTOS
	xTaskCreate(attack, "Attack", 1000, NULL, 1, NULL);
	xTaskCreate(defense, "Defense", 1000, NULL, 1, NULL);
	xTaskCreate(monitor, "Monitor", 1000, NULL, 1, NULL);

	// Inicia o escalonador do FreeRTOS
	vTaskStartScheduler();

	return 0;
}

// Fun��o que inicializa o sistema
void init() {
	// Inicializa as vari�veis globais
	num_missiles = 0;
	num_interceptors = 0;
	num_hits = 0;
	num_intercepts = 0;

	// Inicializa os m�sseis e os interceptores como inativos
	for (int i = 0; i < NUM_MISSILES; i++) {
		missiles[i].active = 0;
	}

	for (int i = 0; i < NUM_INTERCEPTORS; i++) {
		interceptors[i].active = 0;
	}

	// Inicializa as �reas habitadas com posi��es e tamanhos aleat�rios
	for (int i = 0; i < 3; i++) {
		areas[i].x = random(100, 200);
		areas[i].y = random(100, 200);
		areas[i].width = random(10, 50);
		areas[i].height = random(10, 50);
		areas[i].hit = 0;
	}

	// Cria a fila de comunica��o entre o ataque e a defesa
	attack_queue = xQueueCreate(10, sizeof(int));
}


 /* ----------Minhas defini��es de fun��es-------------------*/

// Fun��o da tarefa de ataque
void attack(void* pvParameters) {
	// Declara uma vari�vel para armazenar o n�mero de m�sseis a serem disparados
	int n;

	// Entra em um loop infinito
	while (1) {
		// Gera um n�mero aleat�rio de m�sseis a serem disparados entre 1 e NUM_MISSILES
		n = random(1, NUM_MISSILES);

		// Envia o n�mero de m�sseis para a fila de comunica��o
		xQueueSend(attack_queue, &n, portMAX_DELAY);

		// Lan�a os m�sseis com par�metros aleat�rios
		for (int i = 0; i < n; i++) {
			launch_missile(&missiles[i], 0, WINDOW_HEIGHT, random(10, 80), random(100, 200));
		}

		// Atualiza o n�mero de m�sseis disparados
		num_missiles += n;

		printf("ATAQUE");
		// Aguarda um intervalo aleat�rio entre 1 e ATTACK_INTERVAL milissegundos
		vTaskDelay(random(1, ATTACK_INTERVAL) / portTICK_PERIOD_MS);
	}
}

// Fun��o da tarefa de defesa
void defense(void* pvParameters) {
	// Declara uma vari�vel para armazenar o n�mero de m�sseis recebidos
	int n;

	// Entra em um loop infinito
	while (1) {
		// Recebe o n�mero de m�sseis da fila de comunica��o
		xQueueReceive(attack_queue, &n, portMAX_DELAY);

		// Percorre os m�sseis recebidos
		for (int i = 0; i < n; i++) {
			// Verifica se o m�ssil est� direcionado a uma �rea habitada
			if (missiles[i].targeted) {
				// Lan�a um interceptor com par�metros calculados para interceptar o m�ssil
				launch_interceptor(&interceptors[i], WINDOW_WIDTH, WINDOW_HEIGHT, calculate_angle(&missiles[i]), calculate_speed(&missiles[i]));
			}
		}

		// Atualiza o n�mero de interceptores disparados
		num_interceptors += n;

		printf("DEFESA");
		// Aguarda um intervalo de DEFENSE_INTERVAL milissegundos
		vTaskDelay(DEFENSE_INTERVAL / portTICK_PERIOD_MS);
	}
}

// Fun��o da tarefa de monitor
void monitor(void* pvParameters) {
	// Entra em um loop infinito
	while (1) {
		//vTaskDelay(10 / portTICK_PERIOD_MS);
		update();

	}
}
// Fun��o que atualiza o estado do sistema
void update() {
	// Percorre os m�sseis
	for (int i = 0; i < NUM_MISSILES; i++) {
		// Verifica se o m�ssil est� ativo
		if (missiles[i].active) {
			// Atualiza a posi��o do m�ssil de acordo com a sua velocidade e a gravidade
			missiles[i].x += missiles[i].vx;
			missiles[i].y += missiles[i].vy;
			missiles[i].vy += GRAVITY;

			// Verifica se o m�ssil saiu da janela gr�fica
			if (missiles[i].x < 0 || missiles[i].x > WINDOW_WIDTH || missiles[i].y < 0 || missiles[i].y > WINDOW_HEIGHT) {
				// Desativa o m�ssil
				missiles[i].active = 0;
			}

			// Percorre as �reas habitadas
			for (int j = 0; j < 3; j++) {
				// Verifica se o m�ssil est� dentro de uma �rea habitada
				if (is_in_area(&missiles[i], &areas[j])) {
					// Marca a �rea como atingida
					areas[j].hit = 0;

					// Atualiza o n�mero de m�sseis que atingiram as �reas
					num_hits++;

					// Desativa o m�ssil
					missiles[i].active = 0;
				}
			}
		}
	}

	// Percorre os interceptores
	for (int i = 0; i < NUM_INTERCEPTORS; i++) {
		// Verifica se o interceptor est� ativo
		if (interceptors[i].active) {
			// Atualiza a posi��o do interceptor de acordo com a sua velocidade e a gravidade
			interceptors[i].x += interceptors[i].vx;
			interceptors[i].y += interceptors[i].vy;
			interceptors[i].vy += GRAVITY;

			// Verifica se o interceptor saiu da janela gr�fica
			if (interceptors[i].x < 0 || interceptors[i].x > WINDOW_WIDTH || interceptors[i].y < 0 || interceptors[i].y > WINDOW_HEIGHT) {
				// Desativa o interceptor
				interceptors[i].active = 0;
			}

			// Percorre os m�sseis
			for (int j = 0; j < NUM_MISSILES; j++) {
				// Verifica se o m�ssil est� ativo e direcionado a uma �rea habitada
				if (missiles[j].active && missiles[j].targeted) {
					// Verifica se o m�ssil foi interceptado pelo interceptor
					if (is_intercepted(&missiles[j], &interceptors[i])) {
						// Atualiza o n�mero de m�sseis que foram interceptados
						num_intercepts++;

						// Desativa o m�ssil e o interceptor
						missiles[j].active = 0;
						interceptors[i].active = 0;
					}
				}
			}
		}
	}
}

void launch_missile(struct Missile* m, float x, float y, float angle, float speed) {
	// Atribui os valores de x, y, angle e speed ao m�ssil m
	m->x = x;
	m->y = y;
	m->angle = angle;
	m->speed = speed;

	// Calcula as componentes x e y da velocidade do m�ssil
	m->vx = speed * cos(to_radians(angle));
	m->vy = speed * sin(to_radians(angle));

	// Ativa o m�ssil
	m->active = 1;

	// Verifica se o m�ssil est� direcionado a uma �rea habitada
	m->targeted = 0;
	for (int i = 0; i < 3; i++) {
		if (is_in_area(m, &areas[i])) {
			m->targeted = 1;
			break;
		}
	}
}

int is_in_area(struct Missile* m, struct Area* a) {
	
	if (m->x >= a->x && m->x <= a->x + a->width && m->y >= a->y && m->y <= a->y + a->height) {
		// O m�ssil est� dentro da �rea
		return 0;
	}
	else {
		// O m�ssil n�o est� dentro da �rea
		return 1;
	}
}

int is_intercepted(struct Missile* m1, struct Missile* m2) {
	
	float tolerance = 10.0;
	float distance = sqrt((m1->x - m2->x) * (m1->x - m2->x) + (m1->y - m2->y) * (m1->y - m2->y));

	if (distance <= tolerance) {
		// Os m�sseis se interceptaram
		return 1;
	}
	else {
		// Os m�sseis n�o se interceptaram
		return 0;
	}
}

float random(float min, float max) {
	
	// Gera um n�mero inteiro entre 0 e RAND_MAX
	int r = rand();

	// Converte o n�mero inteiro em um n�mero decimal entre 0 e 1
	float f = (float)r / RAND_MAX;

	// Converte o n�mero decimal entre 0 e 1 em um n�mero decimal entre min e max
	float result = min + f * (max - min);

	// Retorna o resultado
	return result;
}

// Talvez precise
float to_radians(float degrees) {

	// Converte graus em radianos usando a f�rmula
	float radians = degrees * PI / 180;

	// Retorna o resultado
	return radians;
}

float to_degrees(float radians) {

	// Converte radianos em graus usando a f�rmula
	float degrees = radians * 180 / PI;

	// Retorna o resultado
	return degrees;
}

void launch_interceptor(struct Missile* m, float x, float y, float angle, float speed) {
	// Atribui os valores de x, y, angle e speed ao interceptor m
	m->x = x;
	m->y = y;
	m->angle = angle;
	m->speed = speed;

	// Calcula as componentes x e y da velocidade do interceptor
	// Note que a velocidade x � negativa, pois o interceptor � lan�ado da direita para a esquerda
	m->vx = -speed * cos(to_radians(angle));
	m->vy = speed * sin(to_radians(angle));

	// Ativa o interceptor
	m->active = 1;

	// N�o � necess�rio verificar se o interceptor est� direcionado a uma �rea habitada, pois ele s� � lan�ado para interceptar m�sseis que j� est�o
	// indo para uma area habitada
}
float calculate_angle(struct Missile* m) {

	// Calcula a dist�ncia horizontal entre o interceptor e o m�ssil
	float dx = m->x - WINDOW_WIDTH;

	// Calcula a dist�ncia vertical entre o interceptor e o m�ssil
	float dy = WINDOW_HEIGHT - m->y;

	// Calcula o �ngulo de lan�amento do interceptor usando a f�rmula
	float angle = atan2(dy, dx);

	// Converte o �ngulo de radianos para graus
	angle = to_degrees(angle);

	// Retorna o resultado
	return angle;
}

float calculate_speed(struct Missile* m) {

	// Calcula a dist�ncia horizontal entre o interceptor e o m�ssil
	float dx = m->x - WINDOW_WIDTH;

	// Calcula a dist�ncia vertical entre o interceptor e o m�ssil
	float dy = WINDOW_HEIGHT - m->y;

	// Calcula o �ngulo de lan�amento do interceptor usando a fun��o calculate_angle
	float angle = calculate_angle(m);

	// Converte o �ngulo de graus para radianos
	angle = to_radians(angle);

	// Calcula a velocidade de lan�amento do interceptor usando a f�rmula
	float speed = sqrt((GRAVITY * dx * dx) / (2 * (dy - dx * tan(angle)) * cos(angle) * cos(angle)));

	// Retorna o resultado
	return speed;
}


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
	size of the	heap available to pvPortMalloc() is defined by
	configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
	API function can be used to query the size of free heap space that remains
	(although it does not provide information on how the remaining heap might be
	fragmented).  See http://www.freertos.org/a00111.html for more
	information. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If application tasks make use of the
	vTaskDelete() API function to delete themselves then it is also important
	that vApplicationIdleHook() is permitted to return to its calling function,
	because it is the responsibility of the idle task to clean up memory
	allocated by the kernel to any task that has since deleted itself. */

	/* Uncomment the following code to allow the trace to be stopped with any
	key press.  The code is commented out by default as the kbhit() function
	interferes with the run time behaviour. */
	/*
		if( _kbhit() != pdFALSE )
		{
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
				xTraceRunning = pdFALSE;
			}
		}
	*/

	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		vFullDemoIdleFunction();
	}
	#endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  This function is
	provided as an example only as stack overflow checking does not function
	when running the FreeRTOS Windows port. */
	vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
	#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		vFullDemoTickHookFunction();
	}
	#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
	/* This function will be called once only, when the daemon task starts to
	execute	(sometimes called the timer task).  This is useful if the
	application includes initialisation code that would benefit from executing
	after the scheduler has been started. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
static BaseType_t xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

	printf( "ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError() );

 	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if( xPrinted == pdFALSE )
		{
			xPrinted = pdTRUE;
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			__asm{ NOP };
			__asm{ NOP };
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile( void )
{
FILE* pxOutputFile;

	fopen_s( &pxOutputFile, "Trace.dump", "wb");

	if( pxOutputFile != NULL )
	{
		fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
		fclose( pxOutputFile );
		printf( "\r\nTrace output saved to Trace.dump\r\n" );
	}
	else
	{
		printf( "\r\nFailed to create trace dump file\r\n" );
	}
}
/*-----------------------------------------------------------*/

static void  prvInitialiseHeap( void )
{
/* The Windows demo could create one large heap region, in which case it would
be appropriate to use heap_4.  However, purely for demonstration purposes,
heap_5 is used instead, so start by defining some heap regions.  No
initialisation is required when any other heap implementation is used.  See
http://www.freertos.org/a00111.html for more information.

The xHeapRegions structure requires the regions to be defined in start address
order, so this just creates one big array, then populates the structure with
offsets into the array - with gaps in between and messy alignment just for test
purposes. */
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
const HeapRegion_t xHeapRegions[] =
{
	/* Start address with dummy offsets						Size */
	{ ucHeap + 1,											mainREGION_1_SIZE },
	{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
	{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
	{ NULL, 0 }
};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT( ( ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE ) < configTOTAL_HEAP_SIZE );

	/* Prevent compiler warnings when configASSERT() is not defined. */
	( void ) ulAdditionalOffset;

	vPortDefineHeapRegions( xHeapRegions );
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

