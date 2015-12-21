// many, many typedefs 
#pragma once

typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);

// MS_FINDER_ADD_TYPEDEF
typedef void *(__cdecl *lua_newthreadFn)(void *L);
typedef int(__cdecl *lua_typeFn)(void *L, int index);
typedef void(__cdecl *lua_settopFn)(void *L, int index);
typedef int(__cdecl *lua_isnumberFn)(void *L, int index);
typedef void(__cdecl *lua_pushbooleanFn)(void *L, int b);
typedef const char *(__cdecl *lua_tolstringFn)(void *L, int index, size_t *len);
typedef void(__cdecl *lua_getfieldFn)(void *L, int index, const char *k);
typedef void(__cdecl *lua_setfieldFn)(void *L, int idx, const char *k);
typedef int(__cdecl *luaL_loadfileFn)(void *L, const char *filename);
typedef int(__cdecl *luaL_loadbufferFn)(void *L, const char *buff, size_t sz, const char *name);
typedef int(__cdecl *luaL_loadstringFn)(void *L, const char *s);
typedef int(__cdecl *luaL_dostring)(void *L, const char *s);
typedef void *(__cdecl *luaL_newstateFn)(void);
typedef int(__cdecl *lua_pcallFn)(void *L, int nargs, int nresults, int errfunc);
typedef void(__cdecl *lua_closeFn)(void *L);
typedef void(__cdecl *luaL_openlibsFn)(void *L);
typedef void(__cdecl *lua_atpanicFn)(void *L, void *CFunc);
typedef void *(__cdecl *luaL_checkudataFn)(void *L, int narg, const char *tname);
typedef void(__cdecl *lua_pushcclosureFn)(void *L, void *fn, int n);
typedef int(__cdecl *luaL_refFn)(void *L, int t);
typedef void(__cdecl *lua_rawgetiFn)(void *L, int t, int n);

// MS_FINDER_ADD_EXTERN
extern lua_typeFn lua_type;
extern lua_settopFn lua_settop;
extern lua_isnumberFn lua_isnumber;
extern lua_pushbooleanFn lua_pushboolean;
extern lua_tolstringFn lua_tolstring;
extern lua_getfieldFn lua_getfield;
extern lua_setfieldFn lua_setfield;
extern luaL_loadfileFn luaL_loadfile;
extern luaL_loadbufferFn luaL_loadbuffer;
extern luaL_loadstringFn luaL_loadstring;
extern luaL_newstateFn luaL_newstate;
extern lua_pcallFn lua_pcall;
extern lua_closeFn lua_close;
extern luaL_openlibsFn luaL_openlibs;
extern lua_atpanicFn lua_atpanic;
extern luaL_checkudataFn luaL_checkudata;
extern lua_pushcclosureFn lua_pushcclosure;
extern lua_newthreadFn lua_newthread;
extern luaL_refFn luaL_ref;
extern lua_rawgetiFn lua_rawgeti;

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)		

#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define luaL_dofile(L, fn) \
	(luaL_loadfile(L, fn) || lua_pcall(L, 0, -1, 0))

#define luaL_dostring(L, s) \
	(luaL_loadstring(L, s) || lua_pcall(L, 0, -1, 0))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_setglobal(L,s)	lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)	lua_getfield(L, LUA_GLOBALSINDEX, (s)))

#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)

#define lua_pop(L,n)		lua_settop(L, -(n)-1)