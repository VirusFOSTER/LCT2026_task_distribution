#include "common/messages/msg_times_table.hpp"
#include <mps/mps_process_traits/messages_traits/messages_container/register_messages_container.hpp>

using namespace td;
using namespace msg;

//-----------------------------------------------------------------

msg_time_table& msg_time_table::operator=(const mps::json::array::JsonArray* t_) {
    //(?) Очищаем сообщение как элемент буферая
    if (this->matricies_.size()) {
        this->matricies_.clear();
    }

    if (this->profiles_.size()) {
        this->profiles_.clear();
    }

    //(?>) Считываем все матрицы времен путей (по профилям)
    this->matricies_.reserve(t_->size());
    this->profiles_.reserve(t_->size());
    for (int32_t i = 0; i < t_->size(); ++i) {
        auto profile_matrix_ = t_->asObject(i);
        this->profiles_.emplace_back(profile_matrix_->asString("profile"));
        auto matrix_ = profile_matrix_->asArray("matrix");
        _matrix_t_* new_matrix_ = new _matrix_t_(matrix_->size() * matrix_->size());
        for (int32_t j = 0; j < matrix_->size(); ++i) {
            for (int32_t k = 0; k < matrix_->asArray(j)->size(); ++k) {
                float* new_time_ = new float;
                *new_time_ = matrix_->asArray(j)->asReal(k);
                new_matrix_->append(new_time_);
            }
        }
        this->matricies_.emplace_back(new_matrix_);
    }

    // Возвращаем результат заполнения сообщения
    return *this;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

static bool register_message_time_table() {
    //(?) Если контейнер регистрации сообщений инициализирован, регистрируем сообщение типа time_table
    if (register_messages_container_) {
        return register_messages_container_->register_message<td::msg::msg_time_table>("time_table");
    }

    // В противном случае возвращаем ошибку
    return false;
}

static bool register_message_time_table_ = register_message_time_table();
