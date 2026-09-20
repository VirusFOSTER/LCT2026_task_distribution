#ifndef SYSTEM_PROCESS_LIST_BUFFER_MODEL_OPTIONAL_ELEMENT_HPP
#define SYSTEM_PROCESS_LIST_BUFFER_MODEL_OPTIONAL_ELEMENT_HPP

#include "interfaces_traits/message_writer_concept.hpp"

namespace mps {
namespace process {
namespace interface {
namespace buffer {
/** --------------------------------------------------------
                 * Предварительное объявление буфера хранения сообщений
                 - ----*----------------------------------------------------*/
template <typename T_message> class list_buffer_model;

/** ----------------------------------------------------------------------------------------------------------------------------
                 * @brief The optional_list_element class - специальная прослойка между элементом списка оберткой для записи сообщений.
                 * Данная прослойка используется для хранения дополнительной информации об элементе в буфере
                 - ----*------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
class optional_list_element :  public message::write_message<T_message> {
protected:
    using _message_t_ = T_message;

    /**
                     * @brief optional_list_element - конструктор
                     * @param msg_ - указатель на сообщение
                     * @param idx_ - индекс элемента в массиве
                     */
    explicit optional_list_element(_message_t_* msg_, uint16_t idx_);

    /**
                     * @brief ~optional_list_element - деструктор
                     */
    virtual ~optional_list_element() {}

    uint16_t array_index_ = 0;                  /// <--- индекс элемента в массиве сообщений буфера

    //(!) Полный доступ для буфера хранения сообщений заданного типа
    friend class list_buffer_model<_message_t_>;
};


template <typename T>
optional_list_element<T>::optional_list_element(_message_t_* msg_, uint16_t idx_) :
    message::write_message<_message_t_>(msg_),
    array_index_(idx_)
{ }
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
