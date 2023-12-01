//
// Created by Ryan Lee 李瑞林 on 2023/12/1.
//

#ifndef NODEBASE_NODEBASEUTILLITY_H
#define NODEBASE_NODEBASEUTILLITY_H

#include <exception>
#include <string>

namespace NodeBaseUtillity {
    class Warning : public std::exception
    {
    private:
        std::string content;

    public:
        Warning(const std::string& msg) {
            content = msg;
        }

        const char* what()
        {
            return content.c_str();
        }
    };

} // NodeBaseUtillity

#endif //NODEBASE_NODEBASEUTILLITY_H
