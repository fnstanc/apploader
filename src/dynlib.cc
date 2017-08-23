/*
 * File Name: dynlib.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 12:02:47
 */

#include "dynlib.h"
#include <cstdio>
#include <cassert>


#if defined(__linux__)
#include <dlfcn.h>
#define DYNLIB_LOAD(a) dlopen(a, RTLD_LAZY|RTLD_GLOBAL)
#define DYNLIB_GETSYM(a, b) dlsym(a, b)
#define DYNLIB_UNLOAD(a) dlclose(a)
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define DYNLIB_LOAD(a) (void *)LoadLibraryEx(a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define DYNLIB_GETSYM(a, b) (void *)GetProcAddress((HMODULE)(a), b)
#define DYNLIB_UNLOAD(a) FreeLibrary((HMODULE)(a))
#endif


namespace yatl {

DynLib::DynLib(const std::string &lib_name) :
    handle_(NULL)
{
    assert(!lib_name.empty());

#if defined(__linux__)
	filename_ = "lib" + lib_name + ".so";
#elif defined(_WIN32) || defined(_WIN64)
	filename_ = lib_name + ".dll";
#else
#error platform not support
#endif
}

DynLib::~DynLib()
{
    unload();
}

bool DynLib::load()
{
    assert(handle_ == NULL);
    printf("Loading library %s\n", filename_.c_str());
    handle_ = DYNLIB_LOAD(filename_.c_str());
    return NULL != handle_;
}

void DynLib::unload()
{
    if (handle_) {
        printf("Unloading library %s\n", filename_.c_str());
        DYNLIB_UNLOAD(handle_);
        handle_ = NULL;
    }
}

void *DynLib::getSymbol(const char *sym) const throw()
{
    assert(sym != NULL);
    assert(handle_ != NULL);
    return DYNLIB_GETSYM(handle_, sym);
}

const std::string &DynLib::filename() const
{
    return filename_;
}


DynLibManager::~DynLibManager()
{
    unloadAll();
}

DynLib * DynLibManager::load(const std::string &lib_name)
{
    auto iter = dynlibs_.find(lib_name);
    if (dynlibs_.end() != iter)
        return iter->second;
    DynLib *lib = new DynLib(lib_name);
    if (!lib->load()) {
        delete lib;
        return NULL;
    }
    dynlibs_.insert(DynLibs::value_type(lib_name, lib));
    return lib;
}

void DynLibManager::unload(const std::string &lib_name)
{
    auto iter = dynlibs_.find(lib_name);
    if (dynlibs_.end() != iter) {
        iter->second->unload();
        delete iter->second;
        dynlibs_.erase(iter);
    }
}

void DynLibManager::unloadAll()
{
    for (auto &iter : dynlibs_) {
        iter.second->unload();
        delete iter.second;
    }
    dynlibs_.clear();
}

DynLib *DynLibManager::get(const std::string &lib_name) const
{
    auto iter = dynlibs_.find(lib_name);
    return iter != dynlibs_.end() ? iter->second : NULL;
}

} // namespace yatl
