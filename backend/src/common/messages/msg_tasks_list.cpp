#include "common/messages/msg_tasks_list.hpp"
#include <mps/mps_process_traits/messages_traits/messages_container/register_messages_container.hpp>


static bool register_message_list_tasks() {
    //(?) Если контейнер регистрации сообщений инициализирован, регистрируем сообщение типа list_tasks
    if (register_messages_container_) {
        return register_messages_container_->register_message<td::msg::msg_tasks_list>("list_tasks");
    }

    // В противном случае возвращаем ошибку
    return false;
}

static bool register_message_list_tasks_ = register_message_list_tasks();
