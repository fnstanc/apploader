# apploader
A plugin based app framework for C++.

## How to build

For *nix

    git clone https://github.com/fnstanc/apploader
    cd ./apploader
    mkdir build
    cd build && cmake ..
    make -j4


For windows

    Generate vs solution with cmake ...

## Config file
Config file is wirten in json.
* plugins: a list of plugin your app needs.
* assets: path which your app will load from.


    {
        "APPNAME1" : {
            "plugins" : ["shared_lib_name1", "shared_lib_name2", ...],
            "assets" : "/path/to/assets"
        },

        "APPNAME2" : {
            "plugins" : ["shared_lib_name1", "shared_lib_name2", ...],
            "assets" : "/path/to/assets"
        }
    }

## Run

    apploader [options] APPNAME path/to/config_file
