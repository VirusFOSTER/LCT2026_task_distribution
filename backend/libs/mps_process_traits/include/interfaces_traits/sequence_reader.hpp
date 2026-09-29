#ifndef SYSTEM_PROCESS_INTERFACE_SEQUENCE_WRITER_HPP
#define SYSTEM_PROCESS_INTERFACE_SEQUENCE_WRITER_HPP

#include "buffer_concept.hpp"

namespace mps {
namespace process {
namespace interface {
template <typename T_message>
class Isequence_reader {
public:
    using _message_t_ = T_message;
    using _rd_message_t_ = buffer::message::read_message<_message_t_>;

    /**
     * @brief Isequence_reader - конструктор
     * @param bf_ - указатель на буфер хранения сообщений
     */
    explicit Isequence_reader(buffer::buffer_concept<_message_t_>* bf_);

    /**
     * деструктор
     */
    ~Isequence_reader();

    /**
     * @brief Isequence_reader - конструктор копирования (удален, запрещено создавать копии интерфейсов)
     */
    Isequence_reader(const Isequence_reader&) = delete;

    /**
     * @brief Isequence_reader - конструктор перемещения (удален, запрещено передавать другой компоненте)
     */
    Isequence_reader(const Isequence_reader&&) = delete;

    /**
     * @brief operator = - оператор присваивания (удален)
     * @return присвоенное значение (запрещено присваивать интерфейсы)
     */
    Isequence_reader& operator=(const Isequence_reader&) = delete;

    /**
     * @brief read_next_element - получение очередного сообщения из буфера хранения сообщений
     * @return указатель на специальную обертку сообщения из буфера хранения сообщений
     */
    _rd_message_t_* read_next_element();

    /**
     * @brief remove_element - удаление сообщения из очереди
     * @param rm_message_ - указатель на удаляемое сообщение
     * @return результат удаления
     */
    bool remove_element(_rd_message_t_** rm_message_);

private:
    buffer::buffer_concept<_message_t_>* buffer_ = nullptr;         /// <--- указатель на буфер хранения сообщений
    _rd_message_t_* message_ = nullptr;                             /// <--- указатель на текущее читаемое сообщение
};

//----------------------------------------------------------------------

template <typename T>
Isequence_reader<T>::Isequence_reader(buffer::buffer_concept<_message_t_>* bf_) : buffer_(bf_) {
    //(?) Регистрируем интерфейс читателя
    if (this->buffer_) {
        this->buffer_->register_interface_reader();
    }
}

//----------------------------------------------------------------------

template <typename T>
Isequence_reader<T>::~Isequence_reader() {}

//----------------------------------------------------------------------

template <typename T>
Isequence_reader<T>::_rd_message_t_* Isequence_reader<T>::read_next_element() {
    return (this->buffer_) ? this->buffer_->read_next_element(this->message_) : nullptr;
}

//----------------------------------------------------------------------

template <typename T>
bool Isequence_reader<T>::remove_element(_rd_message_t_ **rm_message_) {
    return (this->buffer_) ? this->buffer_->delete_element(rm_message_) : false;
}
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
