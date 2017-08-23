/*
 * File Name: plugin_manager_impl.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:37:17 AM CST
 */

#include "plugin_manager_impl.h"
#include "yatl/plugin.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "dynlib.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace yatl {

PluginManagerImpl::PluginManagerImpl() :
    dynlib_manager_(new DynLibManager()),
    loaded_libraries_(),
    modules_(),
    plugin_conf_file_(),
    appid_(0),
    name_("AppLoader")
{

}

PluginManagerImpl::~PluginManagerImpl()
{

}

void PluginManagerImpl::appid(int id)
{
    appid_ = id;
}

int PluginManagerImpl::appid()
{
    return appid_;
}

const std::string & PluginManagerImpl::name()
{
    return name_;
}

void PluginManagerImpl::name(const std::string &n)
{
    name_ = n;
}

bool PluginManagerImpl::init(const std::string &conf_file)
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
        std::cerr << "Error when parsing config file: " << doc.GetParseError() << std::endl;
        return false;
    }

    assert(doc.IsObject());

	auto &app_conf = doc.FindMember(name().c_str());
	if (app_conf == doc.MemberEnd()) {
		std::cerr << "There is no app named " << name() << std::endl;
		return false;
	}

    auto &plugins = app_conf->value["plugins"];
	assert(plugins.IsArray());

    for (auto it = plugins.Begin(); it != plugins.End(); ++it) {
		std::string lib_name = it->GetString();
        if (loadPluginLibrary(lib_name)) {
            loaded_libraries_.insert(lib_name);
        } else {
            std::cerr << "Failed to load plugin " << lib_name << std::endl;
        }
    }

    for (auto &it : plugins_) {
        if (!it.second->init())
        {
            return false;
        }
    }

    return true;
}

bool PluginManagerImpl::afterInit()
{
    for (auto &it : plugins_) {
        if (!it.second->afterInit())
        {
            return false;
        }
    }
    return true;
}

bool PluginManagerImpl::beforeShutdown()
{
    for (auto &it : plugins_) {
        it.second->beforeShutdown();
    }
    return true;
}

bool PluginManagerImpl::shutdown()
{
    for (auto &it : plugins_) {
        it.second->shutdown();
    }

    for (auto &it : loaded_libraries_) {
        unloadPluginLibrary(it);
    }
    loaded_libraries_.clear();
    return true;
}

void PluginManagerImpl::execute()
{
    for (auto &it : plugins_) {
        it.second->execute();
    }
}

void PluginManagerImpl::install(Plugin *plugin)
{
    bool res = plugin->install();
    if (res) {
        plugins_.emplace(plugin->name(), plugin);
    }
}

void PluginManagerImpl::uninstall(Plugin *plugin)
{
    auto it = plugins_.find(plugin->name());
    assert(it != plugins_.end());
    plugin->uninstall();
    plugins_.erase(it);
}

Plugin *PluginManagerImpl::findPlugin(const std::string &name) const
{
    auto it = plugins_.find(name);
    return it != plugins_.end() ? it->second : nullptr;
}

void PluginManagerImpl::registerModule(const std::string &name, Module *module)
{
    modules_.emplace(name, module);
}

void PluginManagerImpl::unregisterModule(const std::string &name)
{
    modules_.erase(name);
}

Module *PluginManagerImpl::findModule(const std::string &name) const
{
    auto it = modules_.find(name);
    return it != modules_.end() ? it->second : nullptr;
}

bool PluginManagerImpl::loadPluginLibrary(const std::string &lib_name)
{
    DynLib *lib = dynlib_manager_->load(lib_name);
    if (!lib) {
        return false;
    }
    PluginInstallFunc func = (PluginInstallFunc)lib->getSymbol("installPlugin");
    assert(func != NULL);
    func(this);
    return true;
}

bool PluginManagerImpl::unloadPluginLibrary(const std::string &lib_name)
{
    DynLib *lib = dynlib_manager_->get(lib_name);
    assert(lib != NULL);
    PluginUninstallFunc func = (PluginUninstallFunc)lib->getSymbol("uninstallPlugin");
    assert(func != NULL);
    func(this);
    dynlib_manager_->unload(lib_name);
    return true;
}

}
