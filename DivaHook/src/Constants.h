#pragma once
#include <stdint.h>

constexpr uint8_t NOP_OPCODE = 0x90;
constexpr uint8_t RET_OPCODE = 0xC3;
constexpr uint8_t JMP_OPCODE = 0xE9;

constexpr uint64_t ENGINE_UPDATE_HOOK_TARGET_ADDRESS	= 0x000000014018CC40;
constexpr uint64_t ENGINE_UPDATE_INPUT_ADDRESS			= 0x000000014018CBB0;

constexpr uint64_t CURRENT_GAME_STATE_ADDRESS			= 0x0000000140EDA810;
constexpr uint64_t RESOLUTION_WIDTH_ADDRESS				= 0x0000000140EDA8BC;
constexpr uint64_t RESOLUTION_HEIGHT_ADDRESS			= 0x0000000140EDA8C0;
		  
constexpr uint64_t SYSTEM_WARNING_ELAPSED_ADDRESS		= (0x00000001411A1430 + 0x68);
constexpr uint64_t DATA_INIT_STATE_ADDRESS				= 0x0000000140EDA7A8;
		  
constexpr uint64_t AET_FRAME_DURATION_ADDRESS			= 0x00000001409A0A58;
constexpr uint64_t PV_FRAME_RATE_ADDRESS				= 0x0000000140EDA7CC;
constexpr uint64_t FRAME_SPEED_ADDRESS					= 0x0000000140EDA798;
constexpr uint64_t FRAME_RATE_ADDRESS					= 0x0000000140EDA6D0;
		  
constexpr uint64_t INPUT_STATE_PTR_ADDRESS				= 0x0000000140EDA330;
constexpr uint64_t SLIDER_CTRL_TASK_ADDRESS				= 0x000000014CC5DE40;
constexpr uint64_t TASK_TOUCH_ADDRESS					= 0x000000014CC9EC30;
constexpr uint64_t SEL_PV_TIME_ADDRESS					= 0x000000014CC12498;
constexpr uint64_t PLAYER_DATA_ADDRESS					= 0x00000001411A8850;
constexpr uint64_t SET_DEFAULT_PLAYER_DATA_ADDRESS		= 0x00000001404A7370;

constexpr uint64_t UPDATE_TASKS_ADDRESS					= 0x000000014019B980;
constexpr uint64_t GLUT_SET_CURSOR_ADDRESS				= 0x00000001408B68E6;
constexpr uint64_t CHANGE_MODE_ADDRESS					= 0x00000001401953D0;
constexpr uint64_t CHANGE_SUB_MODE_ADDRESS				= 0x0000000140195260;