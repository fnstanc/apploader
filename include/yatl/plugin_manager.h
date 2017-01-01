/*
 * File Name: plugin_manager.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:06:30 AM CST
 */

#ifndef _PLUGIN_MANAGER_H
#define _PLUGIN_MANAGER_H

#include <memory>
#include <string>
#include <set>
#include <map>
#include <cassert>

namespace yatl {

class DynLibManager;
class Module;
class Plugin;

namespace internal {

template<typename T, typename Base>
struct IsBaseOf
{
    using Yes = char;
    struct No { char dummy[2]; };
    static Yes check(Base *base);
    static No ckeck(...);
    enum {
        Value = sizeof(check((T *)0)) == sizeof(Yes)
    };
};

} // namespace internal

#define CHECK_DERIVES(T, Base) \
    static_assert(yatl::internal::IsBaseOf<T, Base>::Value)

class PluginManager
{
public:
    PluginManager() = default;
    virtual ~PluginManager() = default;
    PluginManager(const PluginManager &) = delete;
    PluginManager &operator=(const PluginManager &) = delete;
    virtual bool init(const std::string &plugin_conf_file) = 0;
    virtual bool afterInit() = 0;
    virtual bool beforeUninit() = 0;
    virtual bool uninit() = 0;
    virtual void run() = 0;

    // for plugin library load/unload
    virtual void install(Plugin *plugin) = 0;
    virtual void uninstall(Plugin *plugin) = 0;
    virtual Plugin *findPlugin(const std::string &name) const = 0;

    virtual void registerModule(const std::string &name, Module *module) = 0;
    virtual void unregisterModule(const std::string &name) = 0;
    virtual Module *findModule(const std::string &name) const = 0;

    template <typename T>
    void registerModule(T *module)
    {
        CHECK_DERIVES(T, Module);
        registerModule(typeid(T).name(), module);
    }

    template <typename T>
    void unregisterModule()
    {
        unregisterModule(typeid(T).name());
    }

    template <typename T>
    T *findModule() const
    {
        CHECK_DERIVES(T, Module);
        const char * name = typeid(T).name();
        return (T *)findModule(name);
    }
};

typedef void (*PluginInstallFunc)(PluginManager *);
typedef void (*PluginUninstallFunc)(PluginManager *);

} // namespace yatl
#endif
