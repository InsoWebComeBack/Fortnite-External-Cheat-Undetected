#pragma once 

void world() // world cache
{
	while (true)
	{
		offname::uworld = ReadPointer<DWORD_PTR>(Identifier::ADDRESS + Offsets::External::UWORLD);
		offname::gameinst = ReadPointer<DWORD_PTR>(offname::uworld + Offsets::External::GAMEINST);
		offname::localactors = ReadPointer<DWORD_PTR>(offname::gameinst + Offsets::External::LOCALACTOR);
		offname::localactor = ReadPointer<DWORD_PTR>(offname::localactors);
		offname::controlactor = ReadPointer<DWORD_PTR>(offname::localactor + Offsets::External::LEVEL);
		offname::localactorpawn = ReadPointer<DWORD_PTR>(offname::controlactor + Offsets::External::ACTORPAWN);
		offname::actorstate = ReadPointer<DWORD_PTR>(offname::localactorpawn + Offsets::External::ACTORSTATE);
		offname::rootcomp = ReadPointer<DWORD_PTR>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		if (offname::localactorpawn != 0) {
			offname::localactorid = ReadPointer<int>(offname::localactorpawn + Offsets::External::ITEMDEF);
		}
		offname::perslvl = ReadPointer<DWORD_PTR>(offname::uworld + Offsets::External::PERSLEVEL);
		offname::levels = ReadPointer<DWORD_PTR>(offname::uworld + Offsets::External::LEVELS);
		offname::actorcount = ReadPointer<DWORD>(offname::perslvl + Offsets::External::ACTORCOUNT);
		offname::aactors = ReadPointer<DWORD_PTR>(offname::perslvl + Offsets::External::AACTORS);
		Sleep(1);
	}
}

struct FZiplinePawnState {
	struct AFortAthenaZiplineBase* Zipline; // 0x00(0x08)
	struct AFortAthenaZiplineBase* PreviousZipline; // 0x00(0x08)
	struct UPrimitiveComponent* InteractComponent; // 0x10(0x08)
	bool bIsZiplining; // 0x18(0x01)
	bool bJumped; // 0x19(0x01)
	bool bReachedEnd; // 0x1a(0x01)
	char pad_1B[0x1]; // 0x1b(0x01)
	int32_t AuthoritativeValue; // 0x1c(0x04)struct sdk::structs::FVector SocketOffset; // 0x20(0x0c)
	float TimeZipliningBegan; // 0x2c(0x04)
	float TimeZipliningEndedFromJump; // 0x30(0x04)
	char pad_34[0x4]; // 0x34(0x04)
};

void getting_all_perslvl_actors()
{
	while (true)
	{
		// actor list using vector
		std::vector<actors> actorlist;

		for (int i = 0; i < offname::actorcount; i++)
		{
			uint64_t curactor = ReadPointer<uint64_t>(offname::aactors + i * 0x8);

			int playerid = ReadPointer<int>(curactor + Offsets::External::ITEMDEF);

			std::string FName = GetNameFromFName(playerid);

			std::cout << FName << std::endl;

			if (!curactor || curactor == offname::localactorpawn) continue;

			actors actorsgroup;

			/*if (curactor != 0 && curactor == offname::localactorid)
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.name = XorStr("player").c_str();
				actorlist.push_back(actorsgroup);
			}*/
			if (strstr(FName.c_str(), "PlayerPawn_Athena_C"))
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.name = XorStr("player").c_str();
				actorlist.push_back(actorsgroup);
			}
			else if (strstr(FName.c_str(), "BP_PlayerPawn_Athena_Phoebe_C"))
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.name = XorStr("bot").c_str();
				actorlist.push_back(actorsgroup);
			}
			/*else if (playerid == 21044659 || playerid == 25696452 || playerid == 25689405)
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.MESH = curactormesh;
				actorsgroup.name = XorStr("guard").c_str();
				actorlist.push_back(actorsgroup);
			}*/
			else if (strstr(FName.c_str(), "BP_PlayerPawn_Tandem_C"))
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.name = XorStr("NPC").c_str();
				actorlist.push_back(actorsgroup);
			}
			/*else if (playerid != 0 && features::debug)
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.MESH = curactormesh;
				actorsgroup.name = XorStr("debug").c_str();
				actorsgroup.ID = playerid;
				actorlist.push_back(actorsgroup);
			}*/
		}


		allplayers.clear();
		allplayers = actorlist; // get entity list for esp loop in render.h
		Sleep(1000);
	}
}

void getting_weakspots()
{
	while (true)
	{
		// actor list using vector
		actors actorsgroup;

		for (int i = 0; i < offname::actorcount; i++)
		{
			uint64_t curactor = ReadPointer<uint64_t>(offname::aactors + (i * sizeof(uintptr_t)));

			int playerid = ReadPointer<int>(curactor + Offsets::External::ITEMDEF);

			std::string FName = GetNameFromFName(playerid);

			if (!curactor || curactor == offname::localactorpawn) continue;
			
			if (strstr(FName.c_str(), "WeakSpot"))
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.name = XorStr("weakpoint").c_str();
			}

			/*else if (playerid != 0 && features::debug)
			{
				actorsgroup.ACTOR = curactor;
				actorsgroup.MESH = curactormesh;
				actorsgroup.name = XorStr("debug").c_str();
				actorsgroup.ID = playerid;
				actorlist.push_back(actorsgroup);
			}*/
		}

		weakspot.clear();
		weakspot.push_back(actorsgroup); // get entity list for esp loop in render.h
		Sleep(40);
	}
}

void getting_all_levels_actors()
{
	while (true)
	{
		std::vector<actors> levelslist;

		for (auto Itemlevel_i = 0UL; Itemlevel_i < ReadPointer<DWORD>(offname::uworld + (0x148 + sizeof(PVOID))); ++Itemlevel_i) {
			uintptr_t ItemLevels = ReadPointer<uintptr_t>(offname::uworld + Offsets::External::LEVELS);
			if (!ItemLevels) return;

			uintptr_t ItemLevel = ReadPointer<uintptr_t>(ItemLevels + (Itemlevel_i * sizeof(uintptr_t)));
			if (!ItemLevel) return;

			for (int i = 0; i < ReadPointer<DWORD>(ItemLevel + (Offsets::External::AACTORS + sizeof(PVOID))); ++i) {

				actors levelitemsgroup;

				uintptr_t ItemsPawns = ReadPointer<uintptr_t>(ItemLevel + Offsets::External::AACTORS);
				if (!ItemsPawns) return;

				uintptr_t CurrentItemPawn = ReadPointer<uintptr_t>(ItemsPawns + (i * sizeof(uintptr_t)));

				if (!CurrentItemPawn || CurrentItemPawn == offname::localactorpawn) continue;

				int zobi = ReadPointer<int>(CurrentItemPawn + Offsets::External::ITEMDEF);

				std::string itemname = GetNameFromFName(zobi);

				if (offname::localactorpawn && strstr(itemname.c_str(), XorStr("Tiered_Chest").c_str()) || strstr(itemname.c_str(), XorStr("Tiered_Ammo").c_str()))
				{

					if (strstr(itemname.c_str(), XorStr("Tiered_Chest").c_str()))
					{
						levelitemsgroup.ACTOR = CurrentItemPawn;
						//levelitemsgroup.MESH = ReadPointer<uint64_t>(CurrentItemPawn + Offsets::External::MESH);
						//levelitemsgroup.ID = zobi;
						levelitemsgroup.name = XorStr("chest").c_str();
						levelslist.push_back(levelitemsgroup);
					}
					else if (strstr(itemname.c_str(), XorStr("Tiered_Ammo").c_str()))
					{
						levelitemsgroup.ACTOR = CurrentItemPawn;
						//levelitemsgroup.MESH = ReadPointer<uint64_t>(CurrentItemPawn + Offsets::External::MESH);
						//levelitemsgroup.ID = zobi;
						levelitemsgroup.name = XorStr("ammo").c_str();
						levelslist.push_back(levelitemsgroup);
					}
					/*else if (features::debug)
					{
						levelitemsgroup.ACTOR = CurrentItemPawn;
						levelitemsgroup.MESH = ReadPointer<uint64_t>(CurrentItemPawn + Offsets::External::MESH);
						levelitemsgroup.ID = zobi;
						levelitemsgroup.name = itemname;
						levelslist.push_back(levelitemsgroup);
					}*/
				}
				/*Sleep(1);*/
			}
		}
		levelloot.clear();
		levelloot = levelslist;
		Sleep(1000);
	}
}

void getting_world_loot()
{
	while (1)
	{
		std::vector<actors> worldsloot;

		for (auto item = 0UL; item < ReadPointer<DWORD>(offname::uworld + (Offsets::External::LEVELS + sizeof(PVOID))); ++item)
		{
			uintptr_t levelsitem = ReadPointer<uintptr_t>(offname::uworld + Offsets::External::LEVELS);
			if (!levelsitem) return;

			uintptr_t iteminlevels = ReadPointer<uintptr_t>(levelsitem + (item * sizeof(uintptr_t)));
			if (!iteminlevels) return;

			for (int i = 0; i < ReadPointer<DWORD>(iteminlevels + (Offsets::External::AACTORS + sizeof(PVOID))); ++i)
			{
				actors lootstruct;

				uintptr_t currentitem = ReadPointer<uintptr_t>(iteminlevels + Offsets::External::AACTORS);
				if (!currentitem) return;

				uintptr_t currentitempawn = ReadPointer<uintptr_t>(currentitem + (i * sizeof(uintptr_t)));

				if (!currentitempawn || currentitempawn == offname::localactorpawn) continue;

				int zobi = ReadPointer<int>(currentitempawn + Offsets::External::ITEMDEF);

				std::string itemname = GetNameFromFName(zobi);

				if (strstr(itemname.c_str(), "FortPickupAthena"))
				{
					uintptr_t currentweapon = ReadPointer<uintptr_t>(currentitempawn + 0x308 + 0x18);
					int currentweaponid = ReadPointer<int>(currentweapon + 0x18);
					std::string weaponname = GetNameFromFName(currentweaponid);

					lootstruct.ACTOR = currentitempawn;
					lootstruct.name = weaponname;
					worldsloot.push_back(lootstruct);
				}
			}
		}
		itemsloot.clear();
		itemsloot = worldsloot;

		Sleep(1000);
	}
}

void exploits()
{
	while (true)
	{
		if (offname::localactorpawn)
		{
			if (features::instarevive)
			{
				WritePointer<float>(offname::localactorpawn + 0x3c78, 0.01);
			}

			if (features::ads)
			{
				WritePointer<bool>(offname::localactorpawn + 0x4525, true);
			}		

			/*if (features::pfly)
			{
				WritePointer<float>(offname::localactorpawn + 0x1CE4, 1000.f);
				WritePointer<bool>(offname::localactorpawn + 0x1C00 + 0x18, true);
			}*/
		}
		Sleep(40);
	}
}