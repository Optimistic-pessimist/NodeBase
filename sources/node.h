//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_NODE_H
#define NODEBASE_NODE_H

#include "connection.h"
#include "nodeBaseUtillity.h"
#include <vector>
#include <ostream>

template<typename T>
class Node
{
public:
    std::vector<T> (*__processf)(std::vector<T> inputs);
    std::vector<Point<T>> __input;
    std::vector<Connection<T>> __output;
    unsigned long long __inputc, __outputc;

    Node(unsigned long long inputs, unsigned long long outputs) {
        __processf = nullptr;
        __input = std::vector<Point<T>>(inputs, Point<T>());
        __output = std::vector<Connection<T>>(outputs, Connection<T>());
        __inputc = inputs;
        __outputc = outputs;
    }



    explicit Node(unsigned long long inputs, unsigned long long outputs, std::vector<T> (*processfunc)(std::vector<T> *input)) {
        __processf = processfunc;
        __input = std::vector<Point<T>>(inputs, Point<T>());
        __output = std::vector<Connection<T>>(outputs, Connection<T>());
        __inputc = inputs;
        __outputc = outputs;
    }

    // copy constructor
    explicit Node(Node<T> &from) {
        __processf = from.__processf;
        __input = from.__input;
        __output = from.__output;
        __inputc = from.__inputc;
        __outputc = from.__outputc;
    }

    ~Node() {
        delete __processf;
    }

    void set_processf(std::vector<T> (*processfunc)(std::vector<T> *input))
    {
        __processf = processfunc;
    }

    //process the Node
    void process()
    {
        if(!__processf) {
            throw std::runtime_error("Node process function not set");
        }
        std::vector<T> linput(__inputc);
        for(int i = 0; i < __inputc; i++) {
            linput[i] = __input[i].get();
        }
        std::vector<T> res = __processf(linput);
        for(int i = 0; i < __outputc; i++) {
            __output[i].set(res[i]);
        }
    }

    // check for this Node if process should end early
    bool __validate()
    {
        for(auto i : __input) {
            bool res = i.__validate();
            if(!res) return false;
        }
        return true;
    }

    // check if the outputs of the Node has all been set
    bool __set()
    {
        for(auto i : __output) {
            bool res = i.__validate();
            if(!res) return false;
        }
        return true;
    }

    // return this Node to defalt state
    void __refresh()
    {
        for(auto i : __input) {
            i.__refresh();
        }
    }
};

template<typename T>
class Node_prototype
{
private:
    Node<T> *original;

public:
    Node_prototype(Node<T> &from) {
        original = &from;
    }

    Node<T>* create()
    {
        return new Node<T>(*original);
    }
};

template<typename T>
class Scene
{
private:
    std::vector<Node<T>*> nodes;
    std::vector<Connection<T>> input;
    std::vector<Point<T>> output;
    unsigned long long inputc, outputc, process_max_iteration_count;

    // check if process should end early
    bool __validate()
    {
        for(auto i : nodes) {
            bool res = i->__validate();
            if(!res) return false;
        }
        return true;
    }

    // return all Nodes to default state
    void __refresh()
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
        process_max_iteration_count = 0;
    }

    ~Scene() {
        for(auto i : nodes) delete i;
    }

    void connect(unsigned long long i, Point<T> &to)
    {
        output[i].connect(to);
    }

    void disconnect(unsigned long long i, Point<T> &to)
    {
        output[i].disconnect(to);
    }

    void add_node(Node<T> &node)
    {
        nodes.push_back(&node);
        process_max_iteration_count += node.__inputc;
    }

    void remove_node(Node<T> &node)
    {
        nodes.erase(std::find(nodes.begin(), nodes.end(), &node));
        process_max_iteration_count -= node.__inputc;
    }

    // process Scene
    std::vector<T> process(std::vector<T> inputs)
    {
        __refresh();
        for(int i = 0; i < inputc; i++) {
            input[i].set(inputs[i]);
        }
        unsigned long long iteration_count = 0;
        while(!__validate() && iteration_count <= process_max_iteration_count) {
            for(auto i : nodes) {
                if(i->__validate() && !i->__set()) {
                    i->process();
                }
            }
            iteration_count++;
        }
        throw NodeBaseUtillity::Warning("Scene contains a disconnected Point or a loop");
        std::vector<T> loutput(outputc);
        for(int i = 0; i < outputc; i++) {
            loutput[i] = output[i].get();
        }
        return loutput;
    }
};

#endif //NODEBASE_NODE_H
