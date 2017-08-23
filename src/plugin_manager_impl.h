/*
 * File Name: plugin_manager_impl.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:06:30 AM CST
 */

#ifndef _PLUGIN_MANAGER_IMPL_H
#define _PLUGIN_MANAGER_IMPL_H

#include "yatl/plugin_manager.h"
#include <memory>
#include <string>
#include <set>
#include <map>
#include <cassert>

namespace yatl {

class DynLibManager;
class Module;
class Plugin;

class PluginManagerImpl : public PluginManager
{
public:
    PluginManagerImpl();
    ~PluginManagerImpl();

    virtual void appid(int id);
    virtual int appid();
    virtual std::string &name();
    virtual void name(const std::string &n);

    bool init(const std::string &plugin_conf_file) override;
    bool afterInit() override;
    bool beforeShutdown() override;
    bool shutdown() override;
    void execute() override;

    // for plugin library load/unload
    void install(Plugin *plugin) override;
    void uninstall(Plugin *plugin) override;
    Plugin *findPlugin(const std::string &name) const override;

    void registerModule(const std::string &name, Module *module) override;
    void unregisterModule(const std::string &name) override;
    Module *findModule(const std::string &name) const override;

private:
    bool loadPluginLibrary(const std::string &lib_file);
    bool unloadPluginLibrary(const std::string &lib_file);

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

} // namespace yatl
#endif
