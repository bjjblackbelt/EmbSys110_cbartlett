/**
 *******************************************************************************
 *  @filename   CriticalSection.cpp
 *  @brief      Implements a low-level driver for the timer peripheral.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "Timer.h"
#include "irq.h"
#include <stddef.h>

// TODO: CMB - Remove after testing
#include "CriticalSection.h"

// Define a shared resource for testing
static volatile uint32_t s_sharedCounter;
extern CriticalSection* g_pCritSect;
// TODO: CMB - Remove after testing

// Prototypes
extern "C"
{
extern void led_set(void);
extern void led_clr(void);
}

// Initialize statics
Timer_t* const Timer::s_timers[] = { TIMER0, TIMER1 };
static uint32_t s_sysTick = 0;
static uint32_t s_timer1TicksPerSec = 0;
volatile bool Timer::s_isErrorState = false; // TODO: CMB - Remove after testing

// Macros
#define TIME_MS_TO_TICK(ms) 	(((ms) * (s_timer1TicksPerSec))/1000)

/**
 * Constructor
 */
Timer::Timer()
{
	//!< Empty
}

/**
 * Destructor
 */
Timer::~Timer()
{
	//!< Empty
}

/**
 * Initialize the timer peripheral.
 * @param timer The timer to initialize
 */
void Timer::Open(TimerNumber_t timer, uint32_t timerInterval)
{
	// Enable
	switch (timer)
	{
		case TIMER_00:
		{
			// Enable power
			VOLATILE32(PCONP) |= BIT(1);

			// Enable peripheral clock
			VOLATILE32(PCLKSEL0) &= ~(0x00000004);
			VOLATILE32(PCLKSEL0) |= BIT(2);

			// Install interrupt handler in VIC
			install_irq(TIMER0_INT, (void*) &Timer::Timer0IntHandler,
			HIGHEST_PRIORITY);
			break;
		}
		case TIMER_01:
		{
			// Enable power
			VOLATILE32(PCONP) |= BIT(2);

			// Enable peripheral clock
			VOLATILE32(PCLKSEL0) &= ~(0x00000030);
			VOLATILE32(PCLKSEL0) |= BIT(4);

			// Install interrupt handler in VIC
			install_irq(TIMER1_INT, (void*) &Timer::Timer1IntHandler, HIGHEST_PRIORITY);

			// Configure sys tick
			s_timer1TicksPerSec = PCLK/(timerInterval * Timer::TIMER_PRESCALER);
			break;
		}
	}

	// Configure the timer
	s_timers[timer]->CTCR |= 0x00000000; /* Timer Mode: PC incremented every rising PCLK edge */
	s_timers[timer]->PR = Timer::TIMER_PRESCALER;
	s_timers[timer]->MR0 = timerInterval;
	s_timers[timer]->MCR |= 0x00000003;  /* Interrupt & reset on MR0: an interrupt is generated when MR0
	 	 	 	 	 	 	 	 	 	 	matches the value in the TC and TC is then reset. */
}

void Timer::Start(TimerNumber_t timer)
{
	// Enable counter
	s_timers[timer]->TCR |= 0x00000001;

	// Enable interrupt
	switch (timer)
	{
		case TIMER_00:
		{
			enable_irq(TIMER0_INT);
			break;
		}
		case TIMER_01:
		{
			enable_irq(TIMER1_INT);
			break;
		}
	}
}

void Timer::Stop(TimerNumber_t timer)
{
	// Disable counter
	s_timers[timer]->TCR &= ~(0x00000001);

	// Disable interrupt
	switch (timer)
	{
		case TIMER_00:
		{
			disable_irq(TIMER0_INT);
			break;
		}
		case TIMER_01:
		{
			disable_irq(TIMER1_INT);
			break;
		}
	}
}

/**
 * Reset the registers of the timer peripheral.
 * @param timer The timer to reset
 */
void Timer::Close(TimerNumber_t timer)
{
	// Disable
	switch (timer)
	{
		case TIMER_00:
		{
			VOLATILE32(PCONP) &= ~(BIT(1));
			VOLATILE32(PCLKSEL0) &= ~(0x00000004);
			disable_irq(TIMER0_INT);
			break;
		}
		case TIMER_01:
		{
			VOLATILE32(PCONP) &= ~(BIT(2));
			VOLATILE32(PCLKSEL0) &= ~(0x00000030);
			disable_irq(TIMER1_INT);
			break;
		}
	}

	// Reset timer registers
	s_timers[timer]->IR &= ~(0x0000003F);
	s_timers[timer]->TCR &= ~(0x00000002);
	s_timers[timer]->TC = 0x00000000;
	s_timers[timer]->PC = 0x00000000;
	s_timers[timer]->MCR &= ~(0x00000FFF);
	s_timers[timer]->MR0 = 0x00000000;
	s_timers[timer]->MR1 = 0x00000000;
	s_timers[timer]->MR2 = 0x00000000;
	s_timers[timer]->MR3 = 0x00000000;
	s_timers[timer]->CCR &= ~(0x0000003F);
	s_timers[timer]->EMR &= ~(0x00000FFF);
	s_timers[timer]->CTCR &= ~(0x0000000F);
}

/**
 * Timer 0 interrupt handler
 */
extern "C" void Timer::Timer0IntHandler()
{
	// Clear Timer0 interrupt
	s_timers[TIMER_00]->IR = 0x01;

	// Handle nested interrupt
	IENABLE;

	static bool toggle = false;
	switch (toggle)
	{
		case false:
		{
			led_set();
			break;
		}
		case true:
		{
			led_clr();
			break;
		}
	}
	toggle = !toggle;

	ProcessCounterTest(static_cast<int>(Timer::TIMER_00));

	IDISABLE;

	// Acknowledge interrupt
	VICVectAddr = 0;
}

/**
 * Timer 0 interrupt handler
 */
extern "C" void Timer::Timer1IntHandler()
{
	// Clear Timer1 interrupt
	s_timers[TIMER_01]->IR = 0x01;

	// Handle nested interrupt
	IENABLE;

	ProcessCounterTest(static_cast<int>(Timer::TIMER_01));

	s_sysTick++;

	IDISABLE;

	// Acknowledge interrupt
	VICVectAddr = 0;
}

/**
 * Tests the functionality of the critical section implementation.
 * @param threadID ID of the current timer.
 */
void Timer::ProcessCounterTest(int threadID)
{
	if (s_isErrorState == false)
	{
		// Take Critical Section
#define USE_CRIT_SECT (0U)
#if USE_CRIT_SECT
		CriticalSection::Status_t status = g_pCritSect->Query(threadID);
#else
		(void)threadID;
		CriticalSection::Status_t status = CriticalSection::SUCCESS;
#endif
		if (status == CriticalSection::SUCCESS)
		{
			s_sharedCounter += 1;
			if (s_sharedCounter != 1)
			{
				s_sysTick = 0;
				s_isErrorState = true;
			}
			else
			{
				s_sharedCounter -= 1;
			}

			// Release Critical Section
#if USE_CRIT_SECT
			g_pCritSect->Leave(threadID);
#endif
		}
	}
	else
	{
		if (s_sysTick < TIME_MS_TO_TICK(5000))
		{
			led_set();
		}
		else
		{
			s_sharedCounter = 0;
			s_isErrorState = false;
		}
	}
}

