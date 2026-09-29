#include "components/task_distribution/task_distribution.hpp"


using namespace td;
using namespace component;

// Профили транспорта
const std::string PROFILE_CAR     = "car";
const std::string PROFILE_BIKE    = "bike";
const std::string PROFILE_FOOT    = "foot";
const std::string PROFILE_TRANSIT = "transit";

/**
 * @brief print_summary_table - вывод общей информации в консоль
 * @param sol - полученное решение задачи
 */
void print_summary_table(const vroom::Solution& sol) {
    // Считаем статистику
    struct Row {
        int vehicle;
        std::string profile;
        int jobs;
        int duration_sec;
        int service_sec;
        int distance_m;
        int finish_time;   // время завершения последней задачи
    };
    std::vector<Row> rows;

    for (const auto& r : sol.routes) {
        int jobs = 0;
        int last_arrival = 0;
        for (const auto& s : r.steps) {
            if (s.step_type == vroom::STEP_TYPE::JOB) {
                ++jobs;
                last_arrival = s.arrival + s.service;
            }
        }
        if (jobs == 0) continue;

        rows.push_back({
            static_cast<int>(r.vehicle),
            r.profile,
            jobs,
            static_cast<int>(r.duration),
            static_cast<int>(r.service),
            static_cast<int>(r.distance),
            last_arrival
        });
    }

    // Заголовок
    const int W = 12;
    auto line = [&]() {
        std::cout << '\033[32m+' << std::string(W, '-') << '+'
                  << std::string(W, '-') << '+'
                  << std::string(W, '-') << '+'
                  << std::string(W, '-') << '+'
                  << std::string(W, '-') << '+'
                  << std::string(W, '-') << '+'
                  << std::string(W, '-') << "+\n";
    };

    line();
    std::cout << '|' << std::setw(W) << "Рабочий"
              << '|' << std::setw(W) << "Профиль"
              << '|' << std::setw(W) << "Задач"
              << '|' << std::setw(W) << "Время,мин"
              << '|' << std::setw(W) << "Сервис,мин"
              << '|' << std::setw(W) << "Пробег,км"
              << '|' << std::setw(W) << "Финиш"
              << "|\n";
    line();

    auto fmt_time = [](int sec) {
        int h = sec / 3600;
        int m = (sec % 3600) / 60;
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
        return std::string(buf);
    };

    for (const auto& r : rows) {
        std::cout << '|' << std::setw(W) << r.vehicle
                  << '|' << std::setw(W) << r.profile
                  << '|' << std::setw(W) << r.jobs
                  << '|' << std::setw(W) << r.duration_sec / 60
                  << '|' << std::setw(W) << r.service_sec / 60
                  << '|' << std::setw(W) << std::fixed << std::setprecision(1)
                         << r.distance_m / 1000.0
                  << '|' << std::setw(W) << fmt_time(r.finish_time)
                  << "|\n";
    }
    line();

    // Итоги
    int total_jobs = 0, total_dist = 0;
    for (const auto& r : rows) {
        total_jobs += r.jobs;
        total_dist += r.distance_m;
    }
    std::cout << "Использовано рабочих: " << rows.size()
              << " | Задач: " << total_jobs
              << " | Суммарный пробег: "
              << std::fixed << std::setprecision(1)
              << total_dist / 1000.0 << " км\n\n\033[0m";
}

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

    //(?>) Формируем временные матрицы путей
    std::vector<std::pair<std::string,vroom::Matrix<vroom::UserDuration>>> matricies_ = {};
    for (uint32_t i = 0; i < time_table_->size(); ++i) {
        vroom::Matrix<vroom::UserDuration> matrix_(time_table_->matrix_size());
        for (uint32_t j = 0; j < time_table_->matrix_size(); ++j) {
            for (uint32_t k = 0; k < time_table_->matrix_size(); ++k) {
                matrix_[i][j] = time_table_->value(i, j * time_table_->matrix_size() + k);
            }
        }
        matricies_.push_back(std::make_pair(time_table_->profile(i),matrix_));
    }
    vroom_.set_moving_maxtricies(matricies_);

    // Составляем описание задач и исполнителей для фреймворка vroom
    auto vehicles_ = this->make_vehicles(tasks_, instances_, time_table_);
    auto jobs_ = this->make_jobs(tasks_,instances_);

    vroom_.set_tasks(jobs_);
    vroom_.set_instances(vehicles_);

    // Выполняем задачу о назначениях
    auto solution_ = vroom_.solve_problem();
    print_summary_table(solution_);
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

std::vector<vroom::Job> fc_task_distribution::make_jobs(const msg::msg_tasks_list* const tasks_,
                                                        const msg::msg_list_instances* const instances_) {
    std::vector<vroom::Job> jobs_ = {};
    jobs_.reserve(tasks_->tasks_count());

    for (uint32_t i = 0; i < tasks_->tasks_count(); ++i) {
        auto c_task_ = tasks_->get_task(i);

        // Указываем положение задачи на карте
        vroom::Location task_location_(instances_->instance_count() + i);

        vroom::UserDuration task_service_ = this->define_service(c_task_->task_type());
        vroom::Skills require_skills_;
        require_skills_.insert((uint32_t)c_task_->task_type());

        // Формируем описание задачи в формате vroom-фреймворка
        vroom::Job job_(
                    c_task_->task_uid(),
                    task_location_,
                    0,
                    task_service_,
                    vroom::Amount(0),
                    vroom::Amount(0),
                    require_skills_,
                    this->define_priority(c_task_->task_type()),
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

vroom::UserDuration fc_task_distribution::define_service(types::tg_task tp_) {
    switch (tp_) {
    case types::tg_task::_tg_emergency_: { return 80 * 60; } break;
    case types::tg_task::_tg_connection_: { return 60 * 60; } break;
    case types::tg_task::_tg_additional_order_: { return 10 * 60; } break;
    case types::tg_task::_tg_local_task_: { return 30 * 60; } break;
    case types::tg_task::_tg_unknown_: { return 0; }
    }

    return 0;
}

uint32_t fc_task_distribution::define_priority(types::tg_task tp_) {
    switch (tp_) {
    case types::tg_task::_tg_emergency_: { return 100; } break;
    case types::tg_task::_tg_connection_: { return 30; } break;
    case types::tg_task::_tg_additional_order_: { return 10; } break;
    case types::tg_task::_tg_local_task_: { return 10; } break;
    case types::tg_task::_tg_unknown_: { return 0; }
    }

    return 0;
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

//-----------------------------------------------------------------------------------

void fc_task_distribution::log_solution(const vroom::Solution& sol) {
    std::cout << "=== СВОДКА ===\n";
    std::cout << "Общая стоимость:       " << sol.summary.cost << "\n";
    std::cout << "Не выполнено задач:    " << sol.unassigned.size() << "\n";

    if (!sol.unassigned.empty()) {
        std::cout << "ID невыполненных задач: ";
        for (const auto& j : sol.unassigned) {
            std::cout << j.id << " ";
        }
        std::cout << "\n";
    }

    // Считаем, сколько машин реально использовано
    int used = 0;
    for (const auto& r : sol.routes) {
        for (const auto& s : r.steps) {
            if (s.step_type == vroom::STEP_TYPE::JOB) { ++used; break; }
        }
    }
    std::cout << "Использовано рабочих:  " << used << "\n";

    std::cout << "\n=== МАРШРУТЫ ===\n";
    for (const auto& route : sol.routes) {
        // Пропускаем пустые маршруты
        int job_count = 0;
        for (const auto& s : route.steps)
            if (s.step_type == vroom::STEP_TYPE::JOB) ++job_count;
        if (job_count == 0) continue;

        std::cout << "\nРабочий " << route.vehicle
                  << " | задач: " << job_count
                  << " | стоимость: " << route.cost
                  << " | длительность: " << route.duration / 60 << " мин"
                  << " | сервис: " << route.service / 60 << " мин\n";

        for (const auto& step : route.steps) {
            std::string type;
            switch (step.step_type) {
            case vroom::STEP_TYPE::START: type = "СТАРТ  "; break;
            case vroom::STEP_TYPE::END:   type = "КОНЕЦ  "; break;
            case vroom::STEP_TYPE::JOB:   type = "ЗАДАЧА "; break;
            case vroom::STEP_TYPE::BREAK:
                break;
            }

            int h = step.arrival / 3600;
            int m = (step.arrival % 3600) / 60;
        }
    }
}




//------------------------------------------------------------------------------------------------------------------------
//--------------------------------- COMPONENT REGISTRATION ----------------------------------
//------------------------------------------------------------------------------------------------------------------------
#include <mps/mps_process_traits/components_traits/components_container/components_container.hpp>

static bool task_distribution_registration() {
    //(?) Если контейнер для регистрации компонент инициализирован, регистрируем компоненту task_distribution
    if (register_components_container_) {
        boost::shared_ptr<mps::process::component::base::base_functional_component> task_distribution_(
                    new td::component::fc_task_distribution("task_distribution"));
        return register_components_container_->component_registration(task_distribution_,"task_distribution");
    }

    return false;
}

static bool task_distribution_registration_ = task_distribution_registration();
