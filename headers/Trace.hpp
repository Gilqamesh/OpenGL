#ifndef TRACE_HPP
# define TRACE_HPP

# include <string>
# include <queue>
# include <mutex>
# include <iostream>

/*
* singleton class
* thread-safe
*/
class Trace : public std::queue<std::string>
{
public:
    ~Trace();
    static Trace *getInstance();
    static void add(const std::string &traceMsg);
    void printHistory(std::ostream &os);
private:
    Trace();
    static Trace *traceInstance;
    static std::mutex traceInstanceMutex;
};

#endif
