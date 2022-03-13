#include "Trace.hpp"

Trace *Trace::traceInstance = nullptr;

std::mutex Trace::traceInstanceMutex;

Trace::~Trace()
{
    delete traceInstance;
}

Trace::Trace()
{

}

Trace *Trace::getInstance(void)
{
    if (traceInstance == nullptr)
    {
        traceInstanceMutex.lock();
        traceInstance = new Trace();
        traceInstanceMutex.unlock();
    }
    return (traceInstance);
}

void Trace::add(const std::string &traceMsg)
{
    static Trace *traceInstance = getInstance();
    if (traceInstance->size() > 20)
        traceInstance->pop();
    traceInstance->push(traceMsg);
}

void Trace::printHistory(std::ostream &os)
{
    os << "Printing Trace History:" << std::endl;
    for (std::deque<std::string>::const_iterator cit = c.begin(); cit != c.end(); ++cit)
        os << *cit << std::endl;
}
