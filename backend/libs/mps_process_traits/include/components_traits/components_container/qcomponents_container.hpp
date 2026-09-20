#ifndef SYSTEM_PROCESS_QCOMPONENTS_CONTAINER_HPP
#define SYSTEM_PROCESS_QCOMPONENTS_CONTAINER_HPP

#include "components_traits/base_component/qbase_functional_component.hpp"

namespace mps {
namespace process {
namespace container {
namespace components {
/** ----------------------------------------------------------------------------------------------------------------------
 * @brief The register_qcomponents_container class - класс, содержащий все зарегистрированные компоненты
 * Те компоненты, которые зарегистрированы, но при этом не указаны в конфигурации или указаны как неактивные
 * будут удалены
 ------------------------------------------------------------------------------------------------------------------------*/
class register_qcomponents_container {
public:
    using qcomponent_t = boost::shared_ptr<process::component::base::qbase_functional_component>;

private:
    /** -----------------------------------------------------------------------------
     * @brief The list_qcomponent class - список зарегистрированных компонент системы
     -------------------------------------------------------------------------------*/
    struct list_qcomponent {
        qcomponent_t component_ = nullptr;          /// <--- зарегистрированная компонента процесса системы

        list_qcomponent* next_component_ = nullptr; /// <--- указатель на следующую зарегистрированную компоненту
        list_qcomponent* prev_component_ = nullptr; /// <--- указатель на предыдущую зарегистрированную компоненту
    };

public:
    /** Конструктор */
    explicit register_qcomponents_container();

    /** Деструктор */
    ~register_qcomponents_container() = default;

    /**
     * @brief qcomponent_registration - регистрация компоненты процесса системы
     * @param component_ - указатель на регистрируемую компоненту
     * @param component_name_ - уникальное наименование компоненты
     * @return результат регистрации новой компонены
     */
    bool component_registration(qcomponent_t component_, const std::string& component_name_);

    /**
     * @brief get_qcomponent - получение указателя на компоненту процесса системы по уникальному наименованию
     * @param component_name_ - уникальное наименование компоненты процесса системы
     * @return указатель на компоненту процесса системы (nullptr - если отсутствует в контейнере)
     */
    qcomponent_t get_qcomponent(const std::string& component_name_);

    /**
     * @brief get_qcomponent - получение указателя на компоненту процесса системы по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненту процесса системы
     * @return указатель на компоненту процесса системы (nullptr - если отсутствует в контейнере)
     */
    qcomponent_t get_qcomponent(const uint16_t& component_uid_);

    /**
     * @brief get_qcomponent_name - получение наименования компоненты по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненты процесса системы
     * @return уникальное наименование компоненты
     */
    std::string get_qcomponent_name(const uint16_t component_uid_);

    /**
     * @brief remove_qcomponent - удаление компоненты из контейнера по уникальному наименованию
     * @param component_name - уникальное наименование компоненты процесса системы
     */
    void remove_qcomponent(const std::string component_name_);

    /**
     * @brief remove_qcomponent - удаление компоненты из контейнера по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненты процесса системы
     */
    void remove_qcomponent(const uint16_t& component_uid_);

    /**
     * @brief clear - полная очистка контейнера компонентов
     */
    void clear();

private:

    /**
     * @brief remove_qcomponent - удаление компоненты из контейнера по указателю
     * @param ptr_component_ - указатель на компонент в списке
     */
    void remove_qcomponent(list_qcomponent* ptr_component_);

private:
    list_qcomponent* qcomponents_list_ = nullptr;   /// <--- указатель на список компонент (qbase_functional_component)
};
}               /// <--- components
}           /// <--- container
}       /// <--- process
}   /// <--- mps


extern boost::shared_ptr<mps::process::container::components::register_qcomponents_container> register_qcomponents_container_;

#endif
