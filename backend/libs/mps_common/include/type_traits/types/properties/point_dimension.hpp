#ifndef SYSTEM_POINT_DIMESION_HPP
#define SYSTEM_POINT_DIMESION_HPP

#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/variant.hpp>
#include <boost/array.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/map.hpp>

#include "member_detector.hpp"
#include "type_detector.hpp"

GENERATE_REQUIRED_MEMBER_CLASS(x)
GENERATE_REQUIRED_MEMBER_CLASS(y)
GENERATE_REQUIRED_MEMBER_CLASS(z)

namespace mps {
    namespace type_traits {
    namespace types {
    namespace properties {

    /// Список размерностей точек
    using dimension_0D_t = boost::mpl::int_<0>;     /// points dimension is 0D
    using dimension_1D_t = boost::mpl::int_<1>;     /// points dimension is 1D
    using dimension_2D_t = boost::mpl::int_<2>;     /// points dimension is 2D
    using dimension_3D_t = boost::mpl::int_<3>;     /// points dimension is 3D

    using max_dimension_t = dimension_3D_t;

    /**
    * @brief The point_dimension_ struct - размерность точки типа T_point равна по умолчанию 0D
    */
    template <class T_point, class Enable = void>
    struct point_dimension_ {
        static constexpr uint8_t m_dimension = 0;
    };

    /**
    * @brief The point_dimension_ struct - размерность точки не является равной нулю
    * Тип точки содержит как минимум одно поле x, y или z
    */
    template <class T_point>
    struct point_dimension_ <
            T_point,
            typename boost::enable_if<
            boost::mpl::or_<has_field_x<T_point>, has_field_y<T_point>, has_field_z<T_point>>
            >::type
            > {
        /**
         * @brief The pm_dimension struct - увеличение размерности точки
         * размерность точки не увеличивается, если в структуре точки отсутсвует указанное поле
         */
        template <class U, class En = void>
        struct pm_dimension {
            using pl_dim = boost::mpl::int_<0>;
        };

        template <class U>
        struct pm_dimension <U,typename boost::enable_if<U>::type> {
            using pl_dim = boost::mpl::int_<1>;
        };

        /**
         * @brief dimension - функция определения размерности точек
         * @return размерность точки
         */
        static constexpr uint8_t dimension() {
            using res = boost::mpl::plus<
            /// Проверяем наличие полей в описаниии точки x, y, z
            typename pm_dimension<has_field_x<T_point>>::pl_dim,
            typename pm_dimension<has_field_y<T_point>>::pl_dim,
            typename pm_dimension<has_field_z<T_point>>::pl_dim
            >;

            return res::value;
        }

        static constexpr uint8_t m_dimension = dimension();     /// <--- Итоговая размерность точки

        /// Приоритеты координат точек x,y,z
        using x_priority = boost::mpl::int_<0>;
        using y_priority = boost::mpl::int_<1>;
        using z_priority = boost::mpl::int_<2>;

        /**
         * @brief The sign_coord_ struct - сигнатура рассматриваемой координаты
         * V - значение, P - приоритет
         */
        template <class V, class P>
        struct sign_coord_ {
            using priority = P;
            using value = V;
        };

        /**
         * @brief The has_field_signature struct - определение поля на наличие в структуре
         * По умолчанию считается, что указанного поля в структуре нет
         * Для определения поля передается условие его определения
         */
        template <class U, class En = void>
        struct has_field_signature {
            using has_require_field = boost::mpl::bool_<false>;
        };

        template <class U>
        struct has_field_signature <U, typename boost::enable_if<U>::type>{
            using has_require_field = boost::mpl::bool_<true>;
        };

        /// <--- Сигнатура координаты x
        using sign_coord_X_ = sign_coord_<
        typename has_field_signature<has_field_x<T_point>>::has_require_field,
        x_priority
        >;

        /// <--- Сигнатура координаты y
        using sign_coord_Y_ = sign_coord_<
        typename has_field_signature<has_field_y<T_point>>::has_require_field,
        y_priority
        >;

        /// <--- Сигнатура координаты z
        using sign_coord_Z_ = sign_coord_<
        typename has_field_signature<has_field_z<T_point>>::has_require_field,
        z_priority
        >;

        /// <--- Полная сигнатура размерности
        using sign_dimension = typename boost::variant<sign_coord_X_,sign_coord_Y_,sign_coord_Z_>::types;

        /**
         * @brief The current_signature_ struct - текущая сигнатура координаты
         * Вызывается в случае, если размерность либо превышает максимально допустимую, либо меньше или равна 0
         */
        template <class Sign, class En = void>
        struct current_signature_ {};

        /**
         * @brief The current_signature_ struct - текущая сигнатура координаты
         * Вызывается в случае, если выполняется ряд следующих условий:
         *  - сигнатура размерности больше либо равна 0D;
         *  - сигнатура размерности меньше максимального значения (3D).
         */
        template <class Sign>
        struct current_signature_ <
                Sign,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign, max_dimension_t>
                >
                >::type
                > {
            using c_sign = typename boost::mpl::at<sign_dimension,Sign>::type;
            using v_sign = typename c_sign::value;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в случае, когда карта сигнатуры полностью сформирована
         */
        template <class T_map, class Sign, class Dimn, class En = void>
        struct make_pair_act_map {
            using act_dimension = T_map;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в том случае, когда карта сигнатуры еще не сформирована и при этом имеется в структуре,
         * описывающей точку, поле под текущим порядковым номером сигнатуры
         */
        template <class T_map, class Sign, class Dimn>
        struct make_pair_act_map <
                T_map, Sign, Dimn,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign,max_dimension_t>,
                boost::is_same<boost::mpl::bool_<true>,typename current_signature_<Sign>::v_sign>
                >
                >::type
                > {
            using new_map = typename boost::mpl::insert<
            T_map,
            boost::mpl::pair<Dimn,typename current_signature_<Sign>::c_sign>
            >::type;

            using act_dimension = typename make_pair_act_map<
            new_map,
            boost::mpl::plus<Sign,boost::mpl::int_<1>>,
            boost::mpl::plus<Dimn,boost::mpl::int_<1>>
            >::act_dimension;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в том случае, когда карта сигнатуры еще не сформирована и при этом имеется в структуре,
         * описывающей точку, отсутствует поле под текущим порядковым номером сигнатуры
         */
        template <class T_map, class Sign, class Dimn>
        struct make_pair_act_map <
                T_map, Sign, Dimn,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign,max_dimension_t>,
                boost::is_same<boost::mpl::bool_<false>,typename current_signature_<Sign>::v_sign>
                >
                >::type
                > {
            using act_dimension = typename make_pair_act_map<
            T_map,boost::mpl::plus<Sign,boost::mpl::int_<1>>,Dimn
            >::act_dimension;
        };

        /// <--- Карта сигнатуры полной размерности (рассматриваются все размерности)
        /// Суть данной карты заключается в отображении информации о наличии того или иного поля структуры,
        /// описывающей точку
        /// (например: для структуры point { x,z } будет установлена карта сигнатуры: { 1,0,1 },
        /// что означает наличие полей x и z, а также отсутствие поля y)
        using map_init_t = boost::mpl::map<>;
        using act_dimension = typename make_pair_act_map<
        map_init_t, boost::mpl::int_<0>, boost::mpl::int_<0>
        >::act_dimension;
    };


    /**
    * @brief The point_dimension_ struct - размерность точки не является равной нулю
    * Тип точки описывается в виде boost::array или std::array
    */
    template <class T_point>
    struct point_dimension_ <
            T_point,
            typename boost::enable_if<
            boost::mpl::or_<
            type_detector::is_boost_array<T_point>,
            type_detector::is_std_array<T_point>
            >
            >::type
            > {
        /// Приоритеты координат точек x,y,z
        using x_priority = boost::mpl::int_<0>;
        using y_priority = boost::mpl::int_<1>;
        using z_priority = boost::mpl::int_<2>;

        /**
         * @brief The sign_coord_ struct - сигнатура рассматриваемой координаты
         * V - значение, P - приоритет
         */
        template <class V, class P>
        struct sign_coord_ {
            using priority = P;
            using value = V;
        };

        /**
         * @brief The has_field_signature struct - определение поля на наличие в структуре
         * По умолчанию считается, что указанного поля в структуре нет
         * Для определения поля передается условие его определения
         */
        template <class U, class En = void>
        struct has_field_signature {
            using has_require_field = boost::mpl::bool_<false>;
        };

        template <class U>
        struct has_field_signature <U, typename boost::enable_if<U>::type>{
            using has_require_field = boost::mpl::bool_<true>;
        };

        static constexpr uint8_t m_dimension = T_point::size();     /// <--- Итоговая размерность точки

        /// <--- Сигнатура координат X, Y, Z
        using sign_coord_X_ = sign_coord_<boost::mpl::less<x_priority,boost::mpl::int_<m_dimension>>, x_priority>;
        using sign_coord_Y_ = sign_coord_<boost::mpl::less<y_priority,boost::mpl::int_<m_dimension>>, y_priority>;
        using sign_coord_Z_ = sign_coord_<boost::mpl::less<z_priority,boost::mpl::int_<m_dimension>>, z_priority>;

        using sign_dimension = typename boost::variant<sign_coord_X_,sign_coord_Y_,sign_coord_Z_>::types;

        /**
         * @brief The current_signature_ struct - текущая сигнатура координаты
         * Вызывается в случае, если размерность либо превышает максимально допустимую, либо меньше или равна 0
         */
        template <class Sign, class En = void>
        struct current_signature_ {};

        /**
         * @brief The current_signature_ struct - текущая сигнатура координаты
         * Вызывается в случае, если выполняется ряд следующих условий:
         *  - сигнатура размерности больше либо равна 0D;
         *  - сигнатура размерности меньше максимального значения (3D).
         */
        template <class Sign>
        struct current_signature_ <
                Sign,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign,boost::mpl::int_<m_dimension>>
                >
                >::type
                > {
            using c_sign = typename boost::mpl::at<sign_dimension,Sign>::type;
            using v_sign = typename c_sign::value;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в случае, когда карта сигнатуры полностью сформирована
         */
        template <class T_map, class Sign, class Dimn, class En = void>
        struct make_pair_act_map {
            using act_dimension = T_map;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в том случае, когда карта сигнатуры еще не сформирована и при этом имеется в структуре,
         * описывающей точку, поле под текущим порядковым номером сигнатуры
         */
        template <class T_map, class Sign, class Dimn>
        struct make_pair_act_map <
                T_map, Sign, Dimn,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign,boost::mpl::int_<m_dimension>>,
                boost::is_same<boost::mpl::bool_<true>,typename current_signature_<Sign>::v_sign>
                >
                >::type
                > {
            using new_map = typename boost::mpl::insert<
            T_map,
            boost::mpl::pair<Dimn,typename current_signature_<Sign>::c_sign>
            >::type;

            using act_dimension = typename make_pair_act_map<
            new_map,
            boost::mpl::plus<Sign,boost::mpl::int_<1>>,
            boost::mpl::plus<Sign,boost::mpl::int_<1>>
            >::act_dimension;
        };

        /**
         * @brief The make_pair_act_map struct - генератор карты сигнатуры полной размерности точки
         * Вызывается в том случае, когда карта сигнатуры еще не сформирована и при этом имеется в структуре,
         * описывающей точку, отсутствует поле под текущим порядковым номером сигнатуры
         */
        template <class T_map, class Sign, class Dimn>
        struct make_pair_act_map <
                T_map, Sign, Dimn,
                typename boost::enable_if<
                boost::mpl::and_<
                boost::mpl::or_<
                boost::is_same<dimension_0D_t,Sign>,
                boost::mpl::less<dimension_0D_t,Sign>
                >,
                boost::mpl::less<Sign,boost::mpl::int_<m_dimension>>,
                boost::is_same<boost::mpl::bool_<false>,typename current_signature_<Sign>::v_sign>
                >
                >::type
                > {
            using act_dimension = typename make_pair_act_map<
            T_map,boost::mpl::plus<Sign,boost::mpl::int_<1>>,Dimn
            >::act_dimension;
        };

        /// <--- Карта сигнатуры полной размерности (рассматриваются все размерности)
        /// Суть данной карты заключается в отображении информации о наличии того или иного поля структуры,
        /// описывающей точку
        /// (например: для структуры point { x,z } будет установлена карта сигнатуры: { 1,0,1 },
        /// что означает наличие полей x и z, а также отсутствие поля y)
        using map_init_t = boost::mpl::map<>;
        using act_dimension = typename make_pair_act_map<
        map_init_t, boost::mpl::int_<0>, boost::mpl::int_<0>
        >::act_dimension;
    };

    /**
     * Структура для определения общих полей двух точек
     */
    template <class T_point_1, class T_point_2>
    struct common_points_dimension_ {
        using act_dimension_map_1 = typename point_dimension_<T_point_1>::act_dimension;
        using act_dimension_map_2 = typename point_dimension_<T_point_2>::act_dimension;

        template <class T_map, class T_iter_1, class Enable = void>
        struct make_pair_common_active {
            using common_act_dimension = T_map;
        };

        template <class T_map, class T_iter_1>
        struct make_pair_common_active<
                T_map, T_iter_1,
                typename boost::enable_if<
                boost::mpl::bool_<!boost::is_same<T_iter_1,typename boost::mpl::end<act_dimension_map_1>::type>::value>
                >::type
                > {
            template <class T_mp, class T_iter_2, class Enable = void>
            struct make_pair_common_active_in {
                using common_act_dimension_in = T_mp;
            };

            template <class T_mp, class T_iter_2>
            struct make_pair_common_active_in <
                    T_mp, T_iter_2,
                    typename boost::enable_if<
                    boost::mpl::bool_<!boost::is_same<T_iter_2,typename boost::mpl::end<act_dimension_map_2>::type>::value>
                    >::type
                    > {
                template <class T_mp_in, class T_itr_1, class T_itr_2, class Enable = void>
                struct gen_new_map {
                    using new_map = T_mp_in;
                };

                template <class T_mp_in, class T_itr_1, class T_itr_2>
                struct gen_new_map <
                        T_mp_in, T_itr_1, T_itr_2,
                        typename boost::enable_if<
                        boost::is_same<
                        typename boost::mpl::deref<T_itr_1>::type::second::priority,
                        typename boost::mpl::deref<T_itr_2>::type::second::priority
                        >
                        >::type
                        > {
                    using new_map = typename boost::mpl::insert<
                    T_mp_in,
                    boost::mpl::pair<
                    typename boost::mpl::deref<T_itr_1>::type::first,
                    typename boost::mpl::deref<T_itr_1>::type::second
                    >
                    >::type;
                };

                using new_map_in = typename gen_new_map<T_mp,T_iter_1,T_iter_2>::new_map;

                using common_act_dimension_in = typename make_pair_common_active_in<
                new_map_in, typename boost::mpl::next<T_iter_2>::type>::common_act_dimension_in;
            };

        using new_common_map = typename make_pair_common_active_in<T_map,
        typename boost::mpl::begin<act_dimension_map_2>::type>::common_act_dimension_in;

        using common_act_dimension = typename make_pair_common_active<
        new_common_map,
        typename boost::mpl::next<T_iter_1>::type>::common_act_dimension;
        };

        using common_map_init_t = boost::mpl::map<>;
        using common_act_dimension = typename make_pair_common_active<
        common_map_init_t, typename boost::mpl::begin<act_dimension_map_1>::type>::common_act_dimension;
    };
    }               /// <--- properties
    }           /// <--- types
    }       /// <--- type_traits
}   /// <--- mps

#endif
