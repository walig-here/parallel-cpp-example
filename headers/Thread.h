#ifndef THREAD_H
#define THREAD_H

#pragma once
#include <thread>


class Thread
{
public:

    virtual ~Thread() = 0;
    std::string getId() { return id; }
    void join();
    void detach();

protected:

    Thread(std::string name);
    virtual void main() = 0;
    
private:

    static int id_counter;

    std::string id;
};

#endif
