#include <Windows.h>
#include <fstream>
#include <string>
#include "CLuaInterface.h"
#include "CLuaShared.h"
#include "OriginalLua.h"
#include "vmt.h"
#include <regex>
VT::CLuaShared* LuaShared;
typedef void* (*CreateInterfaceFn)(const char *Name, int *ReturnCode);

VT::CLuaInterface *interfaces[3] = {nullptr, nullptr, nullptr};
void* GetPointer(const char* Module, const char* InterfaceName)
{
	void* Interface = NULL;
	char PossibleInterfaceName[1024];

	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(Module), "CreateInterface");
	for (int i = 1; i < 100; i++)
	{
		sprintf(PossibleInterfaceName, "%s0%i", InterfaceName, i);
		Interface = (void*)CreateInterface(PossibleInterfaceName, NULL);
		if (Interface != NULL)
		{
			printf("%s Found: 0x%X\n", PossibleInterfaceName, (DWORD)Interface);
			break;
		}
		sprintf(PossibleInterfaceName, "%s00%i", InterfaceName, i);
		Interface = (void*)CreateInterface(PossibleInterfaceName, NULL);
		if (Interface != NULL)
		{
			printf("%s Found: 0x%X\n", PossibleInterfaceName, (DWORD)Interface);
			break;
		}
	}

	return Interface;
}
typedef void*(__thiscall*CreateLuaInterfaceFn)(void*,unsigned char, bool);
typedef void*(__thiscall*RunStringFn)(void*, const char *, const char *, const char *, bool, bool);
typedef void *(__thiscall *hCloseLuaInterfaceFn)(void*, VT::CLuaInterface*);
hCloseLuaInterfaceFn oCloseLuaInterface;
RunStringFn oRunString;
luaL_dostring dostring;
CreateLuaInterfaceFn oCreateLuaInterface;
VT::CLuaInterface* luaInterface;

void menu_RunString(std::string code)
{
	HMODULE module = GetModuleHandle("lua_shared.dll");
	luaL_loadstring = (luaL_loadstringFn)GetProcAddress(module, "luaL_loadstring");
	lua_pcall = (lua_pcallFn)GetProcAddress(module, "lua_pcall");
	dostring = (luaL_dostring)GetProcAddress(module, "luaL_dostring");
	VT::CLuaInterface* menustate = (VT::CLuaInterface*)LuaShared->GetLuaInterface(2);
	interfaces[2] = menustate;
	void* lua_menustate = menustate->GetLuaState();
	if (lua_menustate)
	{
		luaL_loadstring(lua_menustate, code.c_str());
		lua_pcall(lua_menustate, 0, -1, 0);
	}
	 else
	 {
		 printf("ERROR: 0x1 - Unable to get Menu State, is the game fully loaded?\n");
	 }
}


std::string ReplaceString(std::string subject, const std::string& search,
						  const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		 subject.replace(pos, search.length(), replace);
		 pos += replace.length();
	}
	return subject;
}
//int num = 0;
bool runonce = false;
bool __fastcall hkRunString(VT::CLuaInterface* _this, void*, const char * filename, const char * a2, const char * contents, bool a4, bool a5)
{
	/*
	CreateDirectory("lua_dump",0);
	CreateDirectory("lua_dump/client",0);

	std::string dumpname = "lua_dump/client/";
	dumpname += ReplaceString(filename,"/","."); //replaces "/" with "."
	//filename += ".lua";
	//no need for that

	std::ofstream fs(dumpname);
	fs << contents;
	fs.close();*/
	std::regex crash1(".*do +end.*", std::regex_constants::icase);
	std::string cumtents = std::string(contents);
	if (std::regex_match(cumtents,crash1)) {
		oRunString(_this, "meme.lau", "", "print('do end blocked!')", a4, a5);
		printf("Crash code: %s\n", contents);
		return false;
	}
	if (!runonce) {
		runonce = true;
		oRunString(_this, "meme.lau", "", " local o_Start3D,o_End3D = cam.Start3D,cam.End3D\
			local counter = 0\
			function cam.Start3D(...) counter = counter + 1 return o_Start3D(...) end\
			function cam.End3D(...) counter = counter - 1 if counter < 0 then counter = counter + 1 return print('cam.end3d crash stopped!(buggy)') end return o_End3D(...) end\
			", a4, a5);
	}
	return oRunString(_this,filename,a2,contents,a4,a5);

}
void *__fastcall hkCreateLuaInterface(void* _this, void*, unsigned char state, VT::CLuaInterface* iface)
{
	VT::CLuaInterface* L = (VT::CLuaInterface*)oCreateLuaInterface(_this,state,iface);
	interfaces[state] = L;

	if(state == 0){
		CVMTHookManager* runstringhook = new CVMTHookManager(L);
		oRunString = (RunStringFn)runstringhook->HookFunction(88, hkRunString);
		runstringhook->HookTable(true);
	}
	printf("%i interface created @ 0x%p\n", state, L);

	return L;
}
void* __fastcall hCloseLuaInterface(void* _this, void*, VT::CLuaInterface* L)
{
	unsigned char state;
	for(state = 0; state < 3; state++)
	{
		if(interfaces[state] == L)
		{
			interfaces[state] = nullptr;
			runonce = false;
			break;
		}
	}

	printf("%i interface destroyed @ 0x%p\n", state, L);

	return oCloseLuaInterface(_this,L);
}
#define CTHRD(x) CreateThread(0,0,(LPTHREAD_START_ROUTINE)x,0,0,0);
void StartCheat()
{
	LuaShared = (VT::CLuaShared*)GetPointer("lua_shared.dll", "LUASHARED");
	if (!LuaShared) return;
	CVMTHookManager* sharedhook = new CVMTHookManager(LuaShared);
	oCreateLuaInterface = (CreateLuaInterfaceFn)sharedhook->HookFunction(4, hkCreateLuaInterface);
	oCloseLuaInterface=  (hCloseLuaInterfaceFn)sharedhook->HookFunction(5, hCloseLuaInterface);
	sharedhook->HookTable(true);
	menu_RunString("print('Load before autorun successful!')");
	LoadLibrary("urlblocker.dll");
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
			if (!interfaces[0]) { continue; }
			void* lua_gamestate = interfaces[0];
			if (lua_gamestate)
			{
				interfaces[0]->RunString("meme.lua", "", "print('test')", true, true);
			}
			else
			{
				printf("ERROR: 0x1 - Unable to get Game State, are you ingame?\n");
			}
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason_for_call, LPVOID reserved)
{
	if (reason_for_call == DLL_PROCESS_ATTACH) 
	{
		DisableThreadLibraryCalls(module_handle); 
		if (reserved == NULL) 
		{
			AllocConsole();

			HWND hwnd = GetConsoleWindow();

			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

			SetConsoleTitle("[Clientside dumper]: Console");
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			CTHRD(StartCheat)			
		}
		else // Static load
		{
			return FALSE;
		}
	}
	return TRUE;
}