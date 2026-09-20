#ifndef MPS_LIST_MEMORY_ELEMENT_TYPE_HPP
#define MPS_LIST_MEMORY_ELEMENT_TYPE_HPP

#include <cstdint>


namespace mps {
namespace container {
/**
 * Предварительное объявление класса list_memory_buffer
 */
template <typename T> class list_memory_buffer;

/** ------------------------------------------------------------------------------------------------------------------
 * @brief The tp_list_memory_element class - описание элемента буфера памяти, представленного в виде списка
 * Данные элементы будут выдаваться в качестве специализированных ячеек памяти.
 --------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct tp_list_memory_element {
    using _value_t_ = T;
    using _element_t_ = tp_list_memory_element<T>;

    _value_t_ value_;
    _element_t_* prev_element_ = nullptr;   /// <--- указатель на предыдущий элемент списка
    _element_t_* next_element_ = nullptr;   /// <--- указатель на следующий элемент списка

    /**
     * @brief index - получение индекса элемента в общем массиве
     * @return  индекс элемета в общем массиве
     */
    inline uint32_t index() const { return this->index_; }

private:
    /**
     * @brief tp_list_memory_element - конструктор
     */
    explicit tp_list_memory_element() = default;

    /**
     * деструктор
     */
    ~tp_list_memory_element();

private:
    uint32_t index_ = 0;        /// <--- индекс элемента в общем массиве

    ///(!) Установление класса list_memory_buffer (буфер, для которого предназначен элемент) дружественным
    friend class list_memory_buffer<T>;
};  /// <--- tp_list_memory_element


//--------------------------------------------------------------------------------------------------------------------

template <typename T>
tp_list_memory_element<T>::~tp_list_memory_element() {
    //(?) Устанавливаем для предыдущего элемента списка новый указатель на следующий
    if (this->prev_element_) {
        this->prev_element_->next_element_ = this->next_element_;
    }

    //(?) Устанавливаем для следующего элемента списка новый указатель на предыдущий
    if (this->next_element_) {
        this->next_element_->prev_element_ = this->prev_element_;
    }
}
}       /// <--- container
}   /// <--- mps

#endif
