/* vim: set ts=8 et sw=4 sta ai cin: */
#include <sofia-sip/su_wait.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "luasofia_su_root.h"

static int lua_su_root_destroy(lua_State *L)
{
    su_root_userdata *luserdata = NULL;
   
    /* get and check first argument (should be a engine) */
    luserdata = (su_root_userdata*)luaL_checkudata(L, 1, SU_ROOT_MTABLE);

    if (luserdata->root) {
        su_root_destroy(luserdata->root);
        luserdata->root = NULL;
    }

    return 0;
}

static int lua_su_root_magic(lua_State *L)
{
    return 0;
}

static int lua_su_root_register(lua_State *L)
{
    return 0;
}

static int lua_su_root_deregister(lua_State *L)
{
    return 0;
}

static int lua_su_root_unregister(lua_State *L)
{
    return 0;
}

static int lua_su_root_run(lua_State *L)
{
    su_root_userdata *luserdata = NULL;
   
    /* get and check first argument (should be a engine) */
    luserdata = (su_root_userdata*)luaL_checkudata(L, 1, SU_ROOT_MTABLE);

    su_root_run(luserdata->root);

    return 0;
}

static int lua_su_root_break(lua_State *L)
{
    su_root_userdata *luserdata = NULL;
   
    /* get and check first argument (should be a engine) */
    luserdata = (su_root_userdata*)luaL_checkudata(L, 1, SU_ROOT_MTABLE);

    su_root_break(luserdata->root);

    return 0;
}

static int lua_su_root_step(lua_State *L)
{
    return 0;
}

static int lua_su_root_get_max_defer(lua_State *L)
{
    return 0;
}

static int lua_su_root_set_max_defer(lua_State *L)
{
    return 0;
}

static int lua_su_root_task(lua_State *L)
{
    return 0;
}

static const luaL_Reg su_root_meths[] = {
    {"magic", lua_su_root_magic },
    {"register", lua_su_root_register },
    {"deregister", lua_su_root_deregister },
    {"unregister", lua_su_root_unregister },
    {"run", lua_su_root_run },
    {"break", lua_su_root_break },
    {"step", lua_su_root_step },
    {"get_max_defer", lua_su_root_get_max_defer },
    {"set_max_defer", lua_su_root_set_max_defer },
    {"task", lua_su_root_task },
    {"__gc", lua_su_root_destroy},
    {NULL, NULL}
};

int luaopen_su_root(lua_State *L)
{
    luaL_newmetatable(L, SU_ROOT_MTABLE);
    /* metatable.__index = metatable */
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, su_root_meths);
    lua_pop(L, 1);

    return 0;
}
