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

namespace yatl {

class PluginManager;
class Plugin;

class Module
{
public:
    Module(const Module &) = delete;
    Module &operator=(const Module &) = delete;
    Module() : plugin_(nullptr) {}
    virtual ~Module() {}
    Plugin *getPlugin() const;
    virtual bool init() { return true; }
    virtual bool afterInit() { return true; }
    virtual bool beforeShutdown() { return true; }
    virtual bool shutdown() { return true; }
    virtual void execute() {}

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
    const std::string &name() const { return name_; }
    PluginManager *pluginManager() const { return plugin_manager_; }
    virtual bool install() = 0;
    virtual bool uninstall() = 0;

    virtual bool init()
    {
        for (auto *module : modules_)
        {
            module->init();
        }
        return true;
    }

    virtual bool afterInit()
    {
        for (auto *module : modules_)
        {
            module->afterInit();
        }
        return true;
    }

    virtual bool beforeShutdown()
    {
        for (auto *module : modules_)
        {
            module->beforeShutdown();
        }
        return true;
    }

    virtual bool shutdown()
    {
        for (auto *module : modules_)
        {
            module->shutdown();
        }
        return true;
    }

    virtual void execute()
    {
        for (auto *module : modules_)
        {
            module->execute();
        }
    }

protected:
    void addModule(Module *module)
    {
        modules_.push_back(module);
    }

    void delModule(Module *module)
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
        pm->install(plugin); \
    } while(0)

#define DESTROY_PLUGIN(pm, PluginClass) \
    do { \
        auto *p = pm->findPlugin(CLASS_NAME(PluginClass)); \
        if (p) { \
            pm->uninstall(p); \
            delete p; \
        } \
    } while(0)

#define REG_MODULE(pm, Interface, Impl) \
    do { \
        Interface *module = new Impl(this);  \
        addModule(module); \
        pm->registerModule<Interface>(module); \
    } while(0)

#define UNREG_MODULE(pm, Interface, Impl) \
    do { \
        Interface *module = pm->findModule<Interface>(); \
        if (module) { \
            delModule(module); \
            pm->unregisterModule<Interface>();  \
            delete module; \
        } \
    } while(0)

#define CLASS_NAME(Class) typeid(Class).name()
} // namespace yatl

#endif
