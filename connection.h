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

    void set(T v)
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
    connection() {
        target = nullptr;
    }

    void connect(const point<T> &to)
    {
        target = &to;
    }

    void disconnect()
    {
        target = nullptr;
    }

    void set(T value)
    {
        target->set(value);
    }
};

#endif //NODEBASE_CONNECTION_H
