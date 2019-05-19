#include "PlayerDataManager.h"
#include <string>
#include "../MainModule.h"
#include "../Constants.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../FileSystem/ConfigFile.h"
#include "../Constants.h"

const std::string PLAYER_DATA_FILE_NAME = "playerdata.ini";

namespace DivaHook::Components
{
	PlayerDataManager::PlayerDataManager()
	{
	}

	PlayerDataManager::~PlayerDataManager()
	{
		if (customPlayerData != nullptr)
			delete customPlayerData;
	}

	const char* PlayerDataManager::GetDisplayName()
	{
		return "player_data_manager";
	}

	void PlayerDataManager::Initialize()
	{
		playerData = (PlayerData*)PLAYER_DATA_ADDRESS;

		ApplyPatch();
		LoadConfig();
		ApplyCustomData();
	}

	void PlayerDataManager::Update()
	{
		if (false && Input::Keyboard::GetInstance()->IsTapped(VK_F12))
		{
			printf("PlayerDataManager::Update(): Loading config...\n");
			LoadConfig();
			ApplyCustomData();
		}
	}

	void PlayerDataManager::ApplyPatch()
	{
		DWORD oldProtect;
		VirtualProtect((void*)SET_DEFAULT_PLAYER_DATA_ADDRESS, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &oldProtect);
		{
			// prevent the PlayerData from being reset
			*(uint8_t*)(SET_DEFAULT_PLAYER_DATA_ADDRESS) = RET_OPCODE;
		}
		VirtualProtect((void*)SET_DEFAULT_PLAYER_DATA_ADDRESS, sizeof(uint8_t), oldProtect, &oldProtect);
	}

	void PlayerDataManager::LoadConfig()
	{
		if (playerData == nullptr)
			return;

		FileSystem::ConfigFile config(MainModule::GetModuleDirectory(), PLAYER_DATA_FILE_NAME);

		if (!config.OpenRead())
			return;

		if (customPlayerData != nullptr)
			delete customPlayerData;

		customPlayerData = new CustomPlayerData();

		auto parseInt = [&](const std::string &key)
		{
			std::string *stringBuffer;

			int result = 0;

			if (config.TryGetValue(key, stringBuffer))
			{
				result = std::stoi(stringBuffer->c_str());
				delete stringBuffer;
			}

			return result;
		};

		config.TryGetValue("player_name", customPlayerData->PlayerName);

		customPlayerData->LevelPlateId = parseInt("level_plate_id");
		customPlayerData->SkinEquip = parseInt("skin_equip");
		customPlayerData->BtnSeEquip = parseInt("btn_se_equip");
		customPlayerData->ChainslideSeEquip = parseInt("chainslide_se_equip");
	}

	void PlayerDataManager::ApplyCustomData()
	{
		// don't want to overwrite the default values
		auto setIfNotEqual = [](int *target, int value, int comparison)
		{
			if (value != comparison)
				*target = value;
		};

		setIfNotEqual(&playerData->level_plate_id, customPlayerData->LevelPlateId, 0);
		setIfNotEqual(&playerData->skin_equip, customPlayerData->SkinEquip, 0);
		setIfNotEqual(&playerData->btn_se_equip, customPlayerData->BtnSeEquip, -1);
		setIfNotEqual(&playerData->chainslide_se_equip, customPlayerData->ChainslideSeEquip, -1);

		if (customPlayerData->PlayerName != nullptr)
		{
			playerData->field_DC = 0x10;
			playerData->player_name = (char*)customPlayerData->PlayerName->c_str();
		}
	}
}
