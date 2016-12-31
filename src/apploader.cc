/*
 * File Name: apploader.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:22:01 AM CST
 */

#include "apploader.h"

namespace yatl {

void AppLoader::run()
{
    if (!plugin_manager_.init("../config.json")) {
        assert(false);
    } else if(!plugin_manager_.afterInit()) {
        assert(false);
    }
    plugin_manager_.run();
    plugin_manager_.beforeUninit();
    plugin_manager_.uninit();
}

}
