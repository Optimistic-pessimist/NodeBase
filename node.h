//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_NODE_H
#define NODEBASE_NODE_H

#include "connection.h"
#include <vector>

template<typename T>
class Node
{
private:
    std::vector<T> (*processf)(std::vector<T> input);
    std::vector<Point<T>> input;
    std::vector<Connection<T>> output;
    unsigned long long inputc, outputc;

public:
    Node(unsigned long long inputs, unsigned long long outputs) {
        processf = [](std::vector<Point<T>> inputs, unsigned long long outputs){return std::vector<T>(outputs, T());};
        input = std::vector<Point<T>>(inputs, Point<T>());
        output = std::vector<Connection<T>>(outputs, Connection<T>());
        inputc = inputs;
        outputc = outputs;
    }

    explicit Node(unsigned long long inputs, unsigned long long outputs, std::vector<T> (*processfunc)(std::vector<T> *input)) {
        processf = processfunc;
        input = std::vector<Point<T>>(inputs, Point<T>());
        output = std::vector<Connection<T>>(outputs, Connection<T>());
        inputc = inputs;
        outputc = outputs;
    }

    ~Node() {
        delete processf;
    }

    bool __validate()
    {
        for(auto i : input) {
            bool res = i.__validate();
            if(!res) return false;
        }
        return true;
    }

    void __refresh()
    {
        for(auto i : input) {
            i.__refresh();
        }
    }
};

template<typename T>
class Scene
{
private:
    std::vector<Node<T>*> nodes;
    std::vector<Connection<T>> input;
    std::vector<Point<T>> output;
    unsigned long long inputc, outputc;

    bool validate()
    {
        for(auto i : nodes) {
            bool res = i->__validate();
            if(!res) return false;
        }
        return true;
    }

    void refresh()
    {
        for(auto i : input) {
            i->__refresh();
        }
    }

public:
    Scene(unsigned long long inputs, unsigned long long outputs) {
        nodes = std::vector<Node<T>*>();
        input = std::vector<Connection<T>>(inputs, Connection<T>());
        output = std::vector<Point<T>>(outputs, Point<T>());
        inputc = inputs;
        outputc = outputs;
    }

    ~Scene() {
        for(auto i : nodes) delete i;
    }

    void connect(unsigned long long i, Point<T> &to)
    {
        output[i].connect(to);
    }

    void disconnect(unsigned long long i)
    {
        output[i].disconnect();
    }

    void add_node(Node<T> &node)
    {
        nodes.push_back(&node);
    }

    void remove_node(Node<T> &node)
    {
        nodes.erase(find(nodes.begin(), nodes.end(), &node));

    }

    void process()
    {
        
    }
};

#endif //NODEBASE_NODE_H
