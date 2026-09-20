#ifndef MPS_AVL_TREE_ELEMENT_TYPE_HPP
#define MPS_AVL_TREE_ELEMENT_TYPE_HPP

#include <cstdint>

namespace mps {
namespace container {
/**
 * Предварительное объявление класса avl_tree_buffer
 */
template <typename T> class avl_tree_buffer;

/** ------------------------------------------------------------------------------------------------------------------
 * @brief The avl_list_element class - описание элемента avl-дерева (элемента списка).
 * Данный элемент предназначен в основном для полного прохода всех элементов avl-дерева
 -------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class avl_list_element {
public:
    using _value_t_ = T;
    using _element_t_ = avl_list_element<T>;

    /**
     * @brief avl_list_element - конструктор (инициализация по умолчанию)
     */
    explicit avl_list_element() = default;

    /**
     * @brief ~avl_list_element - деструктор
     */
    virtual ~avl_list_element();

    /**
     * @brief element - получение указателя на элемент дерева (дочерний элемент)
     * @return указатель на элемент дерева (дочерний элемент)
     */
    inline _value_t_* element() { return static_cast<_value_t_*>(this); }

    /**
     * @brief next_element - получение указателя на следующий элемент списка
     * @return указатель на следующий элемент списка
     */
    inline _element_t_* next_element() const { return this->next_element_; }

    /**
     * @brief prev_element - получение указателя на предыдущий элемент списка
     * @return указатель на предыдущий элемент списка
     */
    inline _element_t_* prev_element() const { return this->prev_element_; }

protected:
    _element_t_* prev_element_ = nullptr;       /// <--- указатель на предыдущий элемент списка
    _element_t_* next_element_ = nullptr;       /// <--- указатель на следующий элемент списка
};


//----------------------------------------------------------------------------------------

template <typename T>
avl_list_element<T>::~avl_list_element() {
    if (this->next_element_) {
        this->next_element_->prev_element_ = this->prev_element_;
    }

    if (this->prev_element_) {
        this->prev_element_->next_element_ = this->next_element_;
    }
}


/** -------------------------------------------------------------------------------------------------------------------
 * @brief The avl_element class - описание элемента avl-дерева.
 * Данный элемент представляет собой стандартное описание ноды avl-дерева за исключением наличия указателя на
 * родительский элемент, от которого ветвится текущий
 ---------------------------------------------------------------------------------------------------------------------*/
template <typename T>
struct avl_element : public avl_list_element<avl_element<T>> {
    using _value_t_ = T;
    using _element_t_ = avl_element<T>;
    using _list_element_t_ = avl_list_element<avl_element<T>>;

    _value_t_* value_ = nullptr;                /// <--- указатель на значение
    _element_t_* parent_element_ = nullptr;     /// <--- указатель на родительский элемент
    _element_t_* left_element_ = nullptr;       /// <--- указатель на левый элемент avl-дерева
    _element_t_* right_element_ = nullptr;      /// <--- указатель на правый элемент avl-дерева
    uint8_t height_ = 1;                        /// <--- высота элемента дерева

    /**
     * @brief avl_element - конструктор (инициализация по умолчанию)
     */
    explicit avl_element() = default;

    /**
     * @brief avl_element - конструктор
     * @param v_ - указатель на фиксируемый элемент (значение)
     */
    explicit avl_element(_value_t_* v_);

    /**
     * деструктор
     */
    ~avl_element();

private:
    inline void set_next_element(_list_element_t_* next_) { this->next_element_ = next_; }
    inline void set_prev_element(_list_element_t_* prev_) { this->prev_element_ = prev_; }

    //(?) Объявление класса avl_tree_buffer как дружественного
    friend class avl_tree_buffer<T>;
};


//----------------------------------------------------------------------------------------

template <typename T>
avl_element<T>::avl_element(_value_t_* v_) : value_(v_) {}

//----------------------------------------------------------------------------------------

template <typename T>
avl_element<T>::~avl_element() { }
}       /// <--- container
}   /// <--- mps

#endif
