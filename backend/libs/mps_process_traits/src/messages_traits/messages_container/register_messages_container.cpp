#include "messages_traits/messages_container/register_messages_container.hpp"

using namespace mps;
using namespace process;
using namespace container;
using namespace messages;


//----------------------------------------------------------------------

typename register_messages_container::el_messages_list*
register_messages_container::find_message(const std::string& message_name_) {
    //(?>) Ищем в списке сообщение с указанным именем
    auto ptr_register_message_ = this->first_message_;
    while (ptr_register_message_) {
        if (ptr_register_message_->register_message_->message_name() == message_name_) {
            break;
        }
        ptr_register_message_ = ptr_register_message_->next_element_;
    }

    return ptr_register_message_;
}

