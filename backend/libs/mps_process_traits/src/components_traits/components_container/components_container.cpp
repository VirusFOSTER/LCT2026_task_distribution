#include "components_traits/components_container/components_container.hpp"

using namespace mps;
using namespace process;
using namespace container;
using namespace components;

//-----------------------------------------------------------------------

register_components_container::register_components_container() {}

//-----------------------------------------------------------------------

bool register_components_container::component_registration(component_t component_,
                                                  const std::string &component_name_)
{
    //(?>) Проверяем наличие компоненты с указанным именем в контейнере
    auto ptr_component_ = this->components_list_;
    decltype(ptr_component_) last_component_ = nullptr;
    while (ptr_component_) {
        if (ptr_component_->component_->component_name() == component_name_) {
            return false;
        }
        last_component_ = ptr_component_;
        ptr_component_ = ptr_component_->next_component_;
    }

    //(?) Если в контейнере имеются уже компоненты и при этом с указанным именем отсутствует,
    // добавлем новую компоненту в контейнер в конец списка
    // В противном случае инициализируем список компонент
    if (last_component_) {
        last_component_->next_component_ = new list_component;
        last_component_->next_component_->prev_component_ = last_component_;
        last_component_->next_component_->component_ = component_;

        return true;
    }

    this->components_list_ = new list_component;
    this->components_list_->component_ = component_;

    return true;
}

//-----------------------------------------------------------------------

typename register_components_container::component_t
register_components_container::get_component(const std::string &component_name_)
{
    //(?>) Ищем в списке компоненту с указанным именем
    // Если такая компонента зарегистрирована, то возвращаем указатель на нее
    // В противном случае возвращаем nullptr
    auto ptr_component_ = this->components_list_;
    while (ptr_component_) {
        if (ptr_component_->component_->component_name() == component_name_) {
            return ptr_component_->component_;
        }
        ptr_component_ = ptr_component_->next_component_;
    }

    return nullptr;
}

//-----------------------------------------------------------------------

typename register_components_container::component_t
register_components_container::get_component(const uint16_t &component_uid_) {
    //(?>) Ищем в списке компоненту с указанным идентификатором
    // Если такая компонента зарегистрирована, то возвращаем указатель на нее
    // В противном случае возвращаем nullptr
    auto ptr_component_ = this->components_list_;
    while (ptr_component_) {
        if (ptr_component_->component_->component_uid() == component_uid_) {
            return ptr_component_->component_;
        }
        ptr_component_ = ptr_component_->next_component_;
    }

    return nullptr;
}

//-----------------------------------------------------------------------

std::string register_components_container::get_component_name(const uint16_t component_uid_) {
    //(?>) Ищем в списке компоненту с указанным идентификатором
    // Если такая компонента зарегистрирована, то возвращаем ее наименование
    // В противном случае возвращаем пустое имя (не соответствует правилу регистрации компоненты)
    auto ptr_component_ = this->components_list_;
    while (ptr_component_) {
        if (ptr_component_->component_->component_uid() == component_uid_) {
            return ptr_component_->component_->component_name();
        }
        ptr_component_ = ptr_component_->next_component_;
    }

    return "";
}

//-----------------------------------------------------------------------

void register_components_container::remove_component(const std::string component_name_) {
    //(?>) Ищем в списке компоненту с указанным именем
    auto ptr_component_ = this->components_list_;
    while (ptr_component_) {
        if (ptr_component_->component_->component_name() == component_name_) {
            break;
        }
        ptr_component_ = ptr_component_->next_component_;
    }

    // (?) Если такая компонента в контейнере зарегистрирована, то удаляем ее
    if (ptr_component_) {
        this->remove_component(ptr_component_);
    }
}

//-----------------------------------------------------------------------

void register_components_container::remove_component(const uint16_t &component_uid_) {
    //(?>) Ищем в списке компоненту с указанным уникальным идентификатором
    auto ptr_component_ = this->components_list_;
    while (ptr_component_) {
        if (ptr_component_->component_->component_uid() == component_uid_) {
            break;
        }
        ptr_component_ = ptr_component_->next_component_;
    }

    //(?) Если такая компонента в контейнере зарегистрирована, то удаляем ее
    if (ptr_component_) {
        this->remove_component(ptr_component_);
    }
}

//-----------------------------------------------------------------------

void register_components_container::remove_component(list_component *ptr_component_) {
    //(?) Если у текущего компонента имеется указатель на предыдущую компоненту, то устанавливаем для
    // него следующим следующий удаляемого
    // В противном случае указываем, что начало списка компонент - это следующая компонента относительно удаляемого
    if (ptr_component_->prev_component_) {
        ptr_component_->prev_component_->next_component_ = ptr_component_->next_component_;
    } else {
        this->components_list_ = ptr_component_->next_component_;
    }

    //(?) Если у удаляемого компонента определен указатель на следующий, то для следующего устанавливаем указатель
    // на предыдущий относительно удаляемого
    if (ptr_component_->next_component_) {
        ptr_component_->next_component_->prev_component_ = ptr_component_->prev_component_;
    }

    // Освобождаем выделенную память для компоненты (при этом сама компонента может остаться в живых)
    delete ptr_component_;
}

//-----------------------------------------------------------------------

void register_components_container::clear() {
    //(?>) Освобождаем весь список компонент
    while (this->components_list_) {
        this->remove_component(this->components_list_);
    }
}
