#include "components/database_query/database_query.hpp"
#include <mps/mps_process_traits/interfaces_traits/message_writer_concept.hpp>

td::component::current_free_element current_free_element_;

using namespace td;
using namespace component;

/**
 * @brief callback_make_instances - парсинг данных (из базы данных) в массив исполнителей
 */
static int callback_make_instances(void* data_, int argc_, char** argv_, char** col_name_) {
    current_free_element_.count_instances_++;

    current_free_element_.instances_.push_back(new types::tp_instance);

    current_free_element_.instances_.back()->set_instance_uid(std::strtoul(argv_[0],nullptr,10));
    current_free_element_.instances_.back()->set_instance_name(argv_[1]);
    current_free_element_.instances_.back()->set_instance_region(argv_[2]);

    std::string token_(argv_[3]);

    if (token_ == "car") { current_free_element_.instances_.back()->set_moving_tag(types::tg_moving::_tg_car_); }
    else if (token_ == "bike") { current_free_element_.instances_.back()->set_moving_tag(types::tg_moving::_tg_bicycle_); }
    else if (token_ == "foot") { current_free_element_.instances_.back()->set_moving_tag(types::tg_moving::_tg_pedestrian_); }
    else if (token_ == "transit") { current_free_element_.instances_.back()->set_moving_tag(types::tg_moving::_tg_public_transport_);}

    uint32_t token_iv_ = std::strtoul(argv_[4],nullptr,10);
    switch (token_iv_) {
    case (uint32_t)types::tg_instance::_tg_status_free_: {
        current_free_element_.instances_.back()->set_tag(types::tg_instance::_tg_status_free_);
        current_free_element_.count_available_instances_++;
        current_free_element_.count_free_instances_++;
        current_free_element_.count_instances_positions_++;
    } break;
    case (uint32_t)types::tg_instance::_tg_status_work_: {
        current_free_element_.instances_.back()->set_tag(types::tg_instance::_tg_status_work_);
        current_free_element_.count_available_instances_++;
        current_free_element_.count_job_instances_++;
        current_free_element_.count_instances_positions_++;
    } break;
    case (uint32_t)types::tg_instance::_tg_status_unavailable_: {
        current_free_element_.instances_.back()->set_tag(types::tg_instance::_tg_status_unavailable_);
    } break;
    case (uint32_t)types::tg_instance::_tg_status_unknown_: {
        current_free_element_.instances_.back()->set_tag(types::tg_instance::_tg_status_unknown_);
    } break;
    };

    types::tp_position pose_;
    pose_.set_longitude(std::strtof(argv_[5],nullptr));
    pose_.set_latitude(std::strtof(argv_[6],nullptr));

    current_free_element_.instances_.back()->set_position(pose_);

    uint8_t b_ = 0x00;
    b_ |=  (std::strtol(argv_[7],nullptr,10)) ? (uint8_t)types::tg_task::_tg_emergency_ : 0x00;
    b_ |=  (std::strtol(argv_[8],nullptr,10)) ? (uint8_t)types::tg_task::_tg_connection_ : 0x00;
    b_ |=  (std::strtol(argv_[9],nullptr,10)) ? (uint8_t)types::tg_task::_tg_local_task_ : 0x00;
    b_ |=  (std::strtol(argv_[10],nullptr,10)) ? (uint8_t)types::tg_task::_tg_additional_order_ : 0x00;

    current_free_element_.instances_.back()->set_competence(b_);

    return 0;
}

/**
 * @brief callback_make_tasks - парсинг данных (из базы данных) в массив задач
 */
static int callback_make_tasks(void* data_, int argc_, char** argv_, char** col_name_) {
    current_free_element_.tasks_.push_back(new types::tp_task);

    current_free_element_.tasks_.back()->set_task_uid(std::strtoul(argv_[0],nullptr,10));
    current_free_element_.tasks_.back()->set_time_create(argv_[1]);

    uint8_t status_ = std::strtol(argv_[3],nullptr,10);
    switch (status_) {
    case (uint8_t)types::tg_status::_tg_free_: {
        current_free_element_.tasks_.back()->set_task_status(types::tg_status::_tg_free_);
        current_free_element_.count_free_tasks_++;
    } break;
    case (uint8_t)types::tg_status::_tg_work_: {
        current_free_element_.tasks_.back()->set_task_status(types::tg_status::_tg_work_);
        current_free_element_.count_job_tasks_++;
    } break;
    case (uint8_t)types::tg_status::_tg_completed_: {
        current_free_element_.tasks_.back()->set_task_status(types::tg_status::_tg_completed_);
        current_free_element_.count_completed_tasks_++;
    } break;
    };

    uint8_t tp_ = std::strtol(argv_[2],nullptr,10);
    switch (tp_) {
    case (uint8_t)types::tg_task::_tg_emergency_: {
        current_free_element_.tasks_.back()->set_task_type(types::tg_task::_tg_emergency_);
    } break;
    case (uint8_t)types::tg_task::_tg_connection_: {
        current_free_element_.tasks_.back()->set_task_type(types::tg_task::_tg_connection_);
    } break;
    case (uint8_t)types::tg_task::_tg_local_task_: {
        current_free_element_.tasks_.back()->set_task_type(types::tg_task::_tg_local_task_);
    } break;
    case (uint8_t)types::tg_task::_tg_additional_order_: {
        current_free_element_.tasks_.back()->set_task_type(types::tg_task::_tg_additional_order_);
    } break;
    case (uint8_t)types::tg_task::_tg_unknown_: {
        current_free_element_.tasks_.back()->set_task_type(types::tg_task::_tg_unknown_);
    } break;
    };

    current_free_element_.tasks_.back()->set_region(argv_[4]);

    types::tp_position pose_;
    pose_.set_longitude(std::strtof(argv_[5],nullptr));
    pose_.set_latitude(std::strtof(argv_[6],nullptr));

    current_free_element_.tasks_.back()->set_postion(pose_);

    types::tp_time_window wind_;
    wind_.set_time_begin(argv_[7]);
    wind_.set_time_end(argv_[8]);

    current_free_element_.tasks_.back()->set_time_window(wind_);
}

//-----------------------------------------------------------------------------------

fc_database_query::fc_database_query(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) { }

//-----------------------------------------------------------------------------------

fc_database_query::~fc_database_query() { this->reset(); }

//-----------------------------------------------------------------------------------

bool fc_database_query::init() {
    // Получаем указатель на интерфейс читателя сообщений типа user_request
    this->ireader_request_ = this->interface_reader<msg::msg_request>("user_request");
    if (!this->ireader_request_) {
        std::cout << "Fail! Pointer to reader_interface \'user_request\' is not defined!";
        this->reset();
        return false;
    }

    this->ireader_tasks_ = this->interface_reader<msg::msg_tasks_list>("list_tasks");
    if (!this->iwriter_instances_) {
        std::cout << "Fail! Pointer to reader_interface \'list_tasks\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа list_instances
    this->iwriter_instances_ = this->interface_writer<msg::msg_list_instances>("list_instances");
    if (!this->iwriter_instances_) {
        std::cout << "Fail! Pointer to writer_interface \'list_instances\' is not defined!";
        this->reset();
        return false;
    }

    this->iwriter_instances_info_ = this->interface_writer<msg::msg_list_instances>("list_instances_info");
    if (!this->iwriter_instances_info_) {
        std::cout << "Fail! Pointer to writer_interface \'list_instances\' is not defined!";
        this->reset();
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return false;
    }

    {
        std::string sql_query_ = "CREATE TABLE INSTANCES("
                                 "UID INT PRIMARY KEY NOT NULL,"
                                 "NAME TEXT NOT NULL,"
                                 "REGION TEXT NOT NULL,"
                                 "MOVING_TYPE TEXT NOT NULL,"
                                 "STATUS_WORK INT NOT NULL,"
                                 "LONGITUDE REAL NOT NULL,"
                                 "LATITUDE REAL NOT NULL,"
                                 "EMERGENCY INT NOT NULL,"
                                 "CONNECTION INT NOT NULL,"
                                 "LOCAL_TASK INT NOT NULL,"
                                 "ADDITIONAL_ORDER INT NOT NULL"
                                 ");";

        char* messagge_error_;
        result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &messagge_error_);
        if (result_ != SQLITE_OK) {
            std::cerr << "Error Create Table \'instances\'" << std::endl;
            sqlite3_free(messagge_error_);
        }
        else
            std::cout << "Table \'instances\' created Successfully" << std::endl;
    }   // Создание таблицы в базе данных (описание исполнителей)

    {
        std::string sql_query_ = "CREATE TABLE TASKS("
                                 "UID INT PRIMARY KEY NOT NULL,"
                                 "TIME_CREATE TEXT NOT NULL,"
                                 "TYPE TEXT NOT NULL,"
                                 "STATUS INT NOT NULL,"
                                 "REGION TEXT NOT NULL,"
                                 "LONGITUDE REAL NOT NULL,"
                                 "LATITUDE REAL NOT NULL,"
                                 "TIME_BEGIN TEXT NOT NULL,"
                                 "TIME_END TEXT NOT NULL"
                                 ");";

        char* messagge_error_;
        result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &messagge_error_);
        if (result_ != SQLITE_OK) {
            std::cerr << "Error Create Table \'tasks\'!" << std::endl;
            sqlite3_free(messagge_error_);
        }
        else
            std::cout << "Table \'tasks\' created Successfully" << std::endl;
    }   // Создание таблицы в базе данных (описание заявленных задач)

    sqlite3_close(this->database_);

    // ВОзвращаем результат успешной инициализации
    return true;
}

//-----------------------------------------------------------------------------------

void fc_database_query::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // Получаем запрос от пользователя
        // Если запрос пришел выполняем соответствующим методом
        auto request_message_ = this->ireader_request_->read_next_element();
        if (request_message_) {
            // Выполняем соответствующий запрос
            this->exec(request_message_->message_);

            // Удаляем прочитанный запрос
            this->ireader_request_->remove_element(&request_message_);
        }

        // Засыпаем на 10 микросекунд
        usleep(10);
    }
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec(const msg::msg_request* const request_message_) {
    switch (request_message_->request_type()) {
    case msg::tg_request_type::_tg_update_database_: {
        this->exec_update_data(request_message_);
    } break;
    case msg::tg_request_type::_tg_get_info_: {
        this->exec_processing_data(request_message_);
    } break;
    case msg::tg_request_type::_tg_processing_data_: {
        this->exec_get_info(request_message_);
    } break;
    case msg::tg_request_type::_tg_unknown_: { }
    };
}

//=======================================================================================

void fc_database_query::exec_update_data(const msg::msg_request* const request_message_) {
    switch (request_message_->update_type()) {
    case msg::tg_update::_tg_append_instances_: { this->append_instances(request_message_); } break;
    case msg::tg_update::_tg_block_instances_: { this->block_instances(request_message_);  } break;
    case msg::tg_update::_tg_remove_instances_: { this->remove_instances(request_message_); } break;
    case msg::tg_update::_tg_update_position_: { this->update_instances_positions(request_message_); } break;
    case msg::tg_update::_tg_unknown_: { }
    }
}

//-----------------------------------------------------------------------------------

void fc_database_query::append_instances(const msg::msg_request* const request_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    // Считываем данные по исполнителем задач
    auto data_ = request_->data();
    mps::json::loader::JsonLoader loader_(data_,mps::json::loader::JsonLoader::String);
    auto dsc_instances_ = loader_.rootObject();

    //(?) Если описание исполнителей задач
    if (this->description_instances_valid(dsc_instances_)) {
        for (int32_t i = 0; i < dsc_instances_->asArray("instances")->size(); ++i) {
            auto dsc_instance_ = dsc_instances_->asArray("instances")->asObject(i);
            auto competence_ = dsc_instance_->asArray("instance_competence");
            std::string sql_query_ = ("INSERT INTO INSTANCES VALUES("
                                      + std::to_string(dsc_instances_->asInteger("instance_uid")) + ", "
                                      + dsc_instance_->asString("instance_name") + ", "
                                      + dsc_instance_->asString("intance_region") + ", "
                                      + dsc_instance_->asString("instance_moving_type") + ", "
                                      + std::to_string((int)types::tg_instance::_tg_status_free_) + ", "
                                      + std::to_string(dsc_instance_->asObject("instance_start_position")->hasProperty("longitude")) + ", "
                                      + std::to_string(dsc_instance_->asObject("instance_start_position")->hasProperty("latitude")) + ", "
                                      + std::to_string(competence_->asInteger(3)) + ", "
                                      + std::to_string(competence_->asInteger(2)) + ", "
                                      + std::to_string(competence_->asInteger(1)) + ", "
                                      + std::to_string(competence_->asInteger(0))
                                      + ");");

            char* message_error_;
            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Insert data to table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            } else {
                std::cout << "Insert new instance:\n" << sql_query_ << std::endl;
            }
        }
    }

    // Закрываем базу данных
    sqlite3_close(this->database_);
}

//-----------------------------------------------------------------------------------

void fc_database_query::append_tasks(const msg::msg_tasks_list * const request_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    for (uint32_t i = 0; i < request_->tasks_count(); ++i) {
        auto task_ = request_->get_task(i);
        if (task_) {
            std::string sql_query_ = ("INSERT INTO TASKS VALUES("
                                      + std::to_string(task_->task_uid()) + ", "
                                      + this->task_type_to_str(task_->task_type()) + ", "
                                      + std::to_string((int)task_->task_status()) + ", "
                                      + task_->region() + ","
                                      + std::to_string(task_->task_position().longitude()) + ", "
                                      + std::to_string(task_->task_position().latitude()) + ", "
                                      + task_->time_window().time_begin() + ", "
                                      + task_->time_window().time_end()
                                      + ");");
            char* message_error_;
            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Insert data to table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            } else {
                std::cout << "Insert new instance:\n" << sql_query_ << std::endl;
            }
        }
    }

    // Закрываем базу данных
    sqlite3_close(this->database_);
}

std::string fc_database_query::task_type_to_str(types::tg_task tg_) {
    switch (tg_) {
    case types::tg_task::_tg_emergency_: { return "global_problem"; } break;
    case types::tg_task::_tg_connection_: { return "connection"; } break;
    case types::tg_task::_tg_local_task_: { return "local_task"; } break;
    case types::tg_task::_tg_additional_order_: { return "additional_order"; } break;
    case types::tg_task::_tg_unknown_: { return ""; }
    }

    return "";
}

//-----------------------------------------------------------------------------------

void fc_database_query::block_instances(const msg::msg_request* const request_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    // Считываем данные по исполнителем задач
    auto data_ = request_->data();
    mps::json::loader::JsonLoader loader_(data_,mps::json::loader::JsonLoader::String);
    auto dsc_instances_ = loader_.rootObject();

    //(?) Если описание исполнителей задач
    if (this->description_instances_valid(dsc_instances_)) {
        for (int32_t i = 0; i < dsc_instances_->asArray("instances")->size(); ++i) {
            auto dsc_instance_ = dsc_instances_->asArray("instances")->asObject(i);
            std::string sql_query_ = ("UPDATE INSTANCES SET STATUS_WORK  = "
                                      + std::to_string((int)types::tg_instance::_tg_status_unavailable_)
                                      + " WHERE ID = " +
                                      std::to_string(dsc_instance_->asInteger("instance_uid")) + ";");

            char* message_error_;
            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Block instance in table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            }
        }
    }

    // Закрываем базу данных
    sqlite3_close(this->database_);
}

//-----------------------------------------------------------------------------------

void fc_database_query::remove_instances(const msg::msg_request* const request_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    // Считываем данные по исполнителем задач
    auto data_ = request_->data();
    mps::json::loader::JsonLoader loader_(data_,mps::json::loader::JsonLoader::String);
    auto dsc_instances_ = loader_.rootObject();

    //(?) Если описание исполнителей задач
    if (this->description_instances_valid(dsc_instances_)) {
        for (int32_t i = 0; i < dsc_instances_->asArray("instances")->size(); ++i) {
            auto dsc_instance_ = dsc_instances_->asArray("instances")->asObject(i);
            std::string sql_query_ = ("DELETE FROM INSTANCES WHERE ID = " +
                                      std::to_string(dsc_instance_->asInteger("instance_uid")) + ";");

            char* message_error_;
            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Delete data from table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            }
        }
    }

    // Закрываем базу данных
    sqlite3_close(this->database_);
}

//-----------------------------------------------------------------------------------

void fc_database_query::update_instances_positions(const msg::msg_request* const request_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    // Считываем данные по исполнителем задач
    auto data_ = request_->data();
    mps::json::loader::JsonLoader loader_(data_,mps::json::loader::JsonLoader::String);
    auto dsc_instances_ = loader_.rootObject();

    //(?) Если описание исполнителей задач
    if (this->description_instances_valid(dsc_instances_)) {
        for (int32_t i = 0; i < dsc_instances_->asArray("instances")->size(); ++i) {
            auto dsc_instance_ = dsc_instances_->asArray("instances")->asObject(i);
            std::string sql_query_ = ("UPDATE INSTANCES SET LONGITUDE  = "
                                      + std::to_string(dsc_instance_->asObject("instance_start_position")->asReal("longitude"))
                                      + " WHERE ID = " +
                                      std::to_string(dsc_instance_->asInteger("instance_uid")) + ";");

            char* message_error_;
            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Block instance in table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            }

            sql_query_ = ("UPDATE INSTANCES SET LATITUDE  = "
                          + std::to_string(dsc_instance_->asObject("instance_start_position")->asReal("latitude"))
                          + " WHERE ID = " +
                          std::to_string(dsc_instance_->asInteger("instance_uid")) + ";");

            result_ = sqlite3_exec(this->database_, sql_query_.c_str(), nullptr, 0, &message_error_);
            if (result_ != SQLITE_OK) {
                std::cerr << "Error Block instance in table \'instances\'!" << std::endl;
                sqlite3_free(message_error_);
            }
        }
    }

    // Закрываем базу данных
    sqlite3_close(this->database_);
}

//-----------------------------------------------------------------------------------

bool fc_database_query::description_instances_valid(const mps::json::object::JsonObject* dsc_insts_) {
    if (!dsc_insts_ || !dsc_insts_->hasProperty("instances") || !dsc_insts_->asArray("instances")) {
        return false;
    }

    //(?>) Проверяем описание каждого исполнителя задач
    for (int32_t i = 0; i < dsc_insts_->asArray("instances")->size(); ++i) {
        auto dsc_instance_ = dsc_insts_->asArray("instances")->asObject(i);
        if (!dsc_instance_ ||
                !dsc_instance_->hasProperty("instance_uid") ||
                !dsc_instance_->hasProperty("instance_name") ||
                !dsc_instance_->hasProperty("intance_region") ||
                !dsc_instance_->hasProperty("instance_moving_type") ||
                !dsc_instance_->hasProperty("instance_start_position") ||
                !dsc_instance_->asObject("instance_start_position") ||
                !dsc_instance_->asObject("instance_start_position")->hasProperty("longitude") ||
                !dsc_instance_->asObject("instance_start_position")->hasProperty("latitude") ||
                !dsc_instance_->hasProperty("instance_competence") ||
                !dsc_instance_->asArray("instance_competence")) {
            return false;
        }
    }

    return true;
}

//===================================================================================
void fc_database_query::exec_processing_data(const msg::msg_request* const request_message_) {
    switch (request_message_->processing_type()) {
    case msg::tg_processing::_tg_task_distribution_: { this->exec_task_distribution(request_message_);  } break;
    case msg::tg_processing::_tg_process_emergency_: { this->exec_task_distribution(request_message_); } break;
    case msg::tg_processing::_tg_process_cancel_: { this->exec_task_distribution(request_message_); } break;
    case msg::tg_processing::_tg_unknown_: { }
    };

    //(?>) Ожидаем сообщение с описанием задач
    while (1) {
        auto tasks_message_ = this->ireader_tasks_->read_next_element();
        if (tasks_message_) {
            // Добавляем новые задачи в базу данных
            this->append_tasks(tasks_message_->message_);

            // Удаляем прочитанное сообщение
            this->ireader_tasks_->remove_element(&tasks_message_);
        }

        // Засыпаем на 10 микросекунд
        usleep(10);
    }
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_task_distribution(const msg::msg_request* const request_message_) {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM INSTANCES;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_instances, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    // Формируем сообщение
    auto free_element_ = this->iwriter_instances_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_available_instances_);
        for (auto& instance_ : current_free_element_.instances_) {
            //(?) Если исоплнитель доступен, дообавляем его в список
            if (instance_->tag() != types::tg_instance::_tg_status_unavailable_ &&
                    instance_->tag() != types::tg_instance::_tg_status_unknown_) {
                free_element_->element_->append_instance(std::move(instance_));
            }
        }

        // Добавляем новое сообщение в буфер
        this->iwriter_instances_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//=======================================================================================

void fc_database_query::exec_get_info(const msg::msg_request* const request_message_) {
    switch (request_message_->info_type()) {
    case msg::tg_get_info::_tg_free_instances_: { this->exec_get_free_instances();  } break;
    case msg::tg_get_info::_tg_job_instances_: { this->exec_get_job_instances(); } break;
    case msg::tg_get_info::_tg_free_tasks_: { this->exec_get_free_tasks(); } break;
    case msg::tg_get_info::_tg_job_tasks_: { this->exec_get_job_tasks(); } break;
    case msg::tg_get_info::_tg_completed_tasks_: { this->exec_completed_tasks(); } break;
    case msg::tg_get_info::_tg_current_positions_: { this->exec_current_position(); } break;
    case msg::tg_get_info::_tg_unknown_: { }
    };
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_free_instances() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM INSTANCES;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_instances, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    // Формируем сообщение
    auto free_element_ = this->iwriter_instances_info_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_free_instances_);
        for (auto& instance_ : current_free_element_.instances_) {
            //(?) Если исоплнитель доступен, дообавляем его в список
            if (instance_->tag() == types::tg_instance::_tg_status_free_) {
                free_element_->element_->append_instance(std::move(instance_));
            }
        }

        // Добавляем новое сообщение в буфер
        this->iwriter_instances_info_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_job_instances() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM INSTANCES;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_instances, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    // Формируем сообщение
    auto free_element_ = this->iwriter_instances_info_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_job_instances_);
        for (auto& instance_ : current_free_element_.instances_) {
            //(?) Если исоплнитель доступен, дообавляем его в список
            if (instance_->tag() == types::tg_instance::_tg_status_work_) {
                free_element_->element_->append_instance(std::move(instance_));
            }
        }

        // Добавляем новое сообщение в буфер
        this->iwriter_instances_info_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_free_tasks() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM TASKS;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_tasks, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    //(?>) Заполняем сообщение (указываем только неназначенные задачи)
    auto free_element_ = this->iwriter_tasks_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_free_tasks_);
        for (auto& task_ : current_free_element_.tasks_) {
            if (task_->task_status() == types::tg_status::_tg_free_) {
                free_element_->element_->append_task(std::move(task_));
            }
        }

        this->iwriter_tasks_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_job_tasks() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM TASKS;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_tasks, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    //(?>) Заполняем сообщение (указываем только назначенные задачи)
    auto free_element_ = this->iwriter_tasks_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_job_tasks_);
        for (auto& task_ : current_free_element_.tasks_) {
            if (task_->task_status() == types::tg_status::_tg_work_) {
                free_element_->element_->append_task(std::move(task_));
            }
        }

        this->iwriter_tasks_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_completed_tasks() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM TASKS;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_tasks, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    //(?>) Заполняем сообщение (указываем только выполненные задачи)
    auto free_element_ = this->iwriter_tasks_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_completed_tasks_);
        for (auto& task_ : current_free_element_.tasks_) {
            if (task_->task_status() == types::tg_status::_tg_completed_) {
                free_element_->element_->append_task(std::move(task_));
            }
        }

        this->iwriter_tasks_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_current_position() {
    // Подключаемся (создаем) к базе данных
    auto result_ = sqlite3_open(this->database_name_.c_str(),&this->database_);
    if (result_) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database_) << std::endl;
        return;
    }

    std::string sql_query_ = "SELECT * FROM INSTANCES;";
    result_ = sqlite3_exec(this->database_, sql_query_.c_str(), callback_make_instances, nullptr, nullptr);
    if (result_ != SQLITE_OK) {
        std::cout << "Error select instances";
    }

    // Отключаемся от базы данных
    sqlite3_close(this->database_);

    // Формируем сообщение
    auto free_element_ = this->iwriter_instances_info_->get_free_element();
    if (free_element_) {
        free_element_->element_->init_list(current_free_element_.count_instances_positions_);
        for (auto& instance_ : current_free_element_.instances_) {
            //(?) Если исоплнитель доступен, дообавляем его в список
            if (instance_->tag() == types::tg_instance::_tg_status_free_ ||
                    instance_->tag() == types::tg_instance::_tg_status_work_) {
                free_element_->element_->append_instance(std::move(instance_));
            }
        }

        // Добавляем новое сообщение в буфер
        this->iwriter_instances_info_->add_new_element(free_element_);
    }

    current_free_element_.reset();
}

//-----------------------------------------------------------------------------------

void fc_database_query::reset() {
    if (this->ireader_request_) {
        delete this->ireader_request_;
        this->ireader_request_ = nullptr;
    }

    if (this->iwriter_instances_) {
        delete this->iwriter_instances_;
        this->iwriter_instances_ = nullptr;
    }

    if (this->iwriter_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }
}




//------------------------------------------------------------------------------------------------------------------------
//--------------------------------- COMPONENT REGISTRATION ----------------------------------
//------------------------------------------------------------------------------------------------------------------------
#include <mps/mps_process_traits/components_traits/components_container/components_container.hpp>

static bool database_query_registration() {
    //(?) Если контейнер для регистрации компонент инициализирован, регистрируем компоненту database_query
    if (register_components_container_) {
        boost::shared_ptr<mps::process::component::base::base_functional_component> database_query_(
                    new td::component::fc_database_query("database_query"));
        return register_components_container_->component_registration(database_query_,"database_query");
    }

    return false;
}

static bool database_query_registration_ = database_query_registration();
