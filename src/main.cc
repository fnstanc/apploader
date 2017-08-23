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

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void usage()
{
    const char *u = "usage: apploader [-n appname] -c path/to/config\r\n";
    fprintf(stdout, u);
}

void setConsoleTitle(const std::string &title)
{
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTitle(title.c_str());
#else

#endif
}

bool exitApp = false;
int main(int argc, char *argv[])
{
    using namespace yatl;
    PluginManagerImpl pm;

    std::string conf_file;
    std::string app_name = "AppLoader";
    std::string str_app_id = "0";
    int ch;
    while ((ch = getopt(argc, argv, "c:h:n")) != -1) {
        switch (ch) {
            case 'c': {
                conf_file = optarg;
            }
            break;
            case 'h': {
                usage();
                return 0;
            }
            case 'n': {
                app_name = optarg;
            }
            break;
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

    int id = std::atoi(str_app_id.c_str());
    pm.appid(id);
    pm.name(app_name);

    std::string title = app_name + "-" + str_app_id;
    setConsoleTitle(title);

    if (!pm.init(conf_file)) {
        assert(false);
    } else if(!pm.afterInit()) {
        assert(false);
    }

    while (!exitApp) {
        pm.execute();
    }

    pm.beforeShutdown();
    pm.shutdown();
    return 0;
}
