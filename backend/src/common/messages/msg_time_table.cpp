#include "common/messages/msg_times_table.hpp"
#include <mps/mps_process_traits/messages_traits/messages_container/register_messages_container.hpp>


static bool register_message_time_table() {
    //(?) Если контейнер регистрации сообщений инициализирован, регистрируем сообщение типа time_table
    if (register_messages_container_) {
        return register_messages_container_->register_message<td::msg::msg_time_table>("time_table");
    }

    // В противном случае возвращаем ошибку
    return false;
}

static bool register_message_time_table_ = register_message_time_table();
