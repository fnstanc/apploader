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

namespace yatl {

class DynLibManager;

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();
    PluginManager(const PluginManager &) = delete;
    PluginManager &operator=(const PluginManager &) = delete;
    bool load(const std::string &lib_file);
    bool unload(const std::string &lib_file);

private:
    typedef std::set<std::string> LoadedPlugins;
    std::unique_ptr<DynLibManager> dynlib_manager_;
    LoadedPlugins loaded_plugins_;
};

typedef bool (*PluginInstallFunc)(PluginManager *);
typedef bool (*PluginUninstallFunc)(PluginManager *);

} // namespace yatl
#endif
