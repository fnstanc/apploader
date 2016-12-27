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

namespace yatl {

class DynLib
{
public:
    DynLib(const std::string &filename);
    ~DynLib();
    bool load();
    void unload();
    void *getSymbol(const char *sym) const throw();
    const std::string &filename() const;

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
    DynLib *load(const std::string &filename);
    void unload(const std::string &filename);
    void unloadAll();
    DynLib *get(const std::string &filename) const;

private:
    DynLibManager(const DynLibManager &);
    DynLibManager &operator=(const DynLibManager &);

private:
    typedef std::map<std::string, DynLib *> DynLibs;
    DynLibs dynlibs_;
}; // class DynLibManager

} // namespace yatl

#endif
