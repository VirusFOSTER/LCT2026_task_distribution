#include "common/messages/msg_request.hpp"
#include <mps/mps_process_traits/messages_traits/messages_container/register_messages_container.hpp>


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

static bool register_message_request() {
    //(?) Если контейнер регистрации сообщений инициализирован, регистрируем сообщение типа user_request
    if (register_messages_container_) {
        return register_messages_container_->register_message<td::msg::msg_request>("user_request");
    }

    // В противном случае возвращаем ошибку
    return false;
}

static bool register_message_request_ = register_message_request();



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

static bool register_message_request_database() {
    //(?) Если контейнер регистрации сообщений инициализирован, регистрируем сообщение типа user_request_database
    if (register_messages_container_) {
        return register_messages_container_->register_message<td::msg::msg_request>("user_request_database");
    }

    // В противном случае возвращаем ошибку
    return false;
}

static bool register_message_request_database_ = register_message_request_database();
