/*
 * File Name: foo.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 16:13:49
 */

#ifndef _BAR_H
#define _BAR_H

#if defined(_WIN32) || defined(_WIN64)
#define YATL_API extern "C" __declspec(dllexport)
#else
#define YATL_API extern "C"
#endif

#include "plugin.h"

namespace uf {
class PluginManager;

class PluginBar : public Plugin
{
public:
    PluginBar(PluginManager *pm);
    ~PluginBar();
    bool Install() override;
    bool Uninstall() override;
};

class ModuleBar : public Module
{
public:
    virtual void bar() = 0;
};

}
#endif
