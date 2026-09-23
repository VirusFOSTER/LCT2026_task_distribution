#include "common/types/utils/tp_position.hpp"

using namespace td;
using namespace types;


//--------------------------------------------------------------------------

tp_position::tp_position() : longitude_(0.0f), latitude_(0.0f) { }


//--------------------------------------------------------------------------

tp_position::tp_position(const tp_position& pose_) {
    *this = pose_;
}

//--------------------------------------------------------------------------

tp_position::tp_position(tp_position&& pose_) noexcept {
    *this = pose_;
}

//--------------------------------------------------------------------------

tp_position& tp_position::operator=(const tp_position& pose_) {
    this->longitude_ = pose_.longitude();
    this->latitude_ = pose_.latitude();

    return *this;
}

//--------------------------------------------------------------------------

tp_position& tp_position::operator=(tp_position&& pose_) noexcept {
    this->longitude_ = pose_.longitude();
    this->latitude_ = pose_.latitude();

    pose_.longitude_ = 0.0f;
    pose_.latitude_ = 0.0f;

    return *this;
}
