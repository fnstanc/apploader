/*
 * File Name: plugin.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Sat 31 Dec 2016 04:24:11 PM CST
 */

#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <string>
#include <list>
#include <algorithm>

namespace uf {

class PluginManager;
class Plugin;

class Module
{
public:
    Module(const Module &) = delete;
    Module &operator=(const Module &) = delete;
    Module() : plugin_(nullptr) {}
    virtual ~Module() {}
    Plugin *GetPlugin() const;
    virtual bool Init() { return true; }
    virtual bool AfterInit() { return true; }
    virtual bool BeforeShutdown() { return true; }
    virtual bool Shutdown() { return true; }
    virtual void Execute() {}

protected:
    Plugin *plugin_;
}; // class Module

class Plugin
{
public:
    // disable copy ctor && assignment
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

    Plugin(const std::string &name, PluginManager *plugim_manager) :
        name_(name), plugin_manager_(plugim_manager){}
    virtual ~Plugin() {}
    const std::string &Name() const { return name_; }
    PluginManager *pluginManager() const { return plugin_manager_; }
    virtual bool Install() = 0;
    virtual bool Uninstall() = 0;

    virtual bool Init()
    {
        for (auto *module : modules_)
        {
            module->Init();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        for (auto *module : modules_)
        {
            module->AfterInit();
        }
        return true;
    }

    virtual bool BeforeShutdown()
    {
        for (auto *module : modules_)
        {
            module->BeforeShutdown();
        }
        return true;
    }

    virtual bool Shutdown()
    {
        for (auto *module : modules_)
        {
            module->Shutdown();
        }
        return true;
    }

    virtual void Execute()
    {
        for (auto *module : modules_)
        {
            module->Execute();
        }
    }

protected:
    void AddModule(Module *module)
    {
        modules_.push_back(module);
    }

    void RemoveModule(Module *module)
    {
        auto it = std::find(modules_.begin(), modules_.end(), module);
        if (it != modules_.end())
            modules_.erase(it);
    }

protected:
    using Modules = std::list<Module *>;
    Modules modules_;
    std::string name_;
    PluginManager *plugin_manager_;
}; // class Plugin

#define CREATE_PLUGIN(pm, PluginClass) \
    do { \
        PluginClass *plugin = new PluginClass(pm); \
        pm->Install(plugin); \
    } while(0)

#define DESTROY_PLUGIN(pm, PluginClass) \
    do { \
        auto *p = pm->FindPlugin(CLASS_NAME(PluginClass)); \
        if (p) { \
            pm->Uninstall(p); \
            delete p; \
        } \
    } while(0)

#define REG_MODULE(pm, Interface, Impl) \
    do { \
        Interface *module = new Impl(this);  \
        AddModule(module); \
        pm->RegisterModule<Interface>(module); \
    } while(0)

#define UNREG_MODULE(pm, Interface, Impl) \
    do { \
        Interface *module = pm->FindModule<Interface>(); \
        if (module) { \
            RemoveModule(module); \
            pm->UnregisterModule<Interface>();  \
            delete module; \
        } \
    } while(0)

#define CLASS_NAME(Class) typeid(Class).name()
} // namespace uf

#endif
