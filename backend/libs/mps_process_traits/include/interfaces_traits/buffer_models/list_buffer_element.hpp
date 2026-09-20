#ifndef SYSTEM_PROCESS_LIST_BUFFER_MODEL_ELEMENT_HPP
#define SYSTEM_PROCESS_LIST_BUFFER_MODEL_ELEMENT_HPP

#include "messages_traits/message_head.hpp"
#include "interfaces_traits/message_reader_concept.hpp"
#include "list_element_optional.hpp"


namespace mps {
namespace process {
namespace interface {
namespace buffer {
/** ------------------------------------------------------------------------------------------------------------------------
 * @brief The list_buffer_element class - описание элемента буфера
 * Данное описание используется в случае применения модели буфера list_buffer_model
 --------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
class list_buffer_element :
                            public message::read_message<T_message>,
                            private optional_list_element<T_message> {
public:
    using _message_t_ = T_message;

    /**
     * @brief list_buffer_element - конструктор
     * @param buffer_ - указатель на буфер хранения сообщений
     * @param index_ - текущий индекс элемента
     */
    explicit list_buffer_element(list_buffer_model<_message_t_>* buffer_, uint16_t& index_);

    /**
     * деструктор
     */
    ~list_buffer_element() = default;

private:
    uint16_t readers_count_ = 0;                            /// <--- текущее количество читателей сообщения

    process::message::message_header* header_ = nullptr;    /// <--- указатель на заголовок сообщения

    list_buffer_element* next_element_ = nullptr;           /// <--- указатель на следующий элемент буфера
    list_buffer_element* prev_element_ = nullptr;           /// <--- указатель на предыдущий элемент буфера

    //(!) Полный доступ для буфера хранения сообщений заданного типа
    friend class list_buffer_model<_message_t_>;
};
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
