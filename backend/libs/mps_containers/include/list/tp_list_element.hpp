#ifndef MPS_LIST_CONTAINER_ELEMENT_TYPE_HPP
#define MPS_LIST_CONTAINER_ELEMENT_TYPE_HPP


namespace mps {
namespace container {
/** ------------------------------------------------------------------------------------------------------------------
 * @brief The tp_list_element class - описание элемента списка системы (mps)
 --------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct tp_list_element {
    using _value_t_ = T;
    using _element_t_ = tp_list_element<_value_t_>;

    _value_t_* value_ = nullptr;            /// <--- указатель на значение
    _element_t_* prev_element_ = nullptr;   /// <--- указатель на предыдущий элемент списка
    _element_t_* next_element_ = nullptr;   /// <--- указатель на следующий элемент списка

    /**
     * @brief tp_list_element - конструктор (инициализация по умолчанию)
     */
    explicit tp_list_element() = default;

    /**
     * @brief tp_list_element - конструктор
     * @param v_ - указатель на фиксируемый элемент (значение)
     */
    explicit tp_list_element(_value_t_* v_);

    /**
     * деструктор
     * ! Освобождается выделенная память только для оболочки значения (tp_list_element)
     */
    ~tp_list_element();
};


//--------------------------------------------------------------------------------------------------------------------

template <typename T>
tp_list_element<T>::tp_list_element(T* v_) : value_(v_) { }

//--------------------------------------------------------------------------------------------------------------------

template <typename T>
tp_list_element<T>::~tp_list_element() {
    //(?) Освобождаем память выделенную под значение
    if (this->value_) {
        delete this->value_;
        this->value_ = nullptr;
    }

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
