#pragma once

#include <string>

class ITree
{
public:
    virtual ~ITree() = default;

    virtual std::string name() const = 0;
    virtual void print() const = 0;
    virtual std::string print_checks() const = 0;

    virtual bool empty() const = 0;
    virtual int  size() const = 0;
    virtual int  height() const = 0;

    virtual void insert(int x) = 0;
    virtual void remove(int x) = 0;
    virtual bool search(int x) = 0;
};
