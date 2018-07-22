#include <istream>
#include <vector>
#include <optional>
#include <array>
#include <deque>
#include <algorithm>

#include <iostream>

namespace LL1
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // aliases, improving readability
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    using int_type_t = typename std::char_traits<T>::int_type;

    template <typename T>
    using optional_token_t = std::optional<int_type_t<T>>;

    //template <typename T, size_t N>
    //using token_set_t = std::array<T, N>;

    template <typename T>
    using input_stream_t = std::basic_istream<T>;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // error handling
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct code_position
    {
        unsigned row;
        unsigned col;
    };

    struct unexpected_token
    {
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token sequences
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct dynamic_token_sequence
    {

    public:

        using int_type = int_type_t<T>;


        void push_back(int_type chr)
        {
            this->storage.push_back(chr);
        }

        dynamic_token_sequence& operator +=(int_type tok)
        {
            this->storage.push_back(tok);

            return *this;
        }

        dynamic_token_sequence& operator+=(const dynamic_token_sequence& other)
        {
            this->storage.insert(
                std::end(this->storage),
                std::begin(other.storage),
                std::end(other.storage)
            );

            return *this;
        }

        const int_type& operator[](size_t i) const
        {
            return this->storage[0];
        }

        auto size() const
        {
            return this->storage.size();
        }

    private:

        std::vector<int_type> storage;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // negated token
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct negated_token
    {
        explicit constexpr negated_token(T tok)
            : tok{ tok }
        { }

        constexpr T token() const
        {
            return this->tok;
        }

    private:
    
        T tok;
    };

    template <typename T1, typename T2>
    constexpr bool operator==(negated_token<T1> tok1, negated_token<T2> tok2)
    {
        return tok1.token() == tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(negated_token<T1> tok1, negated_token<T2> tok2)
    {
        return tok1.token() != tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator==(negated_token<T1> tok1, T2 tok2)
    {
        return tok1.token() != tok2;
    }

    template <typename T1, typename T2>
    constexpr bool operator==(T1 tok1, negated_token<T2> tok2)
    {
        return tok1 != tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(T1 tok1, negated_token<T2> tok2)
    {
        return tok1 == tok2.token();
    }

    template <typename T1, typename T2>
    constexpr bool operator!=(negated_token<T1> tok1, T2 tok2)
    {
        return tok1.token() == tok2;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct is_char_type
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<T, char>,
                std::is_same<T, wchar_t>,
                std::is_same<T, char16_t>,
                std::is_same<T, char32_t>,
                std::is_same<T, negated_token<char>>,
                std::is_same<T, negated_token<wchar_t>>,
                std::is_same<T, negated_token<wchar_t>>,
                std::is_same<T, negated_token<char16_t>>,
                std::is_same<T, negated_token<char32_t>>
            >
        >
    { };

    template <typename T>
    struct is_int_type
        : std::bool_constant<
            std::disjunction_v<
                std::is_same<T, int_type_t<char>>,
                std::is_same<T, int_type_t<wchar_t>>,
                std::is_same<T, int_type_t<char16_t>>,
                std::is_same<T, int_type_t<char32_t>>,
                std::is_same<T, negated_token<int_type_t<char>>>,
                std::is_same<T, negated_token<int_type_t<wchar_t>>>,
                std::is_same<T, negated_token<int_type_t<wchar_t>>>,
                std::is_same<T, negated_token<int_type_t<char16_t>>>,
                std::is_same<T, negated_token<int_type_t<char32_t>>>
            >
        >
    { };

    template <typename T>
    struct is_token_type
        : std::bool_constant<
            std::disjunction_v<
                is_char_type<T>,
                is_int_type<T>
            >
        >
    { };

    template <typename... Ts>
    struct are_token_types
        : std::bool_constant<
            std::disjunction_v<
                is_token_type<Ts>...
            >
        >
    { };

    // checks if T2 fits into T1
    template <typename T1, typename T2>
    struct fits_into
        : std::bool_constant<
            sizeof(T1) >= sizeof(T2)
        >
    { };

    template <typename T1, typename T2, typename = void>
    struct equality_comparable : std::false_type
    { };

    template <typename T1, typename T2>
    struct equality_comparable<
        T1, T2, std::void_t<decltype(std::declval<T1>() == std::declval<T2>())>
    > : std::true_type
    { };

    template <typename T1, typename T2>
    struct is_compatible_token_type
        : std::bool_constant<
            std::conjunction_v<
                are_token_types<T1, T2>,
                equality_comparable<T1, T2>
            >
        >
    { };

    template <typename T, typename... Ts>
    struct are_compatible_token_types
        : std::bool_constant<
            std::conjunction_v<
                is_compatible_token_type<T, Ts>...
            >
        >
    { };

    template <typename T>
    constexpr bool is_char_type_v = is_char_type<T>::value;

    template <typename T>
    constexpr bool is_int_type_v = is_int_type<T>::value;

    template <typename T>
    constexpr bool is_token_type_v = is_token_type<T>::value;

    template <typename... Ts>
    constexpr bool are_token_types_v = are_token_types<Ts...>::value;

    template <typename T1, typename T2>
    constexpr bool is_compatible_token_type_v = is_compatible_token_type<T1, T2>::value;

    template <typename... Ts>
    constexpr bool are_compatible_token_types_v = are_compatible_token_types<Ts...>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined tokens
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace tokens
    {
        constexpr char line_feed = 0xA;
        constexpr char carriage_return = 0xD;
        constexpr char space = 0x20;
        constexpr char tabulator = 0x9;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // predefined token sets
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace sets
    {
        constexpr char space[]{ ' ', '\t', '\n' };

        constexpr char blank[]{ ' ', '\t' };

        constexpr char digit[]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set search functions
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    constexpr bool disjunction(T expr)
    {
        return expr ? true : false;
    }

    template <typename T, typename... Ts>
    constexpr bool disjunction(T expr, Ts... exprs)
    {
        return disjunction(expr) || disjunction(exprs...);
    }

    template <
        typename T1,
        typename T2,
        size_t... I,
        typename = std::enable_if<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool contains_impl(const T1& set, T2 tok, std::index_sequence<I...>)
    {
        return disjunction((std::get<I>(set) == tok)...);
    }

    template <
        typename... Ts,
        typename T,
        typename = std::enable_if<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool contains(const std::tuple<Ts...>& set, T tok)
    {
        return contains_impl(set, tok, std::index_sequence_for<Ts...>{});
    }

    template <
        typename T1,
        size_t N,
        typename T2,
        typename = std::enable_if<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool contains(const std::array<T1, N>& set, T2 tok)
    {
        return contains_impl(set, tok, std::make_index_sequence<N>{});
    }

    template <
        typename T1,
        size_t N,
        typename T2,
        typename = std::enable_if<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool contains(const T1 (&set)[N], T2 tok)
    {
        return std::find(std::begin(set), std::end(set), tok) != std::end(set);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // logical token and token set operations
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename = std::enable_if_t<is_token_type_v<T>>>
    auto not_(T tok)
    {
        return negated_token{ tok };
    }

    template <typename T, typename = std::enable_if_t<is_token_type_v<T>>>
    auto negate(T tok)
    {
        return negated_token{ tok };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // token set type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1, typename T2, typename = void>
    struct is_compatible_token_set_type : std::false_type
    { };

    template <typename T1, typename T2>
    struct is_compatible_token_set_type<
        T1, T2, std::void_t<decltype(contains(std::declval<T2>(), std::declval<T1>()))>
    > : std::true_type
    { };

    template <typename T1, typename T2>
    constexpr bool is_compatible_token_set_type_v = is_compatible_token_set_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // comparison type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1, typename T2>
    struct is_compatible_comparison_type
        : std::bool_constant<
            std::disjunction_v<
                std::conditional_t<
                    is_token_type_v<T2>,
                    is_compatible_token_type<T1, T2>,
                    std::false_type
                >,
                is_compatible_token_set_type<T1, T2>
            >
        >
     { };

     template <typename T1, typename T2>
     constexpr bool is_compatible_comparison_type_v = is_compatible_comparison_type<T1, T2>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // input steam type traits
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T1, typename = void>
    struct is_compatible_input_stream_type : std::false_type
    { };

    template <typename T1>
    struct is_compatible_input_stream_type<T1, std::void_t<decltype(std::declval<T1>().get())>>
        : is_int_type<decltype(std::declval<T1>().get())>::type
    { };

    template <typename T1>
    constexpr bool is_compatible_input_stream_type_v = is_compatible_input_stream_type<T1>::value;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_token_type_v<T1>>,
        typename = std::enable_if_t<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool is(T1 tok, T2 cmp) noexcept
    {
        return tok == cmp;
    }

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_token_type_v<T1>>,
        typename = std::enable_if_t<is_compatible_token_set_type_v<T1, T2>>
    >
    constexpr bool is(T1 tok, const T2& cmp) noexcept
    {
        return contains(cmp, tok);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_token_type_v<T1, T2>>
    >
    constexpr bool is(TT<T1>& bis, T2 cmp)
    {
        return is(bis.peek(), cmp);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_token_set_type_v<T1, T2>>
    >
    constexpr bool is(TT<T1>& bis, const T2& cmp)
    {
        return is(bis.peek(), cmp);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_token_type_v<T1>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr bool is_not(T1 tok, const T2& cmp) noexcept
    {
        return !is(tok, cmp);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_input_stream_type_v<TT<T1>>>,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr bool is_not(TT<T1>& bis, const T2& cmp)
    {
        return !is(bis, cmp);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T1,
        typename T2,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2>>
    >
    constexpr bool is_one_of(T1 tok, T2 cmp1) noexcept
    {
        return is(tok, cmp1);
    }

    template <
        typename T1,
        typename T2,
        typename... Ts,
        typename = std::enable_if_t<are_compatible_token_types_v<T1, T2, Ts...>>
    >
    constexpr bool is_one_of(T1 tok, T2 cmp1, Ts... cmp) noexcept
    {
        return is(tok, cmp1) || is_one_of(tok, cmp...);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_one_of(TT<T>& bis, Ts... cmp)
    {
        return is_one_of(bis.peek(), cmp...);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'is_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_none_of(T tok, Ts... cmp) noexcept
    {
        return !is_one_of(tok, cmp...);
    }

    template <
    template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr bool is_none_of(TT<T>& bis, Ts... cmp)
    {
        return !is_one_of(bis, cmp...);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<is_token_type_v<T>>
    >
    constexpr auto read(TT<T>& bis)
    {
        return bis.get();
    }

    template <
       template <typename...> typename TT,
       typename T,
       typename = std::enable_if_t<is_token_type_v<T>>
    >
    constexpr auto read(TT<T>& bis, code_position& pos)
    {
       auto tok = bis.get();
    
       if(tok == tokens::line_feed)
       {
           pos.row++;
           pos.col = 0;
       }
       else
       {
           pos.col++;
       }

       return tok;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, const T2& cmp)
    {
        if(is(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        if(is(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, const T2& cmp)
    {
        if(is_not(bis, cmp))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr optional_token_t<T1> read_if_not(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        if(is_not(bis, cmp))
            return read(bis, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            return read(bis, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_if_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_none_of(TT<T>& bis, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            return read(bis);
        else
            return std::nullopt;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr optional_token_t<T> read_if_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_none_of(bis, pos, cmp...))
            return read(bis, pos);
        else
            return std::nullopt;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, const T2& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while(TT<T1>& bis, code_position& pos, const T2& cmp)
    {        
        dynamic_token_sequence<T1> tokseq;
        
        while(is(bis, cmp))
            tokseq.push_back(read(bis, pos));
            
        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, const T2& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto read_while_not(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        dynamic_token_sequence<T1> tokseq;
        
        while(is_not(bis, cmp))
            tokseq.push_back(read(bis, pos));

        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_one_of(TT<T>& bis, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
        
        while(is_one_of(bis, cmp...))
            tokseq.push_back(read(bis));

        return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
        
        while(is_one_of(bis, cmp...))
            tokseq.push_back(read(bis, pos));
            
        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'read_while_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
       template <typename...> typename TT,
       typename T,
       typename... Ts,
       typename = std::enable_if_t<sizeof...(Ts) != 0>,
       typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_none_of(TT<T>& bis, Ts... cmp)
    {
       dynamic_token_sequence<T> tokseq;

       while(is_none_of(bis, cmp...))
           tokseq.push_back(read(bis));

       return tokseq;
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    constexpr auto read_while_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        dynamic_token_sequence<T> tokseq;
    
        while(is_none_of(bis, cmp...))
            tokseq.push_back(read(bis, pos));
    
        return tokseq;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
       constexpr void ignore(TT<T>& bis)
    {
        read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename = std::enable_if_t<are_token_types_v<T>>
    >
    constexpr void ignore(TT<T>& bis, code_position& pos)
    {
        read(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
       template <typename...> typename TT,
       typename T1,
       typename T2,
       typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    bool ignore_if(TT<T1>& bis, const T2& cmp)
    {
        return is(bis, cmp) ? ignore(bis), true : false;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    bool ignore_if(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        return is(bis, cmp) ? ignore(bis, pos), true : false;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    bool ignore_if_not(TT<T1>& bis, const T2& cmp)
    {
        return is_not(bis, cmp) ? ignore(bis), true : false;
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    bool ignore_if_not(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        return is_not(bis, cmp) ? ignore(bis, pos), true : false;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_if_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_none_of(input_stream_t<T>& bis, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_if_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_none_of(bis, cmp...))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, const T2& cmp)
    {
        while(is(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    void ignore_while(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        while(is(bis, cmp))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, const T2& cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    void ignore_while_not(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        while(is_not(bis, cmp))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_one_of(TT<T>& bis, Ts... cmp)
    {
        while(is_one_of(bis, cmp...))
            ignore(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        while(is_one_of(bis, cmp...))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'ignore_while_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_none_of(TT<T>& bis, Ts... cmp)
    {
        while(is_none_of(bis, cmp...))
            ignore(bis);
    }
    
    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void ignore_while_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        while(is_none_of(bis, cmp...))
            ignore(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, const T2& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};
        
        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_not' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, const T2& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T1,
        typename T2,
        typename = std::enable_if_t<is_compatible_comparison_type_v<T1, T2>>
    >
    constexpr auto expect_not(TT<T1>& bis, code_position& pos, const T2& cmp)
    {
        if(is_not(bis, cmp))
            throw unexpected_token{};

        return read(bis, pos);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_one_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_one_of(TT<T>& bis, Ts... cmp)
    {
        if(is_not_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_one_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_not_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis, pos);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'expect_none_of' overloads
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_none_of(TT<T>& bis, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis);
    }

    template <
        template <typename...> typename TT,
        typename T,
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) != 0>,
        typename = std::enable_if_t<are_compatible_token_types_v<T, Ts...>>
    >
    void expect_none_of(TT<T>& bis, code_position& pos, Ts... cmp)
    {
        if(is_one_of(bis, cmp...))
            throw unexpected_token{};

        return read(bis, pos);
    }

}



