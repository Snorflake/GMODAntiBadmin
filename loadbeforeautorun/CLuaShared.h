#pragma once
#include <string>
#include <vector>
//the fuck is this shit
// Auto reconstructed from vtable block @ 0x0005F9C0
// from "lua_shared.dylib", by VTableRec.idc
namespace VT { class CLuaShared; };

class VT::CLuaShared
{
public:                                                                                                 //  Index Offset
	//void Func0001(void);                                                          // 0001  0004
	//virtual void *CLuaShared::~CLuaShared();                                                              // 0002  0008
	//      virtual void *CLuaShared::~CLuaShared();        
	virtual void            padding00() = 0; // test                                                        // 0003  000C
	virtual void *CLuaShared::Init(void * (*)(char  const*, int *), bool, void *, void *);     // 1                                                          // 0004  0010
	virtual void *CLuaShared::Shutdown(void); // 2                                                               // 0005  0014
	virtual void *CLuaShared::DumpStats(void);  //        4                                                 // 0006  0018
	virtual void *CLuaShared::CreateLuaInterface(unsigned char, bool);      // 4                                                          // 0007  001C
	virtual void *CLuaShared::CloseLuaInterface(void *);            // 0008  0020
	virtual void *CLuaShared::GetLuaInterface(unsigned char);                                                               // 0009  0024
	virtual void *CLuaShared::LoadFile(std::string  const&, std::string  const&, bool, bool);                                                          // 0010  0028
	virtual void *CLuaShared::GetCache(std::string  const&);                                                                // 0011  002C
	virtual void *CLuaShared::MountLua(char  const*);                                                               // 0012  0030
	virtual void *CLuaShared::MountLuaAdd(char  const*, char  const*);                                                               // 0013  0034
	virtual void *CLuaShared::UnMountLua(char  const*);                                                             // 0014  0038
	virtual void *CLuaShared::SetFileContents(char  const*, char  const*);                                                           // 0015  003C
	virtual void *CLuaShared::SetLuaFindHook(void *);                                                               // 0016  0040
	virtual void *CLuaShared::FindScripts(std::string  const&, std::string  const&, std::vector<std::string, std::allocator<std::string>> &);                                                          // 0017  0044
};