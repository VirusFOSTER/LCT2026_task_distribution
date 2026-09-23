#include "common/types/utils/tp_time_window.hpp"

using namespace td;
using namespace types;


//----------------------------------------------------------------

tp_time_window::tp_time_window:tp_time_window(const tp_time_window& tw_) {
    *this = tw_;
}

//----------------------------------------------------------------

tp_time_window::tp_time_window(tp_time_window&& tw_) noexcept {
    *this = tw_;
}

//----------------------------------------------------------------

tp_time_window& tp_time_window::operator=(const tp_time_window& tw_) {
    this->time_begin_ = tw_.time_begin();
    this->time_end_ = tw_.time_end();

    return *this;
}

//----------------------------------------------------------------

tp_time_window& tp_time_window::operator=(tp_time_window&& tw_) {
    this->time_begin_ = tw_.time_begin();
    this->time_end_ = tw_.time_end();

    tw_.time_begin_.clear();
    tw_.time_end_.clear();

    return *this;
}
