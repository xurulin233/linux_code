#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdio.h>

static int test_hello(lua_State* L)
{
	printf("hello lua!!\r\n");
	return 0;
}

static const struct luaL_Reg testlib[] =
{
	{"hello", test_hello},
	{NULL, NULL}
};

LUALIB_API int luaopen_test(lua_State* L)
{
	luaL_newlib(L, testlib);
	return 1;
}
