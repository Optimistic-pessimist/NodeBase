//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_CONNECTION_H
#define NODEBASE_CONNECTION_H

template<typename T>
class point
{
private:
    T value;

public:
    point()
    {
        value = T();
    }

    void __set(T v)
    {
        value = v;
    }

    T get()
    {
        return value;
    }
};

template<typename T>
class connection
{
private:
    point<T> *target;

public:
    connection()
    {
        target = nullptr;
    }

    explicit connection(point<T> &to)
    {
        target = &to;
    }

    void connect(point<T> &to)
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
