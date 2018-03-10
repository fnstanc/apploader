/*
 * File Name: plugin_manager_impl.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:37:17 AM CST
 */

#include "plugin_manager_impl.h"
#include "plugin.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "dynlib.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace uf {

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

void PluginManagerImpl::AppID(int id)
{
    appid_ = id;
}

int PluginManagerImpl::AppID()
{
    return appid_;
}

const std::string & PluginManagerImpl::AppName()
{
    return name_;
}

void PluginManagerImpl::AppName(const std::string &n)
{
    name_ = n;
}

bool PluginManagerImpl::Init(const std::string &conf_file)
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

	auto &app_conf = doc.FindMember(AppName().c_str());
	if (app_conf == doc.MemberEnd()) {
		std::cerr << "There is no app named " << AppName() << std::endl;
		return false;
	}

    auto &plugins = app_conf->value["plugins"];
	assert(plugins.IsArray());

    for (auto it = plugins.Begin(); it != plugins.End(); ++it) {
		std::string lib_name = it->GetString();
        if (LoadPluginLibrary(lib_name)) {
            loaded_libraries_.insert(lib_name);
        } else {
            std::cerr << "Failed to load plugin " << lib_name << std::endl;
        }
    }

    for (auto &it : plugins_) {
        if (!it.second->Init())
        {
            return false;
        }
    }

    return true;
}

bool PluginManagerImpl::AfterInit()
{
    for (auto &it : plugins_) {
        if (!it.second->AfterInit())
        {
            return false;
        }
    }
    return true;
}

bool PluginManagerImpl::BeforeShutdown()
{
    for (auto &it : plugins_) {
        it.second->BeforeShutdown();
    }
    return true;
}

bool PluginManagerImpl::Shutdown()
{
    for (auto &it : plugins_) {
        it.second->Shutdown();
    }

    for (auto &it : loaded_libraries_) {
        UnloadPluginLibrary(it);
    }
    loaded_libraries_.clear();
    return true;
}

void PluginManagerImpl::Execute()
{
    for (auto &it : plugins_) {
        it.second->Execute();
    }
}

void PluginManagerImpl::Install(Plugin *plugin)
{
    bool res = plugin->Install();
    if (res) {
        plugins_.emplace(plugin->Name(), plugin);
    }
}

void PluginManagerImpl::Uninstall(Plugin *plugin)
{
    auto it = plugins_.find(plugin->Name());
    assert(it != plugins_.end());
    plugin->Uninstall();
    plugins_.erase(it);
}

Plugin *PluginManagerImpl::FindPlugin(const std::string &name) const
{
    auto it = plugins_.find(name);
    return it != plugins_.end() ? it->second : nullptr;
}

void PluginManagerImpl::RegisterModule(const std::string &name, Module *module)
{
    modules_.emplace(name, module);
}

void PluginManagerImpl::UnregisterModule(const std::string &name)
{
    modules_.erase(name);
}

Module *PluginManagerImpl::FindModule(const std::string &name) const
{
    auto it = modules_.find(name);
    return it != modules_.end() ? it->second : nullptr;
}

bool PluginManagerImpl::LoadPluginLibrary(const std::string &lib_name)
{
    DynLib *lib = dynlib_manager_->Load(lib_name);
    if (!lib) {
        return false;
    }
    PluginInstallFunc func = (PluginInstallFunc)lib->GetSymbol("InstallPlugin");
    assert(func != NULL);
    func(this);
    return true;
}

bool PluginManagerImpl::UnloadPluginLibrary(const std::string &lib_name)
{
    DynLib *lib = dynlib_manager_->Get(lib_name);
    assert(lib != NULL);
    PluginUninstallFunc func = (PluginUninstallFunc)lib->GetSymbol("UninstallPlugin");
    assert(func != NULL);
    func(this);
    dynlib_manager_->Unload(lib_name);
    return true;
}

}
