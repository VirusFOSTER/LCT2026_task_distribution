#ifndef SYSTEM_PROCESS_INTERFACE_SEQUENCE_READER_HPP
#define SYSTEM_PROCESS_INTERFACE_SEQUENCE_READER_HPP

#include "buffer_concept.hpp"

namespace mps {
namespace process {
namespace interface {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The Isequence_writer class - интерфейс писателя сообщения типа T_message
 ----------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
class Isequence_writer {
public:
    using _message_t_ = T_message;
    using _wr_message_t_ = buffer::message::write_message<_message_t_>;

    /**
     * @brief Isequence_writer - конструктор
     * @param bf_ - указатель на буфер хранения сообщений
     */
    explicit Isequence_writer(buffer::buffer_concept<_message_t_>* bf_);

    /**
     * деструктор
     */
    ~Isequence_writer();

    /**
     * @brief Isequence_writer - конструктор копирования (удален, запрещено создавать копии интерфейсов)
     */
    Isequence_writer(const Isequence_writer&) = delete;

    /**
     * @brief Isequence_writer - конструктор перемещения (удален, запрещено передавать другой компоненте)
     */
    Isequence_writer(const Isequence_writer&&) = delete;

    /**
     * @brief operator = - оператор присваивания (удален)
     * @return присвоенное значение (запрещено присваивать интерфейсы)
     */
    Isequence_writer& operator=(const Isequence_writer&) = delete;

    /**
     * @brief add_new_element - добавление нового элемента в буфер
     * @param ptr_message_ - указатель на добавляемое сообщение (свободный элемент должен быть взят из самого буфера)
     * @return результат добавления сообщения в буфер хранения сообщений
     */
    bool add_new_element(_wr_message_t_* ptr_message_);

    /**
     * @brief get_free_element - получение указателя на свободный элемент буфера хранения сообщений
     * @return указатель на свободный элемент буфера хранения сообщений (или nullptr, если такового нет)
     */
    _wr_message_t_* get_free_element();

private:
    buffer::buffer_concept<_message_t_>* buffer_ = nullptr;     /// <--- указатель на буфер хранения сообщений
};

//------------------------------------------------------------------------

template <typename T>
Isequence_writer<T>::Isequence_writer(buffer::buffer_concept<_message_t_>* bf_) : buffer_(bf_) { }

//------------------------------------------------------------------------

template <typename T>
bool Isequence_writer<T>::add_new_element(_wr_message_t_* ptr_message_) {
    return (this->buffer_) ? this->buffer_->add_new_element(ptr_message_) : false;
}

//------------------------------------------------------------------------

template <typename T>
typename Isequence_writer<T>::_wr_message_t_* Isequence_writer<T>::get_free_element() {
    return (this->buffer_) ? this->buffer_->get_free_element() : nullptr;
}
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
