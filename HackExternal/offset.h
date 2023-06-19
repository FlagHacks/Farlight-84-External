
#pragma once

#ifndef BLOODHUNT_H

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include  <d3d9.h>
#include  <d3dx9.h>

#include "singleton.h"
#include "vector.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")


inline namespace BloodHunt
{
	class Variables : public Singleton<Variables>
	{
	public:
		const char* dwProcessName = "SolarlandClient-Win64-Shipping.exe";
		DWORD dwProcessId = NULL;
		uint64_t dwProcess_Base = NULL;
		HWND gameHWND = NULL;

		int actor_count = NULL;
		int ScreenHeight = NULL;
		int ScreenWidth = NULL;
		int ScreenLeft = NULL;
		int ScreenRight = NULL;
		int ScreenTop = NULL;
		int ScreenBottom = NULL;

		float ScreenCenterX = ScreenWidth / 2;
		float ScreenCenterY = ScreenHeight / 2;

		DWORD_PTR game_instance = NULL;
		DWORD_PTR u_world = NULL;
		DWORD_PTR local_player_pawn = NULL;
		DWORD_PTR local_player_array = NULL;
		DWORD_PTR local_player = NULL;
		DWORD_PTR local_player_root = NULL;
		DWORD_PTR local_player_controller = NULL;
		DWORD_PTR local_player_state = NULL;
		DWORD_PTR persistent_level = NULL;
		DWORD_PTR actors = NULL;
		DWORD_PTR ranged_weapon_component = NULL;
		DWORD_PTR equipped_weapon_type = NULL;


	};
#define GameVars BloodHunt::Variables::Get()

	class Offsets : public Singleton<Offsets>
	{
	public:

		DWORD offset_u_world = 0x62c8610;
		DWORD offset_g_names = 0x6158c40;
		DWORD offset_camera_manager = 0x2d8; // APlayerController->PlayerCameraManager
		DWORD offset_camera_cache = 0x1bf0; //APlayerCameraManager->CameraCachePrivate
		DWORD offset_persistent_level = 0x30; //UWorld->PersistentLevel
		DWORD offset_game_instance = 0x0220; //UWolrd->OwningGameInstance
		DWORD offset_local_players_array = 0x38; //UGameInstance->LocalPlayers
		DWORD offset_player_controller = 0x30; //UPlayer->PlayerController
		DWORD offset_apawn = 0x2c0;  //APlayerController->AcknowledgedPawn
		DWORD offset_root_component = 0x138; //AActor->RootComponent
		DWORD offset_actor_array = 0x98; //UNetConnection->OwningActor
		DWORD offset_actor_count = 0xa0; //UNetConnection->MaxPacket
		DWORD offset_actor_id = 0x18;
		DWORD offset_player_name = 0x310;
		DWORD offset_player_state = 0x248; //APawn->PlayerState
		DWORD offset_actor_mesh = 0x288; //ACharacter->Mesh
		DWORD offset_bone_array = 0x598;
		DWORD offset_component_to_world = 0x250;
		DWORD offset_ranged_weapon_component = 0x4c8;
		DWORD offset_equipped_weapon_type = 0x170;
		DWORD offset_relative_location = 0x11c; //USceneComponent->RelativeLocation
		DWORD offset_last_submit_time = 0x288;
		DWORD offset_last_render_time = 0x28C;
		DWORD offset_health = 0x578;
		DWORD offset_max_health = 0x57c;
		DWORD offset_BodyType = 0x7a4;
	};

#define GameOffset BloodHunt::Offsets::Get()
}
#endif  !BLOODHUNT_H

