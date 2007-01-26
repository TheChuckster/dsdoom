/**********************************
  21/11/06 Jefklak - Get LED/RTC values and PersonalData
  Thank you Rick Wong! (Lick)
***********************************/
#ifndef _NDSX_LEDBLINK_
#define _NDSX_LEDBLINK_


#include <nds.h>

static int PERSONAL_NAME_I = 0;

/*
    PowerManagement bits that libnds doesn't define (yet?).
    Check out "arm7/serial.h".
*/
#define PM_LEDBLINK_STATUS  BIT(4)
#define PM_LEDBLINK_SPEED   BIT(5)

/*
    FIFO Message-IDs, shared between ARM7 and ARM9.
*/
#define GET_LEDBLINK_STATUS         (0x1211B107)
#define SET_LEDBLINK_ON             (0x1211B108)
#define SET_LEDBLINK_OFF            (0x1211B109)
#define SET_LEDBLINK_TOGGLE         (0x1211B110)

#define GET_LEDBLINK_SPEED          (0x1211B111)
#define SET_LEDBLINK_FAST           (0x1211B112)
#define SET_LEDBLINK_SLOW           (0x1211B113)
#define SET_LEDBLINKSPEED_TOGGLE    (0x1211B114)

#define GET_CLOCK_HOUR				(0x1211B115)
#define GET_PERSONAL_NAME_LEN		(0x1211B116)
#define GET_PERSONAL_NAME			(0x1211B117)

/*
    ARM9 functions to command the ARM7.

    NOTES:
  - swiWaitForVBlank() makes sure you don't interact with the ARM7
    while it's doing the writePM() and readPM() calls. (Those are
    SPI accessing functions and must run without being interrupted.)
  - When calling SPI-functions like writePM()/readPM(), you ACTUALLY
    REALLY have to DISABLE INTERRUPTS first. But swiWaitForVBlank()
    will suffice most of the times.
*/
#ifdef ARM9
inline s16 NDSX_GetPersonalName()
{
    REG_IPC_FIFO_TX = GET_PERSONAL_NAME;
    swiWaitForVBlank();
    while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY);
	return (s16) REG_IPC_FIFO_RX;
}
inline int NDSX_GetClockHour()
{
    REG_IPC_FIFO_TX = GET_CLOCK_HOUR;
    swiWaitForVBlank();
    while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY); 
    return (int)REG_IPC_FIFO_RX;
}
inline int NDSX_GetPersonalNameLen()
{
	REG_IPC_FIFO_TX = GET_PERSONAL_NAME_LEN;
    swiWaitForVBlank();
    while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY); 
    return (int)REG_IPC_FIFO_RX;
}
inline u32 NDSX_GetLedBlink_Status()
{
    REG_IPC_FIFO_TX = GET_LEDBLINK_STATUS;
    swiWaitForVBlank();
    while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY); 
    return (bool)REG_IPC_FIFO_RX;
}

inline void NDSX_SetLedBlink_On()
{
    REG_IPC_FIFO_TX = SET_LEDBLINK_ON;
    swiWaitForVBlank();
}

inline void NDSX_SetLedBlink_Off()
{
    REG_IPC_FIFO_TX = SET_LEDBLINK_OFF;
    swiWaitForVBlank();
}

inline void NDSX_SetLedBlink_Toggle()
{
    REG_IPC_FIFO_TX = SET_LEDBLINK_TOGGLE;
    swiWaitForVBlank();
}

inline u32 NDSX_GetLedBlink_Speed()
{
    REG_IPC_FIFO_TX = GET_LEDBLINK_SPEED;
    swiWaitForVBlank();
    while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY); 
    return (bool)REG_IPC_FIFO_RX;
}

inline void NDSX_SetLedBlink_Fast()
{
    REG_IPC_FIFO_TX = SET_LEDBLINK_FAST;
    swiWaitForVBlank();
}

inline void NDSX_SetLedBlink_Slow()
{
    REG_IPC_FIFO_TX = SET_LEDBLINK_SLOW;
    swiWaitForVBlank();
}

inline void NDSX_SetLedBlinkSpeed_Toggle()
{
    REG_IPC_FIFO_TX = SET_LEDBLINKSPEED_TOGGLE;
    swiWaitForVBlank();
}
#endif // ARM9


/*
    ARM7 code.

    NOTES:
  - The LedBlinkFifo should be called from an 
    ARM7-fifo-on-recv-interrupt-handler.
*/
#define rtc24hours ((IPC->rtc_hours > 51) ? IPC->rtc_hours-40 : IPC->rtc_hours)
#ifdef ARM7
inline void NDSX_ClockFifo(u32 msg)
{
	if(msg == GET_CLOCK_HOUR)
	{
        //REG_IPC_FIFO_TX = (int)rtc24hours;
		REG_IPC_FIFO_TX = (int)IPC->time.rtc.day;
		return;
	}
	if(msg == GET_PERSONAL_NAME_LEN)
	{
		REG_IPC_FIFO_TX = (int)PersonalData->nameLen;
		return;
	}
	if(msg == GET_PERSONAL_NAME)
	{
		REG_IPC_FIFO_TX = (s16)PersonalData->name[PERSONAL_NAME_I];
		PERSONAL_NAME_I++;
		return;
	}
}
inline void NDSX_LedBlinkFifo(u32 msg)
{
    if(msg == GET_LEDBLINK_STATUS)
    {
        REG_IPC_FIFO_TX = (bool)(readPowerManagement(PM_CONTROL_REG) & PM_LEDBLINK_STATUS);
        return;
    }
    else if(msg == SET_LEDBLINK_OFF)
    {
        u32 reg_blink_off = readPowerManagement(PM_CONTROL_REG) & ~PM_LEDBLINK_STATUS;
        writePowerManagement(PM_CONTROL_REG, reg_blink_off);
        return;
    }
    else if(msg == SET_LEDBLINK_ON)
    {
        u32 reg_blink_on = readPowerManagement(PM_CONTROL_REG) | PM_LEDBLINK_STATUS;
        writePowerManagement(PM_CONTROL_REG, reg_blink_on);
        return;
    }
    else if(msg == SET_LEDBLINK_TOGGLE)
    {
        u32 oldreg = readPowerManagement(PM_LEDBLINK_STATUS);
        if(oldreg & PM_LEDBLINK_STATUS) 
            oldreg &= ~PM_LEDBLINK_STATUS;
        else
            oldreg |= PM_LEDBLINK_STATUS;
        writePowerManagement(PM_CONTROL_REG, oldreg);
        return;
    }
    else if(msg == GET_LEDBLINK_SPEED)
    {
        REG_IPC_FIFO_TX = (bool)(readPowerManagement(PM_CONTROL_REG) & PM_LEDBLINK_SPEED);
        return;
    }
    else if(msg == SET_LEDBLINK_SLOW)
    {
        u32 reg_blink_slow = readPowerManagement(PM_CONTROL_REG) & ~PM_LEDBLINK_SPEED;
        writePowerManagement(PM_CONTROL_REG, reg_blink_slow);
        return;
    }
    else if(msg == SET_LEDBLINK_FAST)
    {
        u32 reg_blink_fast = readPowerManagement(PM_CONTROL_REG) | PM_LEDBLINK_SPEED;
        writePowerManagement(PM_CONTROL_REG, reg_blink_fast);
        return;
    }
    else if(msg == SET_LEDBLINKSPEED_TOGGLE)
    {
        u32 oldreg = readPowerManagement(PM_LEDBLINK_SPEED);
        if(oldreg & PM_LEDBLINK_SPEED) 
            oldreg &= ~PM_LEDBLINK_SPEED;
        else
            oldreg |= PM_LEDBLINK_SPEED;
        writePowerManagement(PM_CONTROL_REG, oldreg);
        return;
    }
}
#endif // ARM7


#endif // _NDSX_BRIGHTNESS_
