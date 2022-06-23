#pragma once

#include "world.h"

bool IsVisible(uintptr_t TargetPawn)
{
	auto targetMesh = ReadPointer<uint64_t>(TargetPawn + 0x318);
	auto localMesh = ReadPointer<DWORD_PTR>(offname::localactorpawn + 0x318);

	if (targetMesh && localMesh)
	{
		float targetLastRenderTimeOnScreen = ReadPointer<float>(targetMesh + 0x318);
		float localLastRenderTimeOnScreen = ReadPointer<float>(localMesh + 0x318);

		if (localLastRenderTimeOnScreen == targetLastRenderTimeOnScreen)
			return true;
	}
	return false;
}

void draw_esp()
{
	float closestDistance = FLT_MAX; //gets max float value
	DWORD_PTR closestPawn = NULL; //defines closestpawn
	float closestactor = FLT_MAX;
	for (unsigned long i = 0; i < allplayers.size(); ++i)
	{
		actors entity = allplayers[i];
		uintptr_t entitymesh = ReadPointer<uintptr_t>(entity.ACTOR + Offsets::External::MESH); // need
		uint64_t rootcomp = ReadPointer<uint64_t>(entity.ACTOR + Offsets::External::ROOTCOMPONENT);
		Vector3 actorpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 actorposW2s = w2s(actorpos);

		//if (features::bigplayers)
		//{
		//	WritePointer<Vector3>(entitymesh + 0x134, Vector3(5, 5, 5)); // 0x134 -> big players
		//}
		//else
		//{
		//	WritePointer<Vector3>(entitymesh + 0x134, Vector3(1, 1, 1));
		//}

		DWORD64 otherPlayerState = ReadPointer<uint64_t>(entity.ACTOR + Offsets::External::ACTORSTATE);
		Vector3 Headpos = bonerota(entitymesh, 98); // need
		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(actorpos) / 100.f;

		Vector3 bone0 = bonerota(entitymesh, 0);
		Vector3 bonepp = bonerota(entitymesh, 2);
		Vector3 bottom = w2s(bone0);
		Vector3 pplol = w2s(bonepp);
		Vector3 Headbox = w2s(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		Vector3 w2shead = w2s(Headpos); // need

		float BoxHeight = (float)(Headbox.y - bottom.y);
		float BoxWidth = BoxHeight * 0.250;

		float LeftX = (float)Headbox.x - (BoxWidth / 1);
		float LeftY = (float)bottom.y;

		float CornerHeight = abs(Headbox.y - bottom.y);
		float CornerWidth = CornerHeight * features::boxwidthlol;

		int MyTeamId = ReadPointer<int>(offname::actorstate + Offsets::External::TEAMID);
		int ActorTeamId = ReadPointer<int>(otherPlayerState + Offsets::External::TEAMID);

		auto visiblecheckbyte = ReadPointer<int>(entitymesh + 0x707);
		//std::cout << visiblecheckbyte << std::endl;

		bool bSpotted = false;
		if (visiblecheckbyte == 2703442)
		{
			bSpotted = true;
		}

		/*else if (!IsVisible(entity.ACTOR))
		{
			bSpotted = false;
		}*/

		/*std::cout << visiblecheckbyte << std::endl;*/


		if (MyTeamId != ActorTeamId && offname::localactorpawn && distance < 250)
		{
			if (features::boxes)
			{
				if (entity.name == XorStr("NPC").c_str())
				{
					ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(50, 200, 0, 255), 0, 15, 0.2f);
				}
				else if (entity.name == XorStr("player").c_str() || entity.name == XorStr("bot").c_str() || entity.name == XorStr("guard").c_str())
				{
					if (bSpotted)
					{
						ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(255, 255, 255, 255), 0, 15, 0.2f);
					}
					else
					{
						ImGui::GetBackgroundDrawList()->AddRect(ImVec2(LeftX, LeftY), ImVec2(Headbox.x + BoxWidth, Headbox.y), IM_COL32(210, 190, 0, 255), 0, 15, 0.2f);
					}
				}
			}

			char dist[64];
			std::string nichon;
			ImU32 color;
			if (entity.name == XorStr("player").c_str())
			{
				color = IM_COL32(255, 0, 0, 255);
				nichon = XorStr("Joueur [%.fm]");
			}
			else if (entity.name == XorStr("bot").c_str())
			{
				color = IM_COL32(255, 0, 0, 255);
				nichon = XorStr("IA [%.fm]");
			}
			else if (entity.name == XorStr("NPC").c_str())
			{
				color = IM_COL32(153, 226, 180, 255);
				nichon = XorStr("NPC [%.fm]");
			}
			else if (entity.name == XorStr("guard").c_str())
			{
				color = IM_COL32(255, 189, 0, 255);
				nichon = XorStr("IA [%.fm]");
			}
			/*else if (entity.name == XorStr("debug").c_str() && features::debug)
			{
				nichon = std::to_string(entity.ID);
			}*/

			if (features::distance)
			{
				sprintf_s(dist, nichon.c_str(), distance);
				outlinedtext(l_pFont, dist, ImVec2(bottom.x, bottom.y), 13.0f, color, true);
			}

			//auto weapondname = ReadPointer<FText*>(weapondata + 0x88);

			//std::string porut = GetNameFromFName(weaponname);

			/*auto ididi = reinterpret_cast<FText*>(ReadPointer<uintptr_t>(weaponname + 0x88));*/

			/*if (features::ammoesp) {

				char tet[256];
				sprintf(tet, (XorStr("Ammo: %d").c_str()), (int)ammocount);
				outlinedtext(l_pFont, tet, ImVec2(Text.x, Text.y - 35), 13.0f, IM_COL32(255, 255, 255, 255), true);

			}

			if (features::weaponesp) {

				char teSt2[256];
				sprintf(teSt2, (XorStr("%s").c_str()), ididi);
				outlinedtext(l_pFont, ididi->c_str(), ImVec2(Text.x, Text.y - 20), 13.0f, IM_COL32(255, 255, 255, 255), true);

			}

			if (features::reloadingesp) {

				if (isreloading)
				{
					char buf[256];
					sprintf(buf, (XorStr("Reloading...").c_str()), (int)distance);
					outlinedtext(l_pFont, buf, ImVec2(Text.x + 4, Text.y - 50), 13.0f, IM_COL32(255, 0, 0, 255), true);
				}

			}*/

			/*if (features::chest && players.find("Tiered_Chest") != std::string::npos)
			{
				char loli[64];
				sprintf_s(loli, " Chest | %.fm", distance);
				outlinedtext(l_pFont, loli, ImVec2(actorposW2s.x, actorposW2s.y + 10), 13.0f, IM_COL32(255, 255, 255, 255), true);
			}*/

			//if (features::skeleton)
			//{
			//	Vector3 head = w2s(bonerota(entity.MESH, 98));
			//	Vector3 neck = w2s(bonerota(entity.MESH, 66));
			//	Vector3 pelvis = w2s(bonerota(entity.MESH, 2));
			//	Vector3 chesti = w2s(bonerota(entity.MESH, 37));
			//	Vector3 chestatright = w2s(bonerota(entity.MESH, 8));
			//	Vector3 leftShoulder = w2s(bonerota(entity.MESH, 93));
			//	Vector3 rightShoulder = w2s(bonerota(entity.MESH, 9));
			//	Vector3 leftElbow = w2s(bonerota(entity.MESH, 94));
			//	Vector3 rightElbow = w2s(bonerota(entity.MESH, 10));
			//	Vector3 leftHand = w2s(bonerota(entity.MESH, 100));
			//	Vector3 rightHand = w2s(bonerota(entity.MESH, 99));
			//	Vector3 leftLeg = w2s(bonerota(entity.MESH, 76));
			//	Vector3 rightLeg = w2s(bonerota(entity.MESH, 69));
			//	Vector3 leftThigh = w2s(bonerota(entity.MESH, 82));
			//	Vector3 rightThigh = w2s(bonerota(entity.MESH, 75));
			//	Vector3 leftFoot = w2s(bonerota(entity.MESH, 78));
			//	Vector3 rightFoot = w2s(bonerota(entity.MESH, 71));
			//	Vector3 leftFeet = w2s(bonerota(entity.MESH, 85));
			//	Vector3 rightFeet = w2s(bonerota(entity.MESH, 84));
			//	Vector3 leftFeetFinger = w2s(bonerota(entity.MESH, 81));
			//	Vector3 rightFeetFinger = w2s(bonerota(entity.MESH, 74));

			//	//head->neck


			//	lineesp(head.x, head.y, neck.x, neck.y, &features::skeletoncolor, 0.2f);

			//	//neck->chest
			//	lineesp(neck.x, neck.y, chesti.x, chesti.y, &features::skeletoncolor, 0.2f);

			//	//chest bone
			//	lineesp(chesti.x, chesti.y, leftShoulder.x, leftShoulder.y, &features::skeletoncolor, 0.2f);
			//	lineesp(chesti.x, chesti.y, rightShoulder.x, rightShoulder.y, &features::skeletoncolor, 0.2f);
			//	lineesp(chesti.x, chesti.y, pelvis.x, pelvis.y, &features::skeletoncolor, 0.2f);

			//	//leftarm
			//	lineesp(leftShoulder.x, leftShoulder.y, leftElbow.x, leftElbow.y, &features::skeletoncolor, 0.2f);
			//	lineesp(leftElbow.x, leftElbow.y, leftHand.x, leftHand.y, &features::skeletoncolor, 0.2f);

			//	//rightarm
			//	lineesp(rightShoulder.x, rightShoulder.y, rightElbow.x, rightElbow.y, &features::skeletoncolor, 0.2f);
			//	lineesp(rightElbow.x, rightElbow.y, rightHand.x, rightHand.y, &features::skeletoncolor, 0.2f);

			//	//pelvis->hips
			//	lineesp(pelvis.x, pelvis.y, leftLeg.x, leftLeg.y, &features::skeletoncolor, 0.2f);
			//	lineesp(pelvis.x, pelvis.y, rightLeg.x, rightLeg.y, &features::skeletoncolor, 0.2f);

			//	//right leg
			//	lineesp(rightLeg.x, rightLeg.y, rightThigh.x, rightThigh.y, &features::skeletoncolor, 0.2f);
			//	lineesp(rightThigh.x, rightThigh.y, rightFoot.x, rightFoot.y, &features::skeletoncolor, 0.2f);
			//	lineesp(rightFoot.x, rightFoot.y, rightFeet.x, rightFeet.y, &features::skeletoncolor, 0.2f);
			//	lineesp(rightFeet.x, rightFeet.y, rightFeetFinger.x, rightFeetFinger.y, &features::skeletoncolor, 0.2f);

			//	//left leg
			//	lineesp(leftLeg.x, leftLeg.y, leftThigh.x, leftThigh.y, &features::skeletoncolor, 0.2f);
			//	lineesp(leftThigh.x, leftThigh.y, leftFoot.x, leftFoot.y, &features::skeletoncolor, 0.2f);
			//	lineesp(leftFoot.x, leftFoot.y, leftFeet.x, leftFeet.y, &features::skeletoncolor, 0.2f);
			//	lineesp(leftFeet.x, leftFeet.y, leftFeetFinger.x, leftFeetFinger.y, &features::skeletoncolor, 0.2f);
			//}

			if (features::lines)
			{
				if (features::lineesppos == 1)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, pplol.x, pplol.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, pplol.x, pplol.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
				else if (features::lineesppos == 2)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, bottom.x, bottom.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, bottom.x, bottom.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
				else if (features::lineesppos == 0)
				{
					if (features::allan)
					{
						lineesp(Width / 2, Height / 2, Headbox.x, Headbox.y, IM_COL32(255, 0, 228, 255), 0.2f);
					}
					else
					{
						lineesp(Width / 2, Height, Headbox.x, Headbox.y, IM_COL32(255, 255, 255, 255), 0.2f);
					}
				}
			}

		}
		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);
		if (dist < features::fov && dist < closestDistance/*&& (!ReadPointer<bool>(entity.ACTOR + 0x572))*/) {
			uintptr_t thisone;
			if (closestactor > distance)
			{
				closestactor = distance;
				thisone = entity.ACTOR;
			}
			closestDistance = dist;
			if (MyTeamId != ActorTeamId && entity.ACTOR == thisone)
			{
				closestPawn = entity.ACTOR;
			}
		}
	}
	for (unsigned long i = 0; i < weakspot.size(); ++i)
	{
		actors weakpointlol = weakspot[i];

		uintptr_t entitymesh = ReadPointer<uintptr_t>(weakpointlol.ACTOR + Offsets::External::MESH);
		Vector3 Headpos = bonerota(entitymesh, 0);
		Vector3 w2shead = w2s(Headpos);

		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);
		if (dist < features::fov && dist < closestDistance/*&& (!ReadPointer<bool>(entity.ACTOR + 0x572))*/) {
			closestDistance = dist;
			if (weakpointlol.name == XorStr("weakpoint").c_str())
			{
				closestPawn = weakpointlol.ACTOR;
			}
		}
	}

	if (features::aimbot)
	{
		if (features::aimbot && closestPawn && GetAsyncKeyState(features::aimkey) < 0) {
			targetbone(closestPawn);
		}
	}

	//if (features::noweapswapdelay)
	//{
	//	uintptr_t LocalActorWeapon = ReadPointer<uintptr_t>(offname::localactorpawn + 0x5f8);

	//	if (LocalActorWeapon)
	//	{
	//		WritePointer<bool>(LocalActorWeapon + 0x2B3, true);
	//	}
	//}
	
}

void nikezbiloop()
{
	for (unsigned long i = 0; i < levelloot.size(); ++i)
	{
		actors elements = levelloot[i];

		/*int elementid = ReadPointer<int>(elements.ACTOR + 0x18);*/

		//std::string elementname = GetNameFromFName(elementid);

		uint64_t rootcomp = ReadPointer<uint64_t>(elements.ACTOR + Offsets::External::ROOTCOMPONENT);
		Vector3 elementpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 elementposW2s = w2s(elementpos);

		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(elementpos) / 100.f;

		auto penislmao = ReadPointer<bool>(elements.ACTOR + 0xf49);

		//std::cout << penislmao << std::endl;
		/*char test[64];
		std::string actorid = std::to_string(elements.ID);
		outlinedtext(l_pFont, actorid.c_str(), ImVec2(elementposW2s.x, elementposW2s.y), 13.0f, IM_COL32(255, 255, 255, 255), true);*/
		if (features::chest && elements.name == XorStr("chest").c_str() && distance <= 60 && penislmao == 9)
		{
			char chest[64];
			sprintf(chest, XorStr(" Chest \n[%.fm]").c_str(), distance);
			outlinedtext(l_pFont, chest, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(255, 255, 0, 255), true);
		}
		else if (features::ammo && elements.name == XorStr("ammo").c_str() && distance <= 60 && penislmao == 9)
		{
			char ammobox[64];
			sprintf(ammobox, XorStr(" Ammo Box \n[%.fm]").c_str(), distance);
			outlinedtext(l_pFont, ammobox, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(0, 255, 0, 255), true);
		}
		/*Sleep(1);*/
	}
}

void lootesploop()
{
	for (unsigned long i = 0; i < itemsloot.size(); ++i)
	{
		actors loot = itemsloot[i];

		uint64_t rootcomp = ReadPointer<uint64_t>(loot.ACTOR + 0x188);
		Vector3 elementpos = ReadPointer<Vector3>(rootcomp + Offsets::External::RELATIVELOC);
		Vector3 elementposW2s = w2s(elementpos);

		uintptr_t localroot = ReadPointer<uintptr_t>(offname::localactorpawn + Offsets::External::ROOTCOMPONENT);
		localactorpos = ReadPointer<Vector3>(localroot + Offsets::External::RELATIVELOC);

		float distance = localactorpos.Distance(elementpos) / 100.f;

		std::string weaponcustomname = loot.name;

		//std::cout << weaponcustomname << std::endl;

		if (strstr(loot.name.c_str(), "Assault"))
		{
			weaponcustomname = "Assault Rifle [%.fm]";
		}
		else if (strstr(loot.name.c_str(), "Shotgun"))
		{
			weaponcustomname = "Pump Shotgun [%.fm]";
		}
		else if (strstr(loot.name.c_str(), "Shield"))
		{
			weaponcustomname = "Shields [%.fm]";
		}

		if (features::debug)
		{
			char ammobox[64];
			sprintf(ammobox, weaponcustomname.c_str(), distance);
			outlinedtext(l_pFont, ammobox, ImVec2(elementposW2s.x, elementposW2s.y), 11.0f, IM_COL32(0, 255, 0, 255), true);
		}
	}
}