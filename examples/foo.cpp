/*
 * File Name: foo.cpp
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 16:13:51
 */

#include "foo.h"
#include "bar.h"
#include "plugin_manager.h"
#include <cstdio>
#include <iostream>

YATL_API void InstallPlugin(uf::PluginManager *pm)
{
    CREATE_PLUGIN(pm, uf::PluginFoo);
}

YATL_API void UninstallPlugin(uf::PluginManager *pm)
{
    DESTROY_PLUGIN(pm, uf::PluginFoo);
}

namespace uf {

PluginFoo::PluginFoo(PluginManager *pm) : Plugin(CLASS_NAME(uf::PluginFoo), pm)
{
}

PluginFoo::~PluginFoo()
{
}

bool PluginFoo::Install()
{
    REG_MODULE(plugin_manager_, ModuleFoo, ModuleFoo);
    return true;
}

bool PluginFoo::Uninstall()
{
    UNREG_MODULE(plugin_manager_, ModuleFoo, ModuleFoo);
    return true;
}

bool ModuleFoo::AfterInit()
{
    std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
    ModuleBar *bar = plugin_->pluginManager()->FindModule<ModuleBar>();
    bar->bar();
    return true;
}

void ModuleFoo::Execute()
{
	if (!executed) {
		executed = true;
		std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
	}
}

}
