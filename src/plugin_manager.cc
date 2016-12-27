/*
 * File Name: plugin_manager.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:37:17 AM CST
 */

#include "yatl/plugin_manager.h"
#include "dynlib.h"
#include <cassert>

namespace yatl {

PluginManager::PluginManager() :
    dynlib_manager_(new DynLibManager())
{

}

PluginManager::~PluginManager()
{

}

bool PluginManager::load(const std::string &lib_file)
{
    if (loaded_plugins_.find(lib_file) != loaded_plugins_.end())
        return true;
    DynLib *lib = dynlib_manager_->load(lib_file);
    if (!lib)
        return false;
    PluginInstallFunc func = (PluginInstallFunc)lib->getSymbol("installPlugin");
    assert(func != NULL);
    if (!func(this))
        return false;
    loaded_plugins_.insert(lib_file);
    return true;
}

bool PluginManager::unload(const std::string &lib_file)
{
    if (loaded_plugins_.find(lib_file) == loaded_plugins_.end())
        return true;
    DynLib *lib = dynlib_manager_->get(lib_file);
    assert(lib != NULL);
    PluginUninstallFunc func = (PluginUninstallFunc)lib->getSymbol("uninstallPlugin");
    assert(func != NULL);
    func(this);
    loaded_plugins_.erase(lib_file);
    return true;
}

}
