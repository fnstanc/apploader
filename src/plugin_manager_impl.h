/*
 * File Name: plugin_manager_impl.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:06:30 AM CST
 */

#ifndef _PLUGIN_MANAGER_IMPL_H
#define _PLUGIN_MANAGER_IMPL_H

#include "plugin_manager.h"
#include <memory>
#include <string>
#include <set>
#include <map>
#include <cassert>

namespace uf {

class DynLibManager;
class Module;
class Plugin;

class PluginManagerImpl : public PluginManager
{
public:
    PluginManagerImpl();
    ~PluginManagerImpl();

    virtual void AppID(int id);
    virtual int AppID();
    virtual const std::string &AppName();
    virtual void AppName(const std::string &n);

    bool Init(const std::string &plugin_conf_file) override;
    bool AfterInit() override;
    bool BeforeShutdown() override;
    bool Shutdown() override;
    void Execute() override;

    // for plugin library load/unload
    void Install(Plugin *plugin) override;
    void Uninstall(Plugin *plugin) override;
    Plugin *FindPlugin(const std::string &name) const override;

    void RegisterModule(const std::string &name, Module *module) override;
    void UnregisterModule(const std::string &name) override;
    Module *FindModule(const std::string &name) const override;

private:
    bool LoadPluginLibrary(const std::string &lib_name);
    bool UnloadPluginLibrary(const std::string &lib_name);

private:
    typedef std::set<std::string> LoadedLibraries;
    typedef std::map<std::string, Plugin *> Plugins;
    typedef std::map<std::string, Module *> Modules;

    std::unique_ptr<DynLibManager> dynlib_manager_;
    LoadedLibraries loaded_libraries_;
    Plugins plugins_;
    Modules modules_;
    std::string plugin_conf_file_;
    int appid_;
    std::string name_;
};

} // namespace uf
#endif
