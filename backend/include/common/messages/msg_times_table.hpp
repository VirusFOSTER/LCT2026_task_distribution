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
    ~msg_time_table() = default;

    /**
     * @brief size - получение размера таблицы
     * @return размер таблицы
     */
    inline uint32_t size() const {
        return this->table_?
                   this->table_->count() > this->table_->length() ?
                       this->table_->length() : this->table_->count() :
                   0; }

    /**
     * @brief operator [] - получение значения из таблицы по индексу
     * @param idx_ - индекс значения
     * @return длительность пути
     */
    inline float operator[](uint32_t idx_) {
        return this->table_ ?
                   (this->table_->length() >= idx_) ? *(*this->table_)[idx_] : 0 :
                   0;
    }

private:
    mps::container::list_buffer<float>* table_ = nullptr;       /// <--- описание таблицы
};
}       /// <--- msg
}   /// <--- td

#endif
