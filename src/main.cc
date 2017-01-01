/*
 * File Name: main.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:19:34 AM CST
 */

#include "plugin_manager_impl.h"

int main(int argc, char *argv[])
{
    using namespace yatl;
    PluginManagerImpl pm;

#if defined(_WIN32) || defined(_WIN64)
    if (!pm.init("../config_win.json")) {
#else
    if (!pm.init("../config.json")) {
#endif
        assert(false);
    } else if(!pm.afterInit()) {
        assert(false);
    }

    pm.run();

    pm.beforeUninit();
    pm.uninit();
    return 0;
}
