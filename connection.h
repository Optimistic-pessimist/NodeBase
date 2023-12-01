//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_CONNECTION_H
#define NODEBASE_CONNECTION_H

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
    Point<T> *target;

public:
    Connection()
    {
        target = nullptr;
    }

    explicit Connection(Point<T> &to)
    {
        target = &to;
    }

    void connect(Point<T> &to)
    {
        target = &to;
    }

    void disconnect()
    {
        target = nullptr;
    }

    void set(T value)
    {
        target->__set(value);
    }
};

#endif //NODEBASE_CONNECTION_H
