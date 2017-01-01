/*
 * File Name: main.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:19:34 AM CST
 */

#include "yatl/plugin_manager.h"

int main(int argc, char *argv[])
{
    using namespace yatl;
    PluginManager pm;

    if (!pm.init("../config.json")) {
        assert(false);
    } else if(!pm.afterInit()) {
        assert(false);
    }

    pm.run();

    pm.beforeUninit();
    pm.uninit();
    return 0;
}
