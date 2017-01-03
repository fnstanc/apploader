# apploader
A plugin based app framework for C++.

#How to build

*For unix

    
    git clone https://github.com/fnstanc/apploader
    cd ./apploader
    git submodule update --init
    mkdir build
    cd build && cmake ..
    make -j4


*For windows

    Generate vs solution with cmake ...

#Run

    apploader -c path/to/config_file
