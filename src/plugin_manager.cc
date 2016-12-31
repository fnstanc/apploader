/*
 * File Name: plugin_manager.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:37:17 AM CST
 */

#include "yatl/plugin_manager.h"
#include "yatl/plugin.h"
#include "dynlib.h"
#include <cassert>
#include <fstream>

namespace yatl {

PluginManager::PluginManager() :
    dynlib_manager_(new DynLibManager()),
    loaded_libraries_(),
    modules_(),
    plugin_conf_file_(),
    plugin_configs_()
{

}

PluginManager::~PluginManager()
{

}

bool PluginManager::init(const std::string &plugin_conf_file)
{
    if (!parseConfig(plugin_conf_file))
        return false;

    if (plugin_configs_.empty())
        return false;

    for (auto &it : plugin_configs_) {
        const json &j = it.second;
        std::string lib = j.find("lib").value().get<std::string>();
        if (loaded_libraries_.count(lib) > 0) {
            std::cerr << "Already loaded a plugin from " << lib << std::endl;
            continue;
        }
        loadPluginLibrary(lib);
        loaded_libraries_.insert(lib);
    }

    return true;
}

bool PluginManager::afterInit()
{
    for (auto &it : plugins_) {
        it.second->afterInit();
    }
    return true;
}

bool PluginManager::beforeUninit()
{
    for (auto &it : plugins_) {
        it.second->beforeUninit();
    }
    return true;
}

bool PluginManager::uninit()
{
    for (auto &it : loaded_libraries_) {
        unloadPluginLibrary(it);
    }
    loaded_libraries_.clear();
    return true;
}

void PluginManager::run()
{
    for (auto &it : plugins_) {
        it.second->run();
    }
}

void PluginManager::install(Plugin *plugin)
{
    bool res = plugin->install();
    if (res) {
        plugins_.emplace(plugin->name(), plugin);
    }
}

void PluginManager::uninstall(Plugin *plugin)
{
    auto it = plugins_.find(plugin->name());
    assert(it != plugins_.end());
    plugin->uninstall();
    plugins_.erase(it);
}

Plugin *PluginManager::findPlugin(const std::string &name) const
{
    auto it = plugins_.find(name);
    return it != plugins_.end() ? it->second : nullptr;
}

void PluginManager::registerModule(const std::string &name, Module *module)
{
    modules_.emplace(name, module);
}

void PluginManager::unregisterModule(const std::string &name)
{
    modules_.erase(name);
}

Module *PluginManager::findModule(const std::string &name) const
{
    auto it = modules_.find(name);
    return it != modules_.end() ? it->second : nullptr;
}

bool PluginManager::parseConfig(const std::string &conf)
{
    std::ifstream fs(conf);
    json config;
    fs >> config;

    json::iterator it = config.begin();
    json::iterator last = config.end();

    auto isValid = [](const json &j) {
        if (!j.is_object())
            return false;

        json::const_iterator lib = j.find("lib");
        if (lib == j.end())
            return false;
        else if(!lib->is_string())
            return false;

        return true;
    };

    for (; it != last; ++it) {
        if (isValid(it.value())) {
            plugin_configs_[it.key()] = it.value();
        } else {
            std::cerr << "Error config, key = " << it.key() << ", val = " << it.value() << std::endl;
        }
    }

    return true;
}

bool PluginManager::loadPluginLibrary(const std::string &lib_file)
{
    DynLib *lib = dynlib_manager_->load(lib_file);
    if (!lib) {
        std::cerr << "Failed to load " << lib_file << std::endl;
        return false;
    }
    PluginInstallFunc func = (PluginInstallFunc)lib->getSymbol("installPlugin");
    assert(func != NULL);
    func(this);
    return true;
}

bool PluginManager::unloadPluginLibrary(const std::string &lib_file)
{
    DynLib *lib = dynlib_manager_->get(lib_file);
    assert(lib != NULL);
    PluginUninstallFunc func = (PluginUninstallFunc)lib->getSymbol("uninstallPlugin");
    assert(func != NULL);
    func(this);
    dynlib_manager_->unload(lib_file);
    return true;
}

}
