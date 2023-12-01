//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_CONNECTION_H
#define NODEBASE_CONNECTION_H

#include <vector>

template<typename T>
class Point
{
private:
    T value;
    bool set;

public:
    Point()
    {
        value = T();
        set = false;
    }

    void __set(T v)
    {
        value = v;
        set = true;
    }

    void __refresh()
    {
        value = T();
        set = false;
    }

    bool __validate()
    {
        return set;
    }

    T get()
    {
        return value;
    }
};

template<typename T>
class Connection
{
private:
    std::vector<Point<T>*> target;

public:
    Connection()
    {
        target = std::vector<Point<T>*>();
    }

    void connect(Point<T> &to)
    {
        target.push_back(&to);
    }

    void disconnect(Point<T> &to)
    {
        target.erase(std::find(target.begin(), target.end(), &to));
    }

    void set(T value)
    {
        for(auto i : target) {
            i->__set(value);
        }
    }

    bool __validate()
    {
        return target->set;
    }
};

#endif //NODEBASE_CONNECTION_H
