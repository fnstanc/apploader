/*
 * File Name: dynlib.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2016/12/26 12:02:29
 */

#ifndef _DYNLIB_H
#define _DYNLIB_H

#include <string>
#include <map>

namespace uf {

class DynLib
{
public:
    DynLib(const std::string &lib_name);
    ~DynLib();
    bool Load();
    void Unload();
    void *GetSymbol(const char *sym) const throw();
    const std::string &Filename() const;

private:
    DynLib(const DynLib &);
    DynLib &operator=(const DynLib &);

private:
    std::string filename_;
    void *handle_;
}; // class DynLib

class DynLibManager
{
public:
    DynLibManager() {}
    ~DynLibManager();
    DynLib *Load(const std::string &lib_name);
    void Unload(const std::string &lib_name);
    void UnloadAll();
    DynLib *Get(const std::string &lib_name) const;

private:
    DynLibManager(const DynLibManager &);
    DynLibManager &operator=(const DynLibManager &);

private:
    typedef std::map<std::string, DynLib *> DynLibs;
    DynLibs dynlibs_;
}; // class DynLibManager

} // namespace uf

#endif
