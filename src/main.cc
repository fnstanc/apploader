/*
 * File Name: main.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Wed 28 Dec 2016 12:19:34 AM CST
 */

#include "plugin_manager_impl.h"
#include "optparse.h"
#include <string>
#include <cstdio>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

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
    using namespace uf;

    auto parser = optparse::OptionParser().description("uf apploader - a plugin base app framework.");
    parser.usage("%prog [options] APPNAME /path/to/config");
    parser.add_option("-i", "--id").dest("appid")
          .metavar("APPID").help("set an id to this app");
    const optparse::Values options = parser.parse_args(argc, argv);
    auto args = parser.args();

    if (args.size() != 2) {
        parser.print_usage();
        return -1;
    }

    const std::string &conf_file = args[1];
    const std::string &app_name = args[0];
    std::string str_app_id = "0";
    
    if (options.is_set("appid")) {
        str_app_id = options["appid"];
    }

    PluginManagerImpl pm;
    pm.AppName(app_name);

    std::string title = app_name;
    if (!str_app_id.empty()) {
        title += "-" + str_app_id;
        int id = std::atoi(str_app_id.c_str());
        pm.AppID(id);
    }
    setConsoleTitle(title);

    if (!pm.Init(conf_file)) {
        assert(false);
    } else if(!pm.AfterInit()) {
        assert(false);
    }

    while (!exitApp) {
        pm.Execute();
    }

    pm.BeforeShutdown();
    pm.Shutdown();
    return 0;
}
