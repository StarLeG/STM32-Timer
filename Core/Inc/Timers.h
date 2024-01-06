#pragma once

#include "stdbool.h"
#include "main.h"

typedef struct {

	bool TON_Q; // TRUE - Запуск таймера, FALSE - Остановка и сброс таймера
	bool TON_M; // Память включения таймера
	uint32_t TON_ET; // время которое таймер уже проработал
	uint32_t TON_StartTime; // время начала отсчета

	bool TOF_Q;
	bool TOF_M;
	uint32_t TOF_ET;
	uint32_t TOF_StartTime;

	bool TP_Q;
	bool TP_M;
	uint32_t TP_ET;
	uint32_t TP_StartTime;
} Timers;


void Timer_TON(Timers *tim, bool in, uint32_t pt) {
	if (in) {
		if (!tim->TON_M) {
			tim->TON_StartTime = HAL_GetTick();
		}

		if (!tim->TON_Q) {
			tim->TON_ET = HAL_GetTick() - tim->TON_StartTime;
		}

		if (tim->TON_ET >= pt) {
			tim->TON_Q = true;
			tim->TON_ET = pt;
		}
	} else {
		tim->TON_Q = false;
		tim->TON_ET = 0;
	}

	tim->TON_M = in;
}

void Timer_TOF(Timers *tim, bool in, uint32_t pt){
	if(in){
		tim->TOF_Q = false;
		tim->TOF_ET = 0;
		if(tim->TOF_Q){
			tim->TOF_ET = HAL_GetTick() - tim->TOF_StartTime;
		}
		if(tim->TOF_ET >= pt){
			tim->TOF_Q = false;
			tim->TOF_ET = pt;
		}
	}
	tim->TOF_M = in;
}

void Timer_TP(Timers *tim, bool in, uint32_t pt){
	if(tim->TP_Q){
		tim->TP_ET = HAL_GetTick() - tim->TOF_StartTime;
		if(tim->TP_ET >= pt){
			tim->TP_Q = false;
		}

		if(!in){
			tim->TP_ET = 0;
		}else{
			tim->TP_ET = pt;
		}
	}else if(!in){
		tim->TP_ET = 0;
	}else if(in && tim->TP_ET == 0){
		tim->TP_StartTime = HAL_GetTick();
		tim->TP_Q = true;
		tim->TP_ET = 0;
	}
}
