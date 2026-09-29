#include "components/request_listener/request_listener.hpp"
#include <mps/mps_common/utils/json_io/json.hpp>

#define STR_REQUEST_TYPE_UPDATE_DATABASE    (std::string)"update_database"
#define STR_REQUEST_TYPE_PROCESSING_DATA    (std::string)"processing_data"
#define STR_REQUEST_TYPE_GET_INFO           (std::string)"get_info"

#define STR_SUBTYPE_UPDATE_DATABASE         (std::string)"append_instances"
#define STR_SUBTYPE_REMOVE_INSTANCES        (std::string)"remove_instances"
#define STR_SUBTYPE_BLOCK_INSTANCES         (std::string)"block_instances"
#define STR_SUBTYPE_UPDATE_POSITIONS        (std::string)"update_positions"

#define STR_SUBTYPE_PROCESSING_TASK_DISTRIBUTION                (std::string)"task_distribution"
#define STR_SUBTYPE_PROCESSING_PROCESS_EMERGENCY                (std::string)"process_emergency"
#define STR_SUBTYPE_PROCESSING_PROCESS_CANCEL                   (std::string)"process_cancel"

#define STR_SUBTYPE_GET_INFO_FREE_INSTANCES     (std::string)"free_instances"
#define STR_SUBTYPE_GET_INFO_JOB_INSTANCES      (std::string)"job_instances"
#define STR_SUBTYPE_GET_INFO_FREE_TASKS         (std::string)"free_tasks"
#define STR_SUBTYPE_GET_INFO_JOB_TASKS          (std::string)"job_tasks"
#define STR_SUBTYPE_GET_INFO_COMPLETED_TASKS    (std::string)"completed_tasks"
#define STR_SUBTYPE_GET_INFO_CURRENT_POSITIONS  (std::string)"current_positions"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_request_listener::fc_request_listener(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_), socket_(context_, zmq::socket_type::rep) {
    this->socket_.bind(this->address_);
}

//-----------------------------------------------------------------------------------

fc_request_listener::~fc_request_listener() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->iwriter_request_) {
        delete this->iwriter_request_;
        this->iwriter_request_ = nullptr;
    }
}

//-----------------------------------------------------------------------------------

bool fc_request_listener::init() {
    // Получаем указатель на интерфейс писателя сообщений типа user_request
    this->iwriter_request_ = this->interface_writer<msg::msg_request>("user_request");
    if (!this->iwriter_request_) {
        std::cout << "Fail! Pointer to writer_interface \'user_request\' is not defined!";
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа user_request
    this->iwriter_request_database_ = this->interface_writer<msg::msg_request>("user_request_database");
    if (!this->iwriter_request_) {
        std::cout << "Fail! Pointer to writer_interface \'user_request_database\' is not defined!";
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    return true;
}

//-----------------------------------------------------------------------------------

void fc_request_listener::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";
    std::cout << "[request_listener]: server is started on tcp://*:5555\n";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        zmq::message_t request_;
        auto result_ = this->socket_.recv(request_, zmq::recv_flags::none);
        if (!result_) {
            break;
        }

        std::cout << "[request_listener]: get request from user!\n";

        // Формируем сообщение типа user_request
        this->make_request(request_.to_string());

        // Засыпаем на 10 микросекунд
        usleep(10);
    }
}

//-----------------------------------------------------------------------------------

void fc_request_listener::make_request(const std::string& request_) {
    //(?) Проверяем, что указан тип запроса
    mps::json::loader::JsonLoader loader_(request_, mps::json::loader::JsonLoader::String);
    auto obj_request_ = loader_.rootObject();

    //(?) Определяем тип и субтип запроса
    if (obj_request_->hasProperty("request_type") &&
            obj_request_->hasProperty("request_subtype")) {
        auto request_type_ = this->define_type(obj_request_->asString("request_type"));
        if (request_type_ == msg::tg_request_type::_tg_unknown_) {
            return;
        }

        if (!this->define_subtype(request_type_,obj_request_->asString("request_subtype"),request_)) {
            std::cout << "[request_listener]: fail! incorrect request from user!";
        } else {
            std::cout << "[request_listener]: request is created!";
        }
    }
}

//-----------------------------------------------------------------------------------

bool fc_request_listener::define_subtype(msg::tg_request_type& type_, const std::string& subtype_, const std::string& request_) {
    switch (type_) {
    case msg::tg_request_type::_tg_update_database_: {
        if (this->define_update_subtype(subtype_, request_) != msg::tg_update::_tg_unknown_) {
            return true;
        }
    } break;
    case msg::tg_request_type::_tg_processing_data_: {
        if (this->define_processing_subtype(subtype_, request_) != msg::tg_processing::_tg_unknown_) {
            return true;
        }
    } break;
    case msg::tg_request_type::_tg_get_info_: {
        if (this->define_info_subtype(subtype_, request_) != msg::tg_get_info::_tg_unknown_) {
            return true;
        }
    } break;
    case msg::tg_request_type::_tg_unknown_: { return false;  } break;
    }

    return false;
}

//-----------------------------------------------------------------------------------

msg::tg_request_type fc_request_listener::define_type(const std::string& type_ ) {
    if (type_ == STR_REQUEST_TYPE_UPDATE_DATABASE) {
        return msg::tg_request_type::_tg_update_database_;
    } else if (type_ == STR_REQUEST_TYPE_PROCESSING_DATA) {
        return msg::tg_request_type::_tg_processing_data_;
    } else if (type_ == STR_REQUEST_TYPE_GET_INFO) {
        return msg::tg_request_type::_tg_get_info_;
    }

    return msg::tg_request_type::_tg_unknown_;
}

//-----------------------------------------------------------------------------------

msg::tg_update fc_request_listener::define_update_subtype(const std::string& subtype_, const std::string& request_) {
    auto tg_ = msg::tg_update::_tg_unknown_;

    if (subtype_ == STR_SUBTYPE_UPDATE_DATABASE) { tg_ = msg::tg_update::_tg_append_instances_; }
    else if (subtype_ == STR_SUBTYPE_REMOVE_INSTANCES) { tg_ = msg::tg_update::_tg_remove_instances_; }
    else if (subtype_ == STR_SUBTYPE_BLOCK_INSTANCES) { tg_ = msg::tg_update::_tg_block_instances_; }
    else if (subtype_ == STR_SUBTYPE_UPDATE_POSITIONS) { tg_ = msg::tg_update::_tg_update_position_; }

    //(?) Если подтип запроса определен, формируем соответствующее сообщение в модуль запросов к базе данных
    if (tg_ != msg::tg_update::_tg_unknown_) {
        auto free_element_ = this->iwriter_request_database_->get_free_element();
        if (free_element_) {
            free_element_->element_->set_request_type(msg::tg_request_type::_tg_update_database_);
            free_element_->element_->set_update_type(tg_);
            free_element_->element_->set_data(request_);

            this->iwriter_request_database_->add_new_element(free_element_);
        }
    }

    return tg_;
}

//-----------------------------------------------------------------------------------

msg::tg_processing fc_request_listener::define_processing_subtype(const std::string& subtype_, const std::string& request_) {
    auto tg_ = msg::tg_processing::_tg_unknown_;

    if (subtype_ == STR_SUBTYPE_PROCESSING_TASK_DISTRIBUTION) { tg_ = msg::tg_processing::_tg_task_distribution_; }
    else if (subtype_ == STR_SUBTYPE_PROCESSING_PROCESS_EMERGENCY) { tg_ = msg::tg_processing::_tg_process_emergency_; }
    else if (subtype_ == STR_SUBTYPE_PROCESSING_PROCESS_CANCEL) { tg_ = msg::tg_processing::_tg_process_cancel_; }

    //(?) Если подтипа запроса определен, ...
    if (tg_ != msg::tg_processing::_tg_unknown_) {
        {
            auto free_element_ = this->iwriter_request_database_->get_free_element();
            if (free_element_) {
                free_element_->element_->set_request_type(msg::tg_request_type::_tg_get_info_);
                free_element_->element_->set_processing_type(tg_);
                free_element_->element_->set_data(request_);

                this->iwriter_request_database_->add_new_element(free_element_);
            }
        }   // Формируем сообщение в модуль запросов к базе данных
        {
            auto free_element_ = this->iwriter_request_->get_free_element();
            if (free_element_) {
                free_element_->element_->set_request_type(msg::tg_request_type::_tg_get_info_);
                free_element_->element_->set_processing_type(tg_);
                free_element_->element_->set_data(request_);

                this->iwriter_request_->add_new_element(free_element_);
            }   // Формируем сообщение в модуль конвертации данных
        }
    }

    return tg_;
}

//-----------------------------------------------------------------------------------

msg::tg_get_info fc_request_listener::define_info_subtype(const std::string& subtype_, const std::string &request_) {
    auto tg_ = msg::tg_get_info::_tg_unknown_;

    if (subtype_ == STR_SUBTYPE_GET_INFO_FREE_INSTANCES) { tg_ = msg::tg_get_info::_tg_free_instances_; }
    else if (subtype_ == STR_SUBTYPE_GET_INFO_JOB_INSTANCES) { tg_ = msg::tg_get_info::_tg_job_instances_; }
    else if (subtype_ == STR_SUBTYPE_GET_INFO_FREE_TASKS) { tg_ = msg::tg_get_info::_tg_free_tasks_; }
    else if (subtype_ == STR_SUBTYPE_GET_INFO_JOB_TASKS) { tg_ = msg::tg_get_info::_tg_job_tasks_; }
    else if (subtype_ == STR_SUBTYPE_GET_INFO_COMPLETED_TASKS) { tg_ = msg::tg_get_info::_tg_completed_tasks_; }
    else if (subtype_ == STR_SUBTYPE_GET_INFO_CURRENT_POSITIONS) { tg_ = msg::tg_get_info::_tg_current_positions_; }

    //(?) Если подтип запроса определен, формируем соответствующее сообщение в модуль запросов к базе данных
    if (tg_ != msg::tg_get_info::_tg_unknown_) {
        auto free_element_ = this->iwriter_request_database_->get_free_element();
        if (free_element_) {
            free_element_->element_->set_request_type(msg::tg_request_type::_tg_get_info_);
            free_element_->element_->set_info_type(tg_);
            free_element_->element_->set_data(request_);

            this->iwriter_request_database_->add_new_element(free_element_);
        }
    }

    return tg_;
}



//------------------------------------------------------------------------------------------------------------------------
//--------------------------------- COMPONENT REGISTRATION ----------------------------------
//------------------------------------------------------------------------------------------------------------------------
#include <mps/mps_process_traits/components_traits/components_container/components_container.hpp>

static bool request_listener_registration() {
    //(?) Если контейнер для регистрации компонент инициализирован, регистрируем компоненту request_listener
    if (register_components_container_) {
        boost::shared_ptr<mps::process::component::base::base_functional_component> request_listener_(
            new td::component::fc_request_listener("request_listener"));
        return register_components_container_->component_registration(request_listener_,"request_listener");
    }

    return false;
}

static bool request_listener_registration_ = request_listener_registration();

