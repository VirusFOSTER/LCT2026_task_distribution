#ifndef TASK_DISTRIBUTION_INSTANCE_TAG_HPP
#define TASK_DISTRIBUTION_INSTANCE_TAG_HPP

namespace td {
namespace types {
/** ------------------------------------------------------------------------------------
 * @brief The tg_instance enum - метки исполнителей
 * Данный метки отображают уровень доступности каждого исоплнителя
 ---------------------------------------------------------------------------------------*/
enum tg_instance {
    _tg_status_unknown_ = 0x00,         /// <--- статус исполнителя не известен
    _tg_status_unavailable_ = 0x01,     /// <--- исполнитель недоступен
    _tg_status_work_ = 0x02,            /// <--- исполнитель выполняет задачу
    _tg_status_free_ = 0x04             /// <--- исполнитель не имеет назначенных задач
};
}       /// <--- types
}   /// <--- td

#endif
