/*
 * File Name: main.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:19:34 AM CST
 */

#include "plugin_manager_impl.h"
#include "getopt.h"
#include <string>
#include <cstdio>

void usage()
{
    fprintf(stdout, "This is Usage.\n");
}

int main(int argc, char *argv[])
{
    using namespace yatl;
    PluginManagerImpl pm;

    std::string conf_file;

    int ch;
    while ((ch = getopt(argc, argv, "c:h")) != -1) {
        switch (ch) {
        case 'c': {
            conf_file = optarg;
            break;
        }
        case 'h': {
            usage();
            return 0;
        }
        default: {
            fprintf(stderr, "Unknown arg %d.\n", ch);
            return -1;
        }
        }
    }

    if (conf_file.empty())
    {
        fprintf(stderr, "Need a config file.\n");
        usage();
        return -2;
    }
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
