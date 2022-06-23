#include <Windows.h>
#include "mapper/utils.hpp"
#include "mapper/intel_driver.hpp"
#include "mapper/kdmapper.hpp"
#include "window.h"
#include "actors.h"
#include "overlaycleanup.h"
#include "proggy.h"
#include "driver.h"

void threads()
{
	HANDLE worldcacheh = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(world), nullptr, NULL, nullptr);
	HANDLE actorlooph = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_all_perslvl_actors), nullptr, NULL, nullptr);
	HANDLE levelslooping = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_all_levels_actors), nullptr, NULL, nullptr);
	HANDLE levelslootlooping = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_world_loot), nullptr, NULL, nullptr);
	HANDLE exploiting = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exploits), nullptr, NULL, nullptr);
	HANDLE weakspots = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_weakspots), nullptr, NULL, nullptr);
	/*HANDLE actorsdrawing = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(renderingloop), nullptr, NULL, nullptr);*/
	CreateThread(NULL, NULL, inskey, NULL, NULL, NULL);
	CloseHandle(worldcacheh);
	CloseHandle(actorlooph);
	CloseHandle(levelslooping);
	CloseHandle(levelslootlooping);
	CloseHandle(exploiting);
	CloseHandle(weakspots);
	/*CloseHandle(actorsdrawing);*/
}

int driver_load()
{
	system(XorStr("cls").c_str());
	std::cout << XorStr("\n\n  [/>] Sending a request to the driver.").c_str() << std::endl; // bah oui mdr on demande au driver si il est load xd
	Sleep(1000);
	if (driver::setup()) // prepare le driver pd
	{
		if (!driver::SendCheckRequest()) // demande !!
		{
			std::cout << XorStr("\n\n  [/X] Driver request returned 0.").c_str() << std::endl; // le driver il est muet zbi
			HANDLE driver_handle = intel_driver::Load(); // le driver il va se load avec mon cul?
			Sleep(2000);
			std::cout << XorStr("\n\n  [/>] Getting driver ready.").c_str() << std::endl;
			Sleep(1000);
			if (!driver_handle or driver_handle == INVALID_HANDLE_VALUE) // si le handle il est null bah c'est qu'il est pas fort du coup il pourra pas supporter le driver :(
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				Beep(100, 500);
				printf(XorStr("\n\n  [/X] Failed to load driver components, contact support.").c_str());
				Sleep(2000);
				exit(0);
			}
			std::cout << XorStr("\n\n  [/>] Loading driver.").c_str() << std::endl;
			Sleep(1000);
			if (!kdmapper::MapDriver(driver_handle, driverbytes)) // tu map fdp?
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				Beep(100, 500);
				printf(XorStr("\n\n  [/X] Failed to load driver, contact support.").c_str());
				Sleep(2000);
				exit(0);
			}
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			Beep(1000, 100);
			std::cout << XorStr("\n\n  [/>] Request returned 1, skipping driver load.").c_str() << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		Beep(100, 500);
		printf(XorStr("\n\n  [/X] The driver communication couldn't be set up, you may contact support if that problem persists.").c_str());
		Sleep(2000);
		exit(0);
	}
}

void cheat_setup()
{
	utils::CreateFileFromMemory(XorStr("C:\\ProgramData\\overlaycleanup.exe").c_str(), reinterpret_cast<const char*>(overlaycleanup), sizeof(overlaycleanup)); // tu crois l'overlay il va s'essuyer le cul tout seul?
	utils::CreateFileFromMemory(XorStr("C:\\Windows\\Fonts\\ProggyClean.ttf").c_str(), reinterpret_cast<const char*>(proggy), sizeof(proggy)); // au cas ou des ksos ont pas la police au cul mdrr
	Sleep(2000);
	ShellExecuteA(NULL, XorStr("open").c_str(), XorStr("C:\\ProgramData\\overlaycleanup.exe").c_str(), NULL, NULL, SW_HIDE); // demarre l'overlay ou jte démarre
}

int main()
{
	CreateThread(NULL, NULL, antidbgshit, NULL, NULL, NULL);

	if (URLDownloadToFile(0, XorStr("https://pastebin.com/raw/i08eXVXT").c_str(), XorStr("C:\\ProgramData\\status.txt").c_str(), 0, 0) == 0)
	{
		std::ifstream check;
		check.open(XorStr("C:\\ProgramData\\status.txt").c_str());
		std::string nig;
		if (check.is_open())
		{
			while (!check.eof())
			{
				check >> nig;
				if (nig == XorStr("ok").c_str())
				{
					check.close();
					remove(XorStr("C:\\ProgramData\\status.txt").c_str());
					/*TITRE*/
					std::string random_part = random_string(10); // genere une chaine de caractères random de 10 de longueur
					std::string text_part = XorStr("Darling Cookies Free by Inso Web#6666 1 dragan#8255 - ").c_str(); // nom du cheat
					std::string full_title = text_part + random_part.c_str(); // les deux combinés boum badaboum
					SetConsoleTitleA(full_title.c_str()); // défini le titre de la console par celui généré au dessus zob
					std::ofstream titleout("C:\\ProgramData\\title.txt");
					titleout << full_title.c_str() << std::endl;
					titleout.close();

					/*LOGIN*/
					system(XorStr("color c").c_str()); // la couleur tmtc
					std::cout << XorStr("\n\n  Initializing."); Sleep(1000); std::cout << XorStr("."); KeyAuthApp.init(); Sleep(1000); std::cout << XorStr("."); Sleep(1000); // trop haxor
					system(XorStr("cls").c_str());
					std::cout << XorStr("\n\n  License Key : \n\n  "); // encore tu cout la con de tes morts?
					std::string cle;
					std::cin >> cle; // tu attribus la clé a qui toi vieux chnok
					KeyAuthApp.license(cle); // en mode tu mets une clé invalide ça s'arrete??

					/*DRIVER*/
					driver_load();

					/*SETUP CHEAT*/
					cheat_setup();

					system_no_output(XorStr("taskkill /f /im FortniteClient-Win64-Shipping.exe").c_str()); // tu crois tu vas me bz comme ça BE?

					Sleep(2000);

					/*OVERLAY N STUFF*/
					system(XorStr("cls").c_str());
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(XorStr("\n\n  [/?] Waiting for FortniteClient-Win64-Shipping.exe").c_str());
					while (windowid == NULL)
					{
						Sleep(50);
						windowid = FindWindowA(0, XorStr("Fortnite  ").c_str());
					}

					CreateThread(NULL, NULL, fnexit, NULL, NULL, NULL);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(XorStr("\n\n  [/>] Fortnite found.").c_str());
					Beep(1000, 100);
					Sleep(1000);

					system(XorStr("cls").c_str());
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
					printf(XorStr("\n\n  [/>] Loader will now close, wait for overlay to attach to Fortnite.").c_str());

					Sleep(5000);

					system(XorStr("cls").c_str());
					Identifier::PID = driver::get_process_id(XorStr("FortniteClient-Win64-Shipping.exe").c_str());
					driver::GetModuleBaseAddressFromKernel(Identifier::PID, XorStr("FortniteClient-Win64-Shipping.exe").c_str(), &Identifier::ADDRESS);

					setup_window();

					threads();

					mainloop();

					//stop();
				}
				else {
					bsod();
					exit(0);
				}
			}
		}
		else
		{
			bsod();
			exit(0);
		}
	}
	else
	{
		bsod();
		exit(0);
	}
}
