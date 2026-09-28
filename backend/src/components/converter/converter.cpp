#include "components/converter/converter.hpp"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_converter::fc_converter(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) {}

//-----------------------------------------------------------------------------------

fc_converter::~fc_converter() { this->reset(); }

//-----------------------------------------------------------------------------------

bool fc_converter::init() {
    // Получаем указатель на интерфейс читателя сообщений типа user_request
    this->ireader_request_ = this->interface_reader<msg::msg_request>("user_request");
    if (!this->ireader_request_) {
        std::cout << "Fail! Pointer to reader_interface \'user_request\' is not defined!";
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа time_table
    this->iwriter_time_table_ = this->interface_writer<msg::msg_time_table>("time_table");
    if (!this->iwriter_time_table_) {
        std::cout << "Fail! Pointer to writer_interface \'time_table\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа list_tasks
    this->iwriter_tasks_ = this->interface_writer<msg::msg_tasks_list>("list_tasks");
    if (!this->iwriter_tasks_) {
        std::cout << "Fail! Pointer to writer_interface \'list_tasks\' is not defined!";
        this->reset();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------

void fc_converter::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        auto request_message_ = this->ireader_request_->read_next_element();
        if (request_message_) {
            std::cout << "[converter_component]: get new request!\n";

            // Формируем и отправляем сообщения типа tasks_list и time_table
            this->make_messages(request_message_->message_->data());

            // Удаляем прочитанное сообщение
            this->ireader_request_->remove_element(&request_message_);
        }

        // Засыпаем на 10 микросекунд
        usleep(10);
    }
}

//-----------------------------------------------------------------------------------

bool fc_converter::make_messages(const std::string& message_) {
    // Считываем запрос в формате json
    mps::json::loader::JsonLoader loader_(message_, mps::json::loader::JsonLoader::String);
    auto request_ = loader_.rootObject();

    //(?) Если запрос валиден, то...
    if (this->request_valid(request_)) {
        //(?) Формируем и отправляем сообщение типа tasks_list
        if (!this->make_tasks_lists(request_)) {
            // Формируем и отправляем сообщение типа time_table
            return this->make_time_table(request_);
        }
    }

    // В протвном случае возвращаем отрицательный результат
    return false;
}

//-----------------------------------------------------------------------------------

bool fc_converter::make_tasks_lists(const mps::json::object::JsonObject* request_) {
    // Получаем указатель на свободный элемент буфера хранения сообщений типа tasks_list
    auto free_element_ = this->iwriter_tasks_->get_free_element();
    if (free_element_) {
        //(?>) Формируем описание всех задач на выполнение
        free_element_->element_->init_list(request_->asArray("points")->size());
        for (int32_t i = 0; i < request_->asArray("points")->size(); ++i) {
            free_element_->element_->append_task(new types::tp_task(request_->asArray("points")->asObject(i)));
        }

        // Отправляем сообщение с описанием задач на исоплнение
        std::cout << "[converter_component]: send message \'list_tasks\'!\n";
        return this->iwriter_tasks_->add_new_element(free_element_);
    }

    // В противном случае возвращаем отрицательный результат
    return false;
}

//-----------------------------------------------------------------------------------

bool fc_converter::make_time_table(const mps::json::object::JsonObject* request_) {
    // Получаем указатель на свобожный элемент буфера хранения сообщений типа time_table
    auto free_element_ = this->iwriter_time_table_->get_free_element();
    if (free_element_) {
        *free_element_->element_ = request_->asArray("matrix");

        // Отправляем сообщение с описанием временной таблицы
        std::cout << "[converter_component]: send message \'time_table\'!\n";
        return this->iwriter_time_table_->add_new_element(free_element_);
    }

    // В противном случае возвращаем отрицательный результат
    return false;
}

//-----------------------------------------------------------------------------------
/* Требуемый формат запроса
{
  "generatedAt": ...,
  "source": ...,
  "units": ...,
  "pointCount": ...,
  "points": [
    {
      "index": ...,
      "task_uid": ...,
      "task_region": ...,
      "lat": ...,
      "lon": ...,
      "time_window": {
        "time_begin": ...,
        "time_end": ...
      }
    },
    ...
   ],
   "matricies":
   [
        {
            "profile": ...,
            "matrix":
            [[...],...],
        },
        ...
   ]
}
 */
bool fc_converter::request_valid(const mps::json::object::JsonObject* request_) {
    return request_ &&
            request_->hasProperty("generatedAt") &&
            request_->hasProperty("source") &&
            request_->hasProperty("units") &&
            request_->hasProperty("pointCount") &&
            request_->hasProperty("points") &&
            request_->asArray("points") &&
            request_->hasProperty("matricies") &&
            request_->asArray("matricies") &&
            this->tasks_valid(request_->asArray("points")) &&
            this->matricies_valid(request_);
}

//-----------------------------------------------------------------------------------

bool fc_converter::tasks_valid(const mps::json::array::JsonArray* points_) {
    //(?>) Проверяем описание каждой задачи на валидность и возввращаем соответствующий результат
    for (int32_t i = 0; i < points_->size(); ++i) {
        auto point_ = points_->asObject(i);
        if (!point_ ||
                !point_->hasProperty("index") ||
                !point_->hasProperty("task_uid") ||
                !point_->hasProperty("task_region") ||
                !point_->hasProperty("lat") ||
                !point_->hasProperty("lon") ||
                !point_->hasProperty("time_window") ||
                !point_->asObject("time_window") ||
                !point_->asObject("time_window")->hasProperty("time_begin") ||
                !point_->asObject("time_window")->hasProperty("time_end")) {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------------

bool fc_converter::matricies_valid(const mps::json::object::JsonObject* request_) {
    //(?>) Проверяем кажду матрицу на валидность
    for (int32_t i = 0; i < request_->asArray("matricies")->size(); ++i) {
        auto mtx_ = request_->asArray("matricies")->asObject(i);
        if (!this->matrix_valid(request_, mtx_)) {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------------

bool fc_converter::matrix_valid(const mps::json::object::JsonObject* request_, const mps::json::object::JsonObject* mtx_) {
    //(?>) Проверяем описание временной матрицы и возвращаем соответствующий результат
    if (!mtx_->asArray("matrix") ||
        !mtx_->hasProperty("profile") ||
        request_->asInteger("pointCount") != mtx_->asArray("matrix")->size()) {
        return false;
    }

    for (int32_t i = 0; i < request_->asArray("matrix")->size(); ++i) {
        if (!mtx_->asArray("matrix")->asArray(i) ||
                mtx_->asArray("matrix")->asArray(i)->size() != request_->asInteger("pointCount")) {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------------

void fc_converter::reset() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->ireader_request_) {
        delete this->ireader_request_;
        this->ireader_request_ = nullptr;
    }

    if (this->iwriter_time_table_) {
        delete this->iwriter_time_table_;
        this->iwriter_time_table_ = nullptr;
    }

    if (this->iwriter_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }
}
