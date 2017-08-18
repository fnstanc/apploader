/*
 * File Name: foo.cpp
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 16:13:51
 */

#include "foo.h"
#include "bar.h"
#include "yatl/plugin_manager.h"
#include <cstdio>
#include <iostream>

YATL_API void installPlugin(yatl::PluginManager *pm)
{
    CREATE_PLUGIN(pm, yatl::PluginFoo);
}

YATL_API void uninstallPlugin(yatl::PluginManager *pm)
{
    DESTROY_PLUGIN(pm, yatl::PluginFoo);
}

namespace yatl {

PluginFoo::PluginFoo(PluginManager *pm) : Plugin(CLASS_NAME(yatl::PluginFoo), pm)
{
}

PluginFoo::~PluginFoo()
{
}

bool PluginFoo::install()
{
    REG_MODULE(plugin_manager_, ModuleFoo, ModuleFoo);
    return true;
}

bool PluginFoo::uninstall()
{
    UNREG_MODULE(plugin_manager_, ModuleFoo, ModuleFoo);
    return true;
}

bool ModuleFoo::afterInit()
{
    std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
    ModuleBar *bar = plugin_->pluginManager()->findModule<ModuleBar>();
    bar->bar();
    return true;
}

void ModuleFoo::execute()
{
   std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
}

}
