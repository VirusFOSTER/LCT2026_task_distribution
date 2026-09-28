#include "components/database_query/database_query.hpp"


using namespace td;
using namespace component;

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

    // Получаем указатель на интерфейс писателя сообщений типа list_instances
    this->iwriter_instances_ = this->interface_writer<msg::msg_list_instances>("list_instances");
    if (!this->iwriter_instances_) {
        std::cout << "Fail! Pointer to writer_interface \'list_instances\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс читателя сообщений типа list_tasks
    this->ireader_tasks_ = this->interface_reader<msg::msg_tasks_list>("list_tasks");
    if (!this->ireader_tasks_) {
        std::cout << "Fail! Pointer to reader_interface \'list_tasks\' is not defined!";
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
        result_ = sqlite3_exec(this->database_, sql_query_.c_str(), NULL, 0, &messagge_error_);
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
                                 "REGION TEXT NOT NULL,"
                                 "LONGITUDE REAL NOT NULL,"
                                 "LATITUDE REAL NOT NULL,"
                                 "TIME_BEGIN TEXT NOT NULL,"
                                 "TIME_END TEXT NOT NULL"
                                 ");";

        char* messagge_error_;
        result_ = sqlite3_exec(this->database_, sql_query_.c_str(), NULL, 0, &messagge_error_);
        if (result_ != SQLITE_OK) {
            std::cerr << "Error Create Table \'tasks\'" << std::endl;
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
    case msg::tg_update::_tg_unknown_: { }
    }
}

//-----------------------------------------------------------------------------------

void fc_database_query::append_instances(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::block_instances(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::remove_instances(const msg::msg_request* const request_) {

}

//===================================================================================
void fc_database_query::exec_processing_data(const msg::msg_request* const request_message_) {
    switch (request_message_->processing_type()) {
    case msg::tg_processing::_tg_task_distribution_: {  } break;
    case msg::tg_processing::_tg_process_emergency_: {  } break;
    case msg::tg_processing::_tg_process_cancel_: {  } break;
    case msg::tg_processing::_tg_unknown_: { }
    };
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_task_distribution(const msg::msg_request* const request_message_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_process_emergency(const msg::msg_request* const request_message_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_process_cancel(const msg::msg_request* const request_message_) {

}

//=======================================================================================

void fc_database_query::exec_get_info(const msg::msg_request* const request_message_) {
    switch (request_message_->info_type()) {
    case msg::tg_get_info::_tg_free_instances_: {  } break;
    case msg::tg_get_info::_tg_job_instances_: {  } break;
    case msg::tg_get_info::_tg_free_tasks_: {  } break;
    case msg::tg_get_info::_tg_job_tasks_: {  } break;
    case msg::tg_get_info::_tg_completed_tasks_: {  } break;
    case msg::tg_get_info::_tg_current_positions_: {  } break;
    case msg::tg_get_info::_tg_unknown_: { }
    };
}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_free_instances(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_job_instances(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_free_tasks(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_get_job_tasks(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_completed_tasks(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::exec_current_position(const msg::msg_request* const request_) {

}

//-----------------------------------------------------------------------------------

void fc_database_query::reset() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->ireader_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }

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
