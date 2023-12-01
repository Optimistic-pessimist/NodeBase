//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_NODE_H
#define NODEBASE_NODE_H

#include "connection.h"

template<typename T>
class node
{
private:
    T* (*process)(T *input);
    point<T> *input;
    connection<T> *output;

public:
    node(T inputs, T outputs) {
        process;
    }
};

#endif //NODEBASE_NODE_H
