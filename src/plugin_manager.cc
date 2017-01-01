/*
 * File Name: plugin_manager.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:37:17 AM CST
 */

#include "yatl/plugin_manager.h"
#include "yatl/plugin.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "dynlib.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace yatl {

PluginManager::PluginManager() :
    dynlib_manager_(new DynLibManager()),
    loaded_libraries_(),
    modules_(),
    plugin_conf_file_()
{

}

PluginManager::~PluginManager()
{

}

bool PluginManager::init(const std::string &conf_file)
{
    std::ifstream fs(conf_file);
    if (!fs.good()) {
        std::cerr << "Failed to open config file: " << conf_file << std::endl;
        return false;
    }

    rapidjson::Document doc;
    rapidjson::IStreamWrapper sw(fs);
    doc.ParseStream(sw);
    if (doc.HasParseError()) {
        return false;
    }

    assert(doc.IsObject());

    auto &plugins = doc["plugins"];
    for (auto it = plugins.MemberBegin(); it != plugins.MemberEnd(); ++it) {
        auto name = it->name.GetString();
        auto &val = it->value;

        auto lib = val.FindMember("lib");
        if (lib == val.MemberEnd()) {
            std::cerr << "Plugin config requires shared library path, name = " << name << std::endl;
            continue;
        }
        auto path = lib->value.GetString();
        if (loadPluginLibrary(path)) {
            loaded_libraries_.insert(path);
        } else {
            std::cerr << "Failed to load plugin " << name << std::endl;
        }
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
