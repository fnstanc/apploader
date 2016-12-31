/*
 * File Name: foo.cpp
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 16:13:51
 */

#include "bar.h"
#include "yatl/plugin_manager.h"
#include <cstdio>
#include <iostream>

YATL_API void installPlugin(yatl::PluginManager *pm)
{
    CREATE_PLUGIN(pm, yatl::PluginBar);
}

YATL_API void uninstallPlugin(yatl::PluginManager *pm)
{
    DESTROY_PLUGIN(pm, yatl::PluginBar);
}

namespace yatl {

PluginBar::PluginBar(PluginManager *pm) : Plugin(CLASS_NAME(yatl::PluginBar), pm)
{
}

PluginBar::~PluginBar()
{
}

class ModuleBarImpl : public ModuleBar
{
public:
    ModuleBarImpl(Plugin *plugin) { plugin_ = plugin; }

    void run() override
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
    }

    void bar()
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
    }
};

bool PluginBar::install()
{
    REG_MODULE(plugin_manager_, ModuleBar, ModuleBarImpl);
    return true;
}

bool PluginBar::uninstall()
{
    UNREG_MODULE(plugin_manager_, ModuleBar, ModuleBarImpl);
    return true;
}

}
