#ifndef SYSTEM_PROCESS_RUNNER_HPP
#define SYSTEM_PROCESS_RUNNER_HPP

#include "task_processor/task_processor.hpp"
#include "process_configuration/process_configuration.hpp"

namespace mps {
namespace process {
/** ---------------------------------------------------------------------------------------------------------------------
 * @brief The process_runner class - головной класс запуска процесса системы
 * В основу запуска  провесса системы заложен следующий алгоритм:
 *  1) регистрация компонент системы (выполняется на уровне кода)
 *  2) чтение конфигурации процесса системы (выполняется в случае, когда конфигурация валидна.
 *  В противном случае запуск процесса невозможен)
 *  3) инициализация активных компонент системы (компонента должна в обязательном порядке обладать методом инициализации init)
 *  4) запуск всех активных компонент процесса системы (компонента в обязательном порядке должна обладать методом run.
 *  При этом комопнента должна быть инициализирована) в отдельных потоках
 ------------------------------------------------------------------------------------------------------------------------*/
class process_runner:
                       private task::task_processor,
                       private mps::config::process_configuration {
public:
    /**
     * @brief process_runner - конструктор
     */
    explicit process_runner(const std::string& proc_name_, const std::string& path_cfg_);

    /**
     * Деструктор
     */
    ~process_runner() = default;

    /**
     * @brief configuration - получение конфигурации процесса системы (локальной)
     * Данная конфигурация используется только для компонент
     * @return полный путь к файлу конфигурации процесса системы (локальной)
     */
    inline std::string configuration() const { return this->local_configuration_; }

    /**
     * @brief start - метод запуска процесса системы
     */
    void start_process();

private:
    /**
     * @brief init_process - инициализация процесса системы
     * На базе конфигурации формируются буферы хранения сообщений и интерфейсы взаимодействия
     * @return результат инициализации процесса
     */
    bool init_process();

    /**
     * @brief init_process_components - инициализация компонент процесса системы
     * Компоненты добавляются в контейнер для выполнения задач. Сами компоненты при этом не инициализируются
     * @return результат инициализации компонент процесса системы
     */
    bool init_process_components();

    /**
     * @brief init_process_messages - инициализация сообщений процесса системы
     * @return результат инициализации сообщений процесса системы
     */
    bool init_process_messages();
};
}       /// <--- process
}   /// <--- sys


///(!) Предварительное объявление общего экземпляра класса для запуска процесса системы
extern boost::shared_ptr<mps::process::process_runner> process_runner_;

#endif
