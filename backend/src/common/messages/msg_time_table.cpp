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

    //(?>) Считываем временную матрицу путей
    this->matricies_.reserve(t_->size());
    this->table_ = new mps::container::list_buffer<float>(t_->size() * t_->size());
    for (int32_t i = 0; i < t_->size(); ++i) {
        for (int32_t j = 0; j < t_->asArray(i)->size(); ++j) {
            float* new_time_ = new float;
            *new_time_ = t_->asArray(i)->asReal(j);
            this->table_->append(new_time_);
        }
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
