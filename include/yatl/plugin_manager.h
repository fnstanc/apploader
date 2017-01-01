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
    static_assert(yatl::internal::IsBaseOf<T, Base>::Value);

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager &) = delete;
    PluginManager &operator=(const PluginManager &) = delete;
    bool init(const std::string &plugin_conf_file);
    bool afterInit();
    bool beforeUninit();
    bool uninit();
    void run();

    // for plugin library load/unload
    void install(Plugin *plugin);
    void uninstall(Plugin *plugin);
    Plugin *findPlugin(const std::string &name) const;

    void registerModule(const std::string &name, Module *module);
    void unregisterModule(const std::string &name);
    Module *findModule(const std::string &name) const;

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
};

typedef void (*PluginInstallFunc)(PluginManager *);
typedef void (*PluginUninstallFunc)(PluginManager *);

} // namespace yatl
#endif
