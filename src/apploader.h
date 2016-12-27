/*
 * File Name: apploader.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:21:55 AM CST
 */

#ifndef _APPLOADER_H
#define _APPLOADER_H

#include "yatl/plugin_manager.h"

namespace yatl {

class AppLoader
{
public:
    AppLoader() = default;
    ~AppLoader() = default;

private:
    PluginManager plugin_manager_;
};

} // namespace yatl
#endif
