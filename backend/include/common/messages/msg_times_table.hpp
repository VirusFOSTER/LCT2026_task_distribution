#ifndef TASK_DISTRIBUTION_TIME_TABLE_MESSAGE_HPP
#define TASK_DISTRIBUTION_TIME_TABLE_MESSAGE_HPP

#include <mps/mps_containers/list/list_buffer.hpp>
#include <mps/mps_common/utils/json_io/json.hpp>

namespace td {
namespace msg {
/**-----------------------------------------------------------------------------------------------------------------------
 * @brief The msg_time_table class - временная таблица путей (описание сообщения)
 * Данное сообщение содержит информацию о том, сколько времени будет затрачено от точки A до точки B в виде таблицы
 * Данная таблица может корректироваться в соответствие с текущей дорожной ситуацией
 * Важно! При больших объемах данных (несколько тысяч задач) хранение данной таблицы может привести к полному заполнению ОЗУ
 * В этом случае имеет смысл обращаться к временной таблице через запросы вбазу данных
 ----------------------------------------------------------------------------------------------------------------------*/
class msg_time_table {
    using _matrix_t_ = mps::container::list_buffer<float>;
    using _matricies_t_ = std::vector<_matrix_t_*>;

public:
    /**
     * @brief msg_time_table - конструктор
     */
    explicit msg_time_table() = default;

    /**
     * @brief operator = - оператор считывания таблицы из файла формата json
     * @param t_ - указатель на массив времен путей
     * @return ссылка на заполненную таблицу
     */
    msg_time_table& operator=(const mps::json::array::JsonArray* t_);

    /**
     * деструктор
     */
    ~msg_time_table() {
        //(?) Очиащем сообщение как элемент буфера
        if (this->matricies_.size()) {
            this->matricies_.clear();
        }
    }

    /**
     * @brief size - получение количества временных матриц
     * @return количество временных матриц
     */
    inline uint32_t size() const { return this->matricies_.size(); }

    /**
     * @brief matrix_size - получение размера матриц (все матрицы должны быть одного размера)
     * @return
     */
    inline uint32_t matrix_size() const { return (this->matricies_.size()) ? this->matricies_[0]->length() : 0; }

    /**
     * @brief value - метод получения времени пути по индексу
     * @param midx_ - идентификатор матрицы
     * @param idx_ - индекс элемента в таблице
     * @return значение в таблице
     */
    inline float value(uint32_t midx_, uint32_t idx_) const {
        return (midx_ < this->size()) ? (idx_ < this->matrix_size()) ? *(*this->matricies_[midx_])[idx_] : -1.0f : -1.0f;
    }

private:
    _matricies_t_ matricies_ = {};          /// <--- массив временных матриц путей
};
}       /// <--- msg
}   /// <--- td

#endif
