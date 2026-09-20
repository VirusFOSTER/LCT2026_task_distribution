#ifndef SYSTEM_PROCESS_COMPONENTS_CONTAINER_HPP
#define SYSTEM_PROCESS_COMPONENTS_CONTAINER_HPP

#include "components_traits/base_component/base_functional_component.hpp"
#include "components_traits/base_component/qbase_functional_component.hpp"

namespace mps {
namespace process {
namespace container {
namespace components {
/** ----------------------------------------------------------------------------------------------------------------------
 * @brief The components_container class - класс, содержащий все зарегистрированные компоненты
 * Те компоненты, которые зарегистрированы, но при этом не указаны в конфигурации или указаны как неактивные
 * будут удалены
 ------------------------------------------------------------------------------------------------------------------------*/
class register_components_container {
public:
    using component_t = boost::shared_ptr<process::component::base::base_functional_component>;
    using qcomponent_t = boost::shared_ptr<process::component::base::qbase_functional_component>;

private:
    /** -----------------------------------------------------------------------------
     * @brief The list_component class - список зарегистрированных компонент системы
     -------------------------------------------------------------------------------*/
    struct list_component {
        component_t component_ = nullptr;           /// <--- зарегистрированная компонента процесса системы

        list_component* next_component_ = nullptr;  /// <--- указатель на следующую зарегистрированную комопненту
        list_component* prev_component_ = nullptr;  /// <--- указатель на предыдущую зарегистрированную компоненту
    };

    /** -----------------------------------------------------------------------------
     * @brief The list_component class - список зарегистрированных компонент системы
     -------------------------------------------------------------------------------*/
    struct list_qcomponent {
        qcomponent_t component_ = nullptr;          /// <--- зарегистрированная компонента процесса системы

        list_qcomponent* next_component_ = nullptr; /// <--- указатель на следующую зарегистрированную компоненту
        list_qcomponent* prev_component_ = nullptr; /// <--- указатель на предыдущую зарегистрированную компоненту
    };

public:
    /** Конструктор */
    explicit register_components_container();

    /** Деструктор */
    ~register_components_container() = default;

    ///----------BASE FUNCTIONAL COMPONENT---------------
    /**
     * @brief component_registration - регистрация компоненты процесса системы
     * @param component_ - указатель на регистрируемую компоненту
     * @param component_name_ - уникальное наименование компоненты
     * @return результат регистрации новой компонены
     */
    bool component_registration(component_t component_, const std::string& component_name_);

    /**
     * @brief get_component - получение указателя на компоненту процесса системы по уникальному наименованию
     * @param component_name_ - уникальное наименование компоненты процесса системы
     * @return указатель на компоненту процесса системы (nullptr - если отсутствует в контейнере)
     */
    component_t get_component(const std::string& component_name_);

    /**
     * @brief get_component - получение указателя на компоненту процесса системы по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненту процесса системы
     * @return указатель на компоненту процесса системы (nullptr - если отсутствует в контейнере)
     */
    component_t get_component(const uint16_t& component_uid_);

    /**
     * @brief get_component_name - получение наименования компоненты по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненты процесса системы
     * @return уникальное наименование компоненты
     */
    std::string get_component_name(const uint16_t component_uid_);

    /**
     * @brief remove_component - удаление компоненты из контейнера по уникальному наименованию
     * @param component_name - уникальное наименование компоненты процесса системы
     */
    void remove_component(const std::string component_name_);

    /**
     * @brief remove_component - удаление компоненты из контейнера по уникальному идентификатору
     * @param component_uid_ - уникальный идентификатор компоненты процесса системы
     */
    void remove_component(const uint16_t& component_uid_);


    ///----------QBASE FUNCTIONAL COMPONENT---------------
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
     * @brief remove_component - удаление компоненты из контейнера по указателю
     * @param ptr_component_ - указатель на компонент в списке
     */
    void remove_component(list_component* ptr_component_);

    /**
     * @brief remove_qcomponent - удаление компоненты из контейнера по указателю
     * @param ptr_component_ - указатель на компонент в списке
     */
    void remove_qcomponent(list_qcomponent* ptr_component_);

private:
    list_component* components_list_ = nullptr;     /// <--- указатель на список компонент (base_functional_component)
    list_qcomponent* qcomponents_list_ = nullptr;   /// <--- указатель на список компонент (qbase_functional_component)
};
}               /// <--- components
}           /// <--- container
}       /// <--- process
}   /// <--- mps


extern boost::shared_ptr<mps::process::container::components::register_components_container> register_components_container_;

#endif
