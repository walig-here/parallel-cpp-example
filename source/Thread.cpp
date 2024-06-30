#include <string>

#include "Thread.h"


int Thread::id_counter = 0;

void Thread::join()
{
    std::thread thread(&Thread::main, this);
    if (thread.joinable())
        thread.join();
}

void Thread::detach()
{
    std::thread thread(&Thread::main, this);
    if (thread.joinable())
        thread.detach();
}

Thread::Thread(std::string name)
{
    id = name + "#" + std::to_string(Thread::id_counter++);
}

Thread::~Thread()
{
    
}