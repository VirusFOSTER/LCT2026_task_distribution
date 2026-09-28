#include "components/task_distribution/task_distribution.hpp"


using namespace td;
using namespace component;

// Профили транспорта
const std::string PROFILE_CAR     = "car";
const std::string PROFILE_BIKE    = "bike";
const std::string PROFILE_FOOT    = "foot";
const std::string PROFILE_TRANSIT = "transit";

//-----------------------------------------------------------------------------------

fc_task_distribution::fc_task_distribution(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) { }

//-----------------------------------------------------------------------------------

fc_task_distribution::~fc_task_distribution() { this->reset(); }

//-----------------------------------------------------------------------------------

bool fc_task_distribution::init() {
    // Получаем указатель на интерфейс читателя сообщений типа list_instances
    this->ireader_instance_ = this->interface_reader<td::msg::msg_list_instances>("list_instances");
    if (!this->ireader_instance_) {
        std::cout << "Fail! Pointer to reader_interface \'list_instances\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс читателя сообщений типа time_table
    this->ireader_time_ = this->interface_reader<td::msg::msg_time_table>("time_table");
    if (!this->ireader_time_) {
        std::cout << "Fail! Pointer to reader_interface \'time_table\' is not defined!";
        this->reset();
        return false;
    }

    this->ireader_tasks_ = this->interface_reader<td::msg::msg_tasks_list>("list_tasks");
    if (!this->ireader_tasks_) {
        std::cout << "Fail! Pointer to reader_interface \'list_tasks\' is not defined!";
        this->reset();
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    return true;
}

//-----------------------------------------------------------------------------------

void fc_task_distribution::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // Получаем сообщение со списком задач на исполнение
        auto tasks_list_ = this->ireader_tasks_->read_next_element();

        //(?) Если сообщение пришло,...
        if (tasks_list_) {
            //(?>) Ожидаем список исполнителей
            while (1) {
                auto instances_list_ = this->ireader_instance_->read_next_element();

                //(?) Если список исполнителей пришел,...
                if (instances_list_) {
                    //(?>) Ожидаем временную диаграмму путей движения
                    while (1) {
                        auto moving_time_ = this->ireader_time_->read_next_element();
                        if (moving_time_) {
                            // Решаем задачу VRP
                            this->make_solve_problem(tasks_list_->message_,
                                                     instances_list_->message_,
                                                     moving_time_->message_);

                            // Освобождаем элемент буфера хранения сообщений типа time_table
                            this->ireader_time_->remove_element(&moving_time_);

                            break;
                        }

                        // Засыпаем на 10 микросекунд
                        usleep(10);
                    }

                    // Освобождаем элемент буфера хранения сообщений типа list_instances
                    this->ireader_instance_->remove_element(&instances_list_);

                    break;
                }

                // Засыпаем на 10 микросекунд
                usleep(10);
            }

            // Освобождаем элемент буфера хранения сообщений list_tasks
            this->ireader_tasks_->remove_element(&tasks_list_);
        }

        // Засыпаем на 10 микросекунд
        usleep(10);
    }
}

//-----------------------------------------------------------------------------------

void fc_task_distribution::make_solve_problem(const msg::msg_tasks_list* const tasks_,
                                              const msg::msg_list_instances* const instances_,
                                              const msg::msg_time_table* const time_table_) {
    // Рабочее время (полный день)
    const vroom::TimeWindow shift_(SHIFT_START, SHIFT_END);

    // Инициализируем проблему
    algorithms::vroom_problem_solver vroom_(shift_);
    vroom_.init_problem();

    // Составляем описание задач и исполнителей для фреймворка vroom
    auto vehicles_ = this->make_vehicles(tasks_, instances_, time_table_);
    auto jobs_ = this->make_jobs(tasks_);

    vroom_.set_tasks(jobs_);
    vroom_.set_instances(vehicles_);

    // Выполняем задачу о назначениях
    vroom_.solve_problem();
}

//-----------------------------------------------------------------------------------

std::vector<vroom::Vehicle> fc_task_distribution::make_vehicles(const msg::msg_tasks_list * const tasks_,
                                                                const msg::msg_list_instances* const instances_,
                                                                const msg::msg_time_table * const time_table_) {
    // Рабочее время (полный день)
    const vroom::TimeWindow shift_(SHIFT_START, SHIFT_END);

    // Фиксированная стоимость, чтобы минимизировать число задействованных ТС
    vroom::VehicleCosts costs_(
        1'000'000,   // fixed
        3600,        // per_hour
        0,           // per_km
        0            // per_task_hour
        );

    //(?>) Формируем описание исполнителей задач в формате vroom-фреймворка
    std::vector<vroom::Vehicle> vehicles_ = {};
    vehicles_.reserve(instances_->instance_count());

    for (uint32_t i = 0; i < instances_->instance_count(); ++i) {
        auto c_instance_ = instances_->get_instance(i);

        // Определяем профиль исполнителя задач
        auto profile_ = this->define_profile(c_instance_->moving_tag());

        // Указываем текущее положение исполнителя задач
        // Если начало рабочего дня, то это дом
        // Если поступает какая-то срочная заявка или происходит отмена, то это текущая выполняемая задача
        vroom::Location start_position_(time_table_->value(profile_, i * tasks_->tasks_count() + i));

        // Устанавливаем точку старта и точку окончания маршрута
        std::optional<vroom::Location> start_location_(start_position_);
        std::optional<vroom::Location> end_location_;

        //(?>) Указываем компетентность исполнителя
        vroom::Skills skills_;
        auto competence_ = c_instance_->competence();
        for (uint8_t i = 0; i < 8; ++i) {
            if (competence_ & (1u << i)) {
                skills_.insert(i);
            }
        }

        // Формируем описание исоплнителя задачи в тип vroom-фреймворка
        vroom::Vehicle vehicle_(
            i + 1,
            start_location_,
            end_location_,
            profile_,
            vroom::Amount(0),
            skills_,
            shift_,
            {},
            "",
            costs_
            );

        // Добавляем исполнителя в массив
        vehicles_.emplace_back(vehicle_);
    }

    return vehicles_;
}

//-----------------------------------------------------------------------------------

std::vector<vroom::Job> fc_task_distribution::make_jobs(const msg::msg_tasks_list* const tasks_) {
    std::vector<vroom::Job> jobs_ = {};
    jobs_.reserve(tasks_->tasks_count());

    for (uint32_t i = 0; i < tasks_->tasks_count(); ++i) {
        auto c_task_ = tasks_->get_task(i);

        // Указываем положение задачи на карте
        vroom::Location task_location_(
            vroom::Coordinates(c_task_->task_position().longitude(),c_task_->task_position().latitude()));

        vroom::UserDuration task_service_;  // TODO: требуется заоплнение
        vroom::Skills require_skills_;      // TODO: требуется заполнение

        // Формируем описание задачи в формате vroom-фреймворка
        vroom::Job job_(
            c_task_->task_uid(),
            task_location_,
            0,
            task_service_,
            vroom::Amount(0),
            vroom::Amount(0),
            require_skills_,
            0,      // TODO: должен определяться исходя из описания
            { vroom::TimeWindow(c_task_->get_seconds_begin(), c_task_->get_seconds_end()) }
            );

        jobs_.emplace_back(job_);
    }

    // Возвращаем массив сформированных задач
    return jobs_;
}

//-----------------------------------------------------------------------------------

std::string fc_task_distribution::define_profile(types::tg_moving tg_) {
    switch (tg_) {
    case types::tg_moving::_tg_car_: { return PROFILE_CAR; }
    case types::tg_moving::_tg_bicycle_: { return PROFILE_BIKE; }
    case types::tg_moving::_tg_motorbike_: { return PROFILE_BIKE; }
    case types::tg_moving::_tg_pedestrian_: { return PROFILE_FOOT; }
    case types::tg_moving::_tg_public_transport_: { return PROFILE_TRANSIT; }
    case types::tg_moving::_tg_unknown_: { return ""; }
    };

    return "";
}

//-----------------------------------------------------------------------------------

void fc_task_distribution::reset() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->ireader_instance_) {
        delete this->ireader_instance_;
        this->ireader_instance_ = nullptr;
    }

    if (this->ireader_time_) {
        delete this->ireader_time_;
        this->ireader_time_ = nullptr;
    }

    if (this->ireader_tasks_) {
        delete this->ireader_tasks_;
        this->ireader_tasks_ = nullptr;
    }
}
