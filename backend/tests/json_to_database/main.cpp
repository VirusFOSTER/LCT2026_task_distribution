#include <mps/mps_common/utils/json_io/json.hpp>

namespace td {
namespace types {
/** -----------------------------------------------------------------------------------------------------------------
 * @brief The tp_instance class - описание исполнителя задач
 * Каждому исполнителю назначается определенное количество задач. В результате формируется полноценный список задач
 * на исполнение.
 --------------------------------------------------------------------------------------------------------------------*/
class tp_instance {
public:
    /**
     * @brief tp_instance - конструктор
     * @param obj_cfg_ - конфигурация (описание) исполнителя задач
     */
    explicit tp_instance(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * деструктор
     */
    ~tp_instance() = default;

    inline uint16_t uid() const { return this->instance_uid_; }
    inline std::string name() const { return this->instance_name_; }

private:
    /**
     * @brief read_configuration - метод чтения конфигурации (описания) исполнителя задач
     * @param obj_cfg_ - указатель на описание исполнителя задач
     * @return результат чтения описания
     */
    bool read_configuration(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация описания исполнителя задач на валидность
     * @param obj_cfg_ - указатель на описание исполнителя задач
     * @return результат верификации
     */
    bool configuration_valid(const mps::json::object::JsonObject* obj_cfg_);

private:
    bool description_valid_ = false;        /// <--- Признак чтения описания исполнителя

    uint16_t instance_uid_ = 0;             /// <--- уникальный идентификатор исполнителя
    std::string instance_name_ = "";        /// <--- уникальное имя исполнителя
    std::string instance_region_ = "";      /// <--- регион обработки задач исполнителем
    uint8_t byte_competence_ = 0x00;        /// <--- байт компетентности
};
}       /// <--- types
}   /// <--- td

using namespace td;
using namespace types;


//-------------------------------------------------

tp_instance::tp_instance(const mps::json::object::JsonObject* obj_cfg_) {
    // ПОлучаем описание исполнителя задачи и фиксируем результат чтения
    this->description_valid_ = this->read_configuration(obj_cfg_);
}

//-------------------------------------------------
/*
    uint16_t instance_uid_ = 0;             /// <--- уникальный идентификатор исполнителя
    std::string instance_name_ = "";        /// <--- уникальное имя исполнителя
    std::string instance_region_ = "";      /// <--- регион обработки задач исполнителем
    uint8_t byte_competence_ = 0x00;        /// <--- байт компетентности
 */
bool tp_instance::read_configuration(const mps::json::object::JsonObject* obj_cfg_) {
    //(?) Если описание исполнителя задач валидно, считываем его и возвращаем результат
    if (this->configuration_valid(obj_cfg_)) {
        this->instance_uid_ = obj_cfg_->asInteger("instance_uid");
        this->instance_name_ = obj_cfg_->asString("instance_name");
        this->instance_region_ = obj_cfg_->asString("intance_region");

        auto ar_competence_ = obj_cfg_->asArray("instance_competence");
        for (int8_t i = 0; i < ar_competence_->size(); ++i) {
            if (ar_competence_->asInteger(i)) {
                this->byte_competence_ |= static_cast<uint8_t>(1u << i);
            }
        }
        // (!) Заметка: количество типов задач ограничено. Имеется допущение, что этих типов не больше чем 8

        return true;
    }

    // В противном случае возвращаем соответствующий результат
    return false;
}

//-------------------------------------------------
/*
{
    "instance_uid": 0,
    "instance_name": "name_1",
    "intance_region": "yugotsentr",
    "instance_competence":
    [
        1, 1, 1, 0
    ]
}
 */
bool tp_instance::configuration_valid(const mps::json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("instance_uid") &&
           obj_cfg_->hasProperty("instance_name") &&
           obj_cfg_->hasProperty("intance_region") &&
           obj_cfg_->hasProperty("instance_competence") &&
           obj_cfg_->asArray("instance_competence");
}


int main(int argc, char **argv) {
    mps::json::loader::JsonLoader loader_("instances_yugotsentr.json");
    mps::json::object::JsonObject* root_ = loader_.rootObject();

    auto ar_instances_ = root_->asArray("instances");
    std::cout << ar_instances_->size() << "\n";

    std::vector<td::types::tp_instance> instances_;
    instances_.reserve(ar_instances_->size());

    for (int i = 0; i < ar_instances_->size(); ++i) {
        mps::json::object::JsonObject* obj_ = ar_instances_->asObject(i);
        // std::cout << obj_->asString("instance_name") << "\n";
        instances_.emplace_back(obj_);
    }

    /*
string sql("INSERT INTO PERSON VALUES(1, 'STEVE', 'GATES', 30, 'PALO ALTO', 1000.0);"
               "INSERT INTO PERSON VALUES(2, 'BILL', 'ALLEN', 20, 'SEATTLE', 300.22);"
               "INSERT INTO PERSON VALUES(3, 'PAUL', 'JOBS', 24, 'SEATTLE', 9900.0);");
     */
    for (auto& inst_ : instances_) {
        std::string sql_ = ("INSERT INTO INSTANCES VALUES(" + std::to_string(inst_.uid()) + ","
                 + "'" + inst_.name() + "');");
        std::cout << sql_ << "\n";
    }

    return 0;
}
