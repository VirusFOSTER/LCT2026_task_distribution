#ifndef REFERENCE_COUNTER_HPP
#define REFERENCE_COUNTER_HPP

#include <iostream>

namespace mps {
namespace type_traits {
namespace types {
namespace properties {
/** --------------------------------------------------------------------------------------------------------------
* @brief The counter_ class - счетчик ссылок для умного указателя
-----------------------------------------------------------------------------------------------------------------*/
class counter {
public:
    counter() : counter_(0) { }
    counter(const counter&) = delete;
    counter(counter&&) = default;

    counter& operator=(const counter&) = delete;

    ~counter() { }

    inline void reset() { this->counter_ = 0; }
    inline unsigned long get() { return this->counter_; }

    void operator++() { this->counter_++; }
    void operator++(int) { this->counter_++; }
    void operator--() { this->counter_--; }
    void operator--(int) { this->counter_--; }

private:
    unsigned long counter_ = 0;
};
}               /// <--- properties
}           /// <--- types
}       /// <--- type_traits
}   /// <--- mps

#endif
