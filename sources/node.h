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
private:
    std::vector<T> (*processf)(std::vector<T> inputs);
    std::vector<Point<T>> input;
    std::vector<Connection<T>> output;
    unsigned long long inputc, outputc;

public:
    Node(unsigned long long inputs, unsigned long long outputs) {
        processf = [&outputs](std::vector<T> inputs){return std::vector<T>(outputs, T());};
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

    //process the Node
    void process()
    {
        std::vector<T> linput(inputc);
        for(int i = 0; i < inputc; i++) {
            linput[i] = input[i].get();
        }
        std::vector<T> res = processf(linput);
        for(int i = 0; i < outputc; i++) {
            output[i].set(res[i]);
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
        for(auto i : output) {
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
        process_max_iteration_count += node.inputc;
    }

    void remove_node(Node<T> &node)
    {
        nodes.erase(std::find(nodes.begin(), nodes.end(), &node));
        process_max_iteration_count -= node.inputc;
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
