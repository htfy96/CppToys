#ifndef HT_AUTODIFF_HPP
#define HT_AUTODIFF_HPP

#if __cplusplus < 201402L
#error Must be compiled with C++14 standard (-std=c++14 under g++/clang++)
#endif

#include <type_traits>



namespace adiff
{
    enum class ExprType
    {
        Number,
        Variable,
        Plus,
        Product
    };

    template<int C>
    struct Number : std::integral_constant<int, C>
    {
        using type = std::integral_constant<ExprType, ExprType::Number>;
    };

    enum struct VarName
    {
        x,
        y,
        z
    };

    template<VarName vn>
        struct Var
    {
        using name = std::integral_constant<VarName, vn>;
        using type = std::integral_constant<ExprType, ExprType::Variable>;
    };


    template<typename T1, typename T2>
        struct Plus
    {
        static_assert(std::is_same<typename T1::type::value_type, ExprType>::value, "All Operators of plus should be Expr");
        static_assert(std::is_same<typename T2::type::value_type, ExprType>::value, "All Operators of plus should be Expr");
        using type = std::integral_constant<ExprType, ExprType::Plus>;
        using left = T1;
        using right = T2;
    };

    template<typename T1, typename T2>
        struct Product
        {
            static_assert(std::is_same<typename T1::type::value_type, ExprType>::value, "All operators of product should be expr");
            static_assert(std::is_same<typename T2::type::value_type, ExprType>::value, "All operators of product should be expr");
            using type = std::integral_constant<ExprType, ExprType::Product>;
            using left = T1;
            using right = T2;
        };


    template<typename T, typename TType = typename T::type>
        struct ConstantFold // common types cannot be folded
        {
            using type = T;
        };


    namespace detail
    {
        template<typename NumNode, typename CommonNode, typename = typename CommonNode::left::type, typename = typename CommonNode::right::type >
            struct PartialFold2
            {
                using type = Plus<NumNode, CommonNode>;
            };

        template<typename NumNode, typename CommonNode, typename FoldNode1>
            struct PartialFold2<NumNode, CommonNode, FoldNode1, std::integral_constant<ExprType, ExprType::Number> >
            {
                using type = Plus<Number<NumNode::value + CommonNode::right::value>, typename CommonNode::left>;
            };

        template<typename NumNode, typename CommonNode, typename FoldNode2>
            struct PartialFold2<NumNode, CommonNode, std::integral_constant<ExprType, ExprType::Number>, FoldNode2>
            {
                using type = Plus<Number<NumNode::value + CommonNode::left::value>, typename CommonNode::right>;
            };
        template<typename NumNode, typename CommonNode, typename = typename CommonNode::type>
            struct PartialFold
            {
                using type = Plus<NumNode, CommonNode>;
            };

        template<typename NumNode, typename CommonNode>
            struct PartialFold<NumNode, CommonNode, std::integral_constant<ExprType, ExprType::Plus>>
            {
                using type = typename PartialFold2<NumNode, CommonNode>::type;
            };

        template<typename L, typename R, bool isLeftNumber, bool isRightNumber>
            struct CFIfLeftRightNumber
            {
                using type = Plus<L, R>;
            };

        template<typename L, typename R> 
            struct CFIfLeftRightNumber<L, R, true, true>
            {
                using type = Number<L::value + R::value>;
            };

        template<typename L, typename R> 
            struct CFIfLeftRightNumber<L, R, true, false>
            {
                using type = typename PartialFold<L, R>::type;
            };

        template<typename L, typename R> 
            struct CFIfLeftRightNumber<L, R, false, true>
            {
                using type = typename PartialFold<R, L>::type;
            };

        template<typename L, typename R>
            struct CFLogic
            {
                using type = typename CFIfLeftRightNumber<L, R, L::type::value == ExprType::Number, R::type::value == ExprType::Number>::type;
            };


    }

    template<typename T>
        struct ConstantFold<T, std::integral_constant<ExprType, ExprType::Plus>>
        {
            friend struct ConstantFold;
            private:
                using L = typename ConstantFold<typename T::left>::type;
                using R = typename ConstantFold<typename T::right>::type;
            public:
            using type = typename detail::CFLogic<L,R>::type;
        };

    template<typename T>
        struct ConstantFold<T, std::integral_constant<ExprType, ExprType::Product>>
        {
            friend struct ConstantFold;
            private:
                using L = typename ConstantFold<typename T::left>::type;
                using R = typename ConstantFold<typename T::right>::type;
            public:
            // TODO
            using type = Product<L, R>;
        };

}
#endif
