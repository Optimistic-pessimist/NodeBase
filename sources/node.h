//
// Created by Ryan Lee 李瑞林 on 2023/11/30.
//

#ifndef NODEBASE_NODE_H
#define NODEBASE_NODE_H

#include "connection.h"
#include <vector>
#include <utility>

template<typename T>
class Node
{
public:
    std::function<std::vector<T>(std::vector<T>)> __processf;
    std::vector<Point<T>> input;
    std::vector<Connection<T>> __output;
    unsigned long long __inputc, __outputc;

    void connect(unsigned long long i, Point<T> &to)
    {
        __output[i].connect(to);
    }

    void disconnect(unsigned long long i, Point<T> &to)
    {
        __output[i].disconnect(to);
    }

    Node(unsigned long long inputs, unsigned long long outputs) {
        __processf = [outputs](std::vector<T> inputs){return std::vector<T>(outputs, T());};
        input = std::vector<Point<T>>(inputs, Point<T>());
        __output = std::vector<Connection<T>>(outputs, Connection<T>());
        __inputc = inputs;
        __outputc = outputs;
    }

    explicit Node(unsigned long long inputs, unsigned long long outputs, std::function<std::vector<T>(std::vector<T>)> processfunc) {
        __processf = processfunc;
        input = std::vector<Point<T>>(inputs, Point<T>());
        __output = std::vector<Connection<T>>(outputs, Connection<T>());
        __inputc = inputs;
        __outputc = outputs;
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
            linput[i] = input[i].get();
        }
        std::vector<T> res = __processf(linput);
        for(int i = 0; i < __outputc; i++) {
            __output[i].set(res[i]);
        }
    }

    // check for this Node if process should end early
    bool __validate()
    {
        for(auto i : input) {
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
        for(auto i : input) {
            i.__refresh();
        }
    }
};

template<typename T>
class Node_prototype
{
public:
    std::function<std::vector<T>(std::vector<T>)> __processf;
    unsigned long long __inputc, __outputc;

    explicit Node_prototype(unsigned long long inputs, unsigned long long outputs, std::function<std::vector<T>(std::vector<T>)> processfunc) {
        __processf = processfunc;
        __inputc = inputs;
        __outputc = outputs;
    }

    Node<T>* create()
    {
        return new Node<T>(__inputc, __outputc, __processf);
    }
};

template<typename T>
class Scene
{
private:
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
        for(auto i : nodes) {
            i->__refresh();
        }
    }

public:
    std::vector<Node<T>*> nodes;

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
        input[i].connect(to);
    }

    void disconnect(unsigned long long i, Point<T> &to)
    {
        input[i].disconnect(to);
    }

    void add_node(Node<T> *node)
    {
        nodes.push_back(node);
        process_max_iteration_count += node->__inputc;
    }

    void remove_node(Node<T> *node)
    {
        nodes.erase(std::find(nodes.begin(), nodes.end(), node));
        process_max_iteration_count -= node->__inputc;
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
        for(auto i : nodes) {
            if(i->__validate() && !i->__set()) {
                i->process();
            }
        }
        std::vector<T> loutput(outputc);
        for(int i = 0; i < outputc; i++) {
            loutput[i] = output[i].get();
        }
        return loutput;
    }
};

#endif //NODEBASE_NODE_H
