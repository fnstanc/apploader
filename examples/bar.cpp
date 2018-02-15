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

YATL_API void InstallPlugin(yatl::PluginManager *pm)
{
    CREATE_PLUGIN(pm, yatl::PluginBar);
}

YATL_API void UninstallPlugin(yatl::PluginManager *pm)
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

    bool Init() override
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
        return true;
    }

    bool AfterInit() override
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
        return true;
    }

    virtual bool BeforeShutdown() override
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
        return true;
    }

    virtual bool Shutdown() override
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
        return true;
    }

    void Execute() override
    {
		if (!executed) {
			executed = true;
			std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
		}
    }

    void bar()
    {
        std::cout << __FUNCTION__ << " (" << __FILE__ << "," << __LINE__ << ")" << std::endl;
    }

private:
	bool executed{ false };
};

bool PluginBar::Install()
{
    REG_MODULE(plugin_manager_, ModuleBar, ModuleBarImpl);
    return true;
}

bool PluginBar::Uninstall()
{
    UNREG_MODULE(plugin_manager_, ModuleBar, ModuleBarImpl);
    return true;
}

}
