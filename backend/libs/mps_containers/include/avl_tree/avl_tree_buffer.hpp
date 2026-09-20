#ifndef MPS_AVL_TREE_BUFFER_HPP
#define MPS_AVL_TREE_BUFFER_HPP

#include "tp_tree_element.hpp"
#include "memory/list_memory_buffer.hpp"
//#include <vector>

namespace mps {
namespace container {
/** --------------------------------------------------------------------------------------------------------------------
 * @brief The avl_tree_buffer class - avl-дерево системы (mps)
 * Особенностью данного контейнера является встроенный список для полного обхода всех элементов.
 * Для всех элементов контейнера выделяется последовательный набор памяти (так же для более быстрого доступа ко всем
 * элементам по индексу). При добавлении нового значения в дерево возвращается указатель на узел дерева или индекс элемента
 * в массиве, что позволит определять элементы быстрее (без поиска).
 ----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class avl_tree_buffer {
public:
    using _value_t_ = T;
    using _element_t_ = avl_element<T>;
    using _list_element_t_ = typename avl_element<T>::_list_element_t_;

    /**
     * @brief avl_tree_buffer - конструктор
     * @param l_ - количество элементов в дереве (максимальное количество элементов в дереве)
     */
    explicit avl_tree_buffer(uint32_t l_);

    /**
     * @brief ~avl_tree_buffer - деструктор
     */
    virtual ~avl_tree_buffer();

    /**
     * @brief append - добавление нового элемента в дерево (по значению)
     * @param value_ - указатель на новый элемент дерева (значение)
     * @return указатель на элемент дерева (узел дерева)
     */
    _element_t_* append(_value_t_* value_);

    /**
     * @brief remove - удаление элемента из дерева
     * @param element_ - удаляемый элемент
     * @return результат удаления
     * (0x00 - удаление выполнено без ошибок)
     */
    uint8_t remove(_element_t_* element_);

    /**
     * @brief reset - сброс всех значений, зафиксированных в дереве
     * Новая инициализация дерева
     * @param l_ - новый размер выделяемой памяти под элементы дерева
     */
    void reset(uint32_t l_ = 0);

    /**
     * @brief main_element - получение указателя на головной (верхний) элемент дерева
     * @return указатель на головной (верхний) элемент дерева
     */
    inline _element_t_* main_element() const { return this->main_element_; }

    /**
     * @brief first - получение указателя на первый элемент списка дерева
     * @return указатель на первый элемент списка дерева
     */
    inline _list_element_t_* first() const { return this->first_element_; }

    /**
     * @brief last - получение указателя на последний элемент списка дерева
     * @return указатель на последний элемент списка дерева
     */
    inline _list_element_t_* last() const { return this->last_element_; }

    /**
     * @brief length - получение максимального количества элементов в дереве (количество зарезервированной памяти)
     * @return максимальное количество элементов в дереве (количество зарезервированной памяти)
     */
    inline uint32_t length() const { return this->length_; }

    /**
     * @brief count - получение количества записанных в дерево элементов
     * @return количество записанных в дерево элементов
     */
    inline uint32_t count() const { return this->count_; }

private:
    /**
     * @brief find_parent_element - поиск родительского элемента для указанного значения
     * @param value_ - указатель на добавляемый элемент
     * @param b_ - байт управления поиска
     * (0x00 - родительский элемент не определен,
     * 0x01 - родительский элемент выше по значению,
     * 0x02 - родительский элемент ниже по значению)
     * @return указатель на родительский элеменет
     */
    _element_t_* find_parent_element(_value_t_* value_, uint8_t& b_);

    /**
     * @brief height - получение высоты для элемента дерева
     * @param element_ - указатель на исследуемый элемент дерева
     * @return высота исследуемого элемента дерева
     */
    inline uint8_t height(_element_t_* element_) const { return (element_) ? element_->height_ : 0; }

    /**
     * @brief b_factor - вычисление balance factor для исследуемого элемента дерева (разница высот между правым и
     * левым элементами)
     * @param element_ - указатель на исследуемый элемент дерева
     * @return balance factor исследуемого элемента дерева
     */
    inline uint8_t b_factor(_element_t_* element_) {
        return this->height(element_->right_element_) - this->height(element_->left_element_);
    }

    /**
     * @brief fix_height - восстановление корректного значения поля height исследуемого элемента дерева
     * @param element_ - указатель на исследуемй элемент дерева
     */
    inline void fix_height(_element_t_* element_) {
        auto hl_ = this->height(element_->left_element_);
        auto hr_ = this->height(element_->right_element_);

        element_->height_ = (hl_ > hr_ ? hl_ : hr_) + 1;
    }

    /**
     * @brief rotate_right - правый поворот (при наличии расбалансировки дерева)
     * @param element_ - указатель на элемент, вокруг которого производится поворот
     * @return указатель на новую вершину поддерева
     */
    _element_t_* rotate_right(_element_t_* element_);

    /**
     * @brief rotate_left - левый поворот (при наличии расбалансировки дерева)
     * @param element_ - указатель на элемент, вокруг которого производится поворот
     * @return указатель на новую вершину поддерева
     */
    _element_t_* rotate_left(_element_t_* element_);

    /**
     * @brief balance_element - балансировка элемента дерева
     * @param element_ - указатель на элемент дерева, для которого выполняется балансировка
     * Должна выполняться при каждом добавлении/удалении значения в дерево
     * @return указатель на новую вершину поддерева
     */
    _element_t_* balance_element(_element_t_* element_);

    /**
     * @brief balance_tree - балансировка дерева начиная с текущего элемента
     * @param element_ - исходный элемент дерева, с которого начинается балансировка
     */
    void balance_tree(_element_t_* element_);

    /**
     * @brief insert_to_list - добавление элемента в двусвязный список
     * @param parent_ - указатель на родительский элемент
     * @param element_ - указатель на добавляемый элемент
     * @param left_ - признак добавления слева (значение element_ меньше значения parent_)
     */
    void insert_to_list(_element_t_* parent_, _element_t_* element_, bool left_ = false);

    /**
     * @brief init - метод инициализации avl-дерева (массива элементов дерева)
     */
    void init();

    /**
     * @brief clear - полная очистка avl-дерева
     */
    void clear();

protected:
    _element_t_* main_element_ = nullptr;           /// <--- верхний элемент дерева
    _list_element_t_* first_element_ = nullptr;     /// <--- первый в списке элемент дерева
    _list_element_t_* last_element_ = nullptr;      /// <--- последний в списке элемент дерева

    list_memory_buffer<_element_t_> elements_;      /// <--- массив элементов дерева

    uint32_t length_ = 0;                           /// <--- максимально количество элементов в дереве
    uint32_t count_ = 0;                            /// <--- количество записанных элементов в дерево
};


//---------------------------------------------------------------------

template <typename T>
avl_tree_buffer<T>::avl_tree_buffer(uint32_t l_) : elements_(l_), length_(l_) {
    // Инициализируем avl-дерево
    this->init();
}

//---------------------------------------------------------------------

template <typename T>
avl_tree_buffer<T>::~avl_tree_buffer() {
    // Полностью очищаем дерево от добавленных элементов
    this->clear();
}

//---------------------------------------------------------------------

template <typename T>
typename avl_tree_buffer<T>::_element_t_* avl_tree_buffer<T>::append(_value_t_* value_) {
    //(?) Если передан ненулевой указатель на значение и при этом в дереве имеются свободные ячейки для записи,
    // добавляем новое значение в дерево
    if ((value_) && (this->count_ < this->length_)) {
        uint8_t byte_control_ = 0x00;
        _element_t_* parent_element_ = this->find_parent_element(value_,byte_control_);
        _element_t_* current_element_ = nullptr;

        //(?) Если avl-дерево еще не инициализировано, добавляем первый элемент,...
        if (!parent_element_) {
            this->main_element_ = this->elements_[this->count_];
            this->main_element_->value_ = value_;
            current_element_ = this->main_element_;

            this->first_element_ = this->main_element_;
            this->last_element_ = this->main_element_;
        } else {    //... иначе добавляем новый элемент в дерево
            current_element_ = this->elements_[this->count_];
            current_element_->parent_element_ = parent_element_;

            //(?) В зависимости от байта управления поиска определеяем соответствующие указатели
            if (byte_control_ == 0x01) {
                parent_element_->left_element_ = current_element_;
                current_element_->value_ = value_;

                // Определяем указатели двусвязного списка
                this->insert_to_list(parent_element_,current_element_,true);
            } else if (byte_control_ == 0x02) {
                parent_element_->right_element_ = current_element_;
                current_element_->value_ = value_;

                // Определяем указатели двусвязного списка
                this->insert_to_list(parent_element_,current_element_);
            }

            // балансируем дерево начиная с родительского элемента
            this->balance_tree(parent_element_);
        }

        // Увеличиваем счетчик добавленных элементов
        this->count_++;

        // Возвращаем указатель на новый элемент дерева
        return current_element_;
    }

    // в противном случае возвращаем пустой указатель
    return nullptr;
}

//---------------------------------------------------------------------

template <typename T>
uint8_t avl_tree_buffer<T>::remove(_element_t_* element_) {
    //(?) Если указатель на элемент дерева определен, удаляем его
    if (element_) {
        _element_t_* left_element_ = element_->left_element_;
        _element_t_* right_element_ = element_->right_element_;
        _element_t_* parent_element_ = element_->parent_element_;

        // TODO: дореализовать
        // Последний добавленный элемент при удалении в текущей реализации должен занимать место удаляемого
        // Это необходимо для более простого способа получения элемента из массива
    }

    // В противном случае возвращаем ошибку
    return 0x01;
}

//---------------------------------------------------------------------

template <typename T>
typename avl_tree_buffer<T>::_element_t_* avl_tree_buffer<T>::find_parent_element(_value_t_* value_, uint8_t& b_) {
    _element_t_* parent_element_ = nullptr;
    _element_t_* current_element_ = this->main_element_;

    //(?) Если в дереве имеются элементы, определяем для указанного значения родительский элемент
    while (current_element_) {
        parent_element_ = current_element_;
        if ((*current_element_->value_) > (*value_)) {
            current_element_ = current_element_->left_element_;
            b_ = 0x01;
        } else {
            current_element_ = current_element_->right_element_;
            b_ = 0x02;
        }
    }

    // Возвращаем найденный родительский элемент или nullptr (дерево не инициализировано)
    return parent_element_;
}

//---------------------------------------------------------------------

template <typename T>
void avl_tree_buffer<T>::balance_tree(_element_t_* element_) {
    //(?) Выполняем балансировку, если передан ненулевой указатель на элемент дерева
    if (element_) {
        _element_t_* parent_element_ = element_;
        _element_t_* current_element_ = nullptr;

        while (parent_element_) {
            current_element_ = parent_element_;
            parent_element_ = parent_element_->parent_element_;
            if (parent_element_) {
                if (parent_element_->left_element_ == current_element_) {
                    parent_element_->left_element_ = this->balance_element(current_element_);
                } else {
                    parent_element_->right_element_ = this->balance_element(current_element_);
                }
                continue;
            }
            this->main_element_ = this->balance_element(current_element_);
        }

        this->main_element_->parent_element_ = nullptr;
    }
}

//---------------------------------------------------------------------

template <typename T>
typename avl_tree_buffer<T>::_element_t_* avl_tree_buffer<T>::balance_element(_element_t_* element_) {
    // Корректируем значение height_ для указанного элемента дерева
    this->fix_height(element_);

    //(?) Если для указанного элемента дерева наблюдается расбалансировка, корректируем его путем поворотов...
    if (this->b_factor(element_) == 2) {
        if (this->b_factor(element_->right_element_) < 0) {
            element_->right_element_ = this->rotate_right(element_->right_element_);
        }
        return this->rotate_left(element_);
    }

    if (this->b_factor(element_) == -2) {
        if (this->b_factor(element_->left_element_) > 0) {
            element_->left_element_ = this->rotate_left(element_->left_element_);
        }
        return this->rotate_right(element_);
    }

    return element_;
}

//---------------------------------------------------------------------

template <typename T>
typename avl_tree_buffer<T>::_element_t_* avl_tree_buffer<T>::rotate_right(_element_t_* element_) {
    // Осуществляем правый поворот вокруг указанного элемента дерева
    _element_t_* left_element_ = element_->left_element_;

    //(?) Устанавливаем указатель на родительский элемент для правого элемента левого элемента на
    // текущий (указанный) элемент
    if (left_element_->right_element_) {
        left_element_->right_element_->parent_element_ = element_;
    }

    element_->left_element_ = left_element_->right_element_;

    // Устанавливаем соответствующие указатели на родительские элементы
    left_element_->parent_element_ = element_->parent_element_;
    element_->parent_element_ = left_element_;

    left_element_->right_element_ = element_;

    // Восстанавливаем корректное значение поля height_ для element_ и left_element_
    this->fix_height(element_);
    this->fix_height(left_element_);

    // Возвращаем указатель на новую вершину (элемент дерева) поддерева
    return left_element_;
}

//---------------------------------------------------------------------

template <typename T>
typename avl_tree_buffer<T>::_element_t_* avl_tree_buffer<T>::rotate_left(_element_t_* element_) {
    // Осуществляем левый поворот вокруг указанного элемента дерева
    _element_t_* right_element_ = element_->right_element_;

    //(?) Устанавливаем указатель на родительский элемент для левого элемента правого элемента на
    // текущий (указанный) элемент
    if (right_element_->left_element_) {
        right_element_->left_element_->parent_element_ = element_;
    }

    element_->right_element_ = right_element_->left_element_;

    // Устанавливаем соответствующие указатели на родительские элементы
    right_element_->parent_element_ = element_->parent_element_;
    element_->parent_element_ = right_element_;

    right_element_->left_element_ = element_;

    // Восстанавливаем корректное значение поля height_ для element_ и right_element_
    this->fix_height(element_);
    this->fix_height(right_element_);

    // Возвращаем указатель на новую вершину (элемент дерева) поддерева
    return right_element_;
}

//---------------------------------------------------------------------

template <typename T>
void avl_tree_buffer<T>::insert_to_list(_element_t_* parent_, _element_t_* element_, bool left_ ) {
    //(?) Добавляем новый элемент в список относительно родительского элемента слева, если выставлен соответствующий
    // признак
    if (left_) {
        element_->set_prev_element(parent_->prev_element());
        if (parent_->prev_element()) {
            parent_->prev_element()->element()->set_next_element(element_);
        } else {
            this->first_element_ = element_;
        }

        element_->set_next_element(parent_);
        parent_->set_prev_element(element_);

        return;
    }

    //... иначе добавляем справа
    element_->set_next_element(parent_->next_element());
    if (parent_->next_element()) {
        parent_->next_element()->element()->set_prev_element(element_);
    } else {
        this->last_element_ = element_;
    }

    element_->set_prev_element(parent_);
    parent_->set_next_element(element_);
}

//---------------------------------------------------------------------

template <typename T>
void avl_tree_buffer<T>::init() { }

//---------------------------------------------------------------------

template <typename T>
void avl_tree_buffer<T>::reset(uint32_t l_) {
    // Выделяем новый блок памяти под элементы
    this->length_ = l_;
    this->elements_.reset(this->length_);

    // Сбрасываем количество записанных элементов
    this->count_ = 0;

    this->first_element_ = nullptr;
    this->last_element_ = nullptr;

    // Инициализируем список
    this->init();
}

//---------------------------------------------------------------------

template <typename T>
void avl_tree_buffer<T>::clear() {
    // Очищаем массив элементов дерева
    this->elements_.reset();

    // Устанавливаем указатели на элементы как nullptr
    this->main_element_ = nullptr;
    this->first_element_ = nullptr;
    this->last_element_ = nullptr;

    this->count_ = 0;
    this->length_ = 0;
}
}       /// <--- container
}   /// <--- mps

#endif
