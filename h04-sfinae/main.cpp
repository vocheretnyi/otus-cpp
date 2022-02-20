#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <type_traits>
#include <iostream>
#include <array>
#include <iterator>


// Tries to make is_string traits
//
// is_string
//    template<typename T, typename Fake = void>
//    struct is_string : std::false_type {
//    };
//
//    template<typename T>
//    struct is_string<T, std::enable_if_t<std::is_same_v<std::void_t<decltype(std::declval<T&>().c_str())>, void>>>
//            : std::true_type {
//    };
//
// template<typename T>
// constexpr bool is_string_v = is_string<T>::value;
//
// template<typename T, std::enable_if_t<my::is_string_v < T>, bool> = true>
// void print_ip(const T& arg) {
//    std::cout << arg << "\n";
// }


namespace TuplePrinter {

    // helper function to print a tuple of any size
    template<class Tuple, std::size_t N>
    struct TuplePrinter {
        static void print(const Tuple& t) {
            TuplePrinter<Tuple, N - 1>::print(t);
            std::cout << ", " << std::get<N - 1>(t);
        }
    };

    template<class Tuple>
    struct TuplePrinter<Tuple, 1> {
        static void print(const Tuple& t) {
            std::cout << std::get<0>(t);
        }
    };

    template<
            typename T,
            class... Args,
            typename = std::enable_if_t<
                    std::conjunction_v<
                            std::is_same<T, Args>...
                    >
            >
    >
    void print(const std::tuple<T, Args...>& t) {
        std::cout << "(";
        TuplePrinter<decltype(t), sizeof...(Args) + 1>::print(t);
        std::cout << ")\n";
    }
} // namespace TuplePrinter

namespace my {
    // is_container
    template<typename T, typename Fake = void>
    struct is_container : std::false_type {
    };

    // TODO: it's a not always working trick.
    template<typename T>
    struct is_container<T, std::enable_if_t<std::is_same_v<std::void_t<typename T::const_iterator>, void>>>
            : std::true_type {
    };

    template<typename T>
    constexpr bool is_container_v = is_container<T>::value;

    // is_tuple
    template<typename T>
    struct is_tuple : std::false_type {
    };

    template<typename... Ts>
    struct is_tuple<std::tuple<Ts...>> : std::true_type {
    };

    template<typename T>
    constexpr bool is_tuple_v = is_tuple<T>::value;
} // namespace my


/// \brief Printing `std::list` and `std::vector` containers as IP
/// \details An address represented as `std::list` or `std::vector` containers.
/// The full content of the container is displayed element by element and separated by `.` (dot character).
/// Items are displayed as is.
template<typename T, std::enable_if_t<my::is_container_v<T>, bool> = true>
void print_ip(const T& arg) {
    for (auto it = arg.cbegin(); it != arg.cend(); ++it) {
        std::cout << *it;
        if (std::next(it) != arg.cend()) {
            std::cout << '.';
        }
    }
    std::cout << "\n";
}


/// \brief Printing string as IP
/// \details The address represented as a string. Rendered as is, regardless of content.
template<
        template<typename, typename, typename> typename String,
        typename CharT = char,
        typename Traits = std::char_traits<CharT>,
        typename Allocator = std::allocator<CharT>,
        std::enable_if_t<
                std::is_same_v<
                        String<CharT, Traits, Allocator>,
                        std::basic_string<CharT, Traits, Allocator>
                >, bool
        > = true
>
void print_ip(const String<CharT, Traits, Allocator>& arg) {
    std::cout << arg << "\n";
}


/// \brief Printing `std::tuple` as IP
/// \details The address represented as a `std::tuple` provided that all types are the same.
/// The complete content is displayed element by element and separated by `.` (single dot character).
/// Items are displayed as is.
/// If the tuple types are not the same, an error should be thrown when compiling the code.
template<typename T, std::enable_if_t<my::is_tuple_v<T>, bool> = true>
void print_ip(const T& arg) {
    TuplePrinter::print(arg);
}


/// \brief Printing integer as IP
/// \details The address represented as an arbitrary integer type.
/// Output unsigned byte by byte, high byte first, with `.` (dot character) as delimiter.
/// All bytes of the number are output.
template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
void print_ip(const T& arg) {
    auto argT = arg;
    constexpr static size_t bytesNum = sizeof(T);
    std::array<uint8_t, bytesNum> arr = {};

    for (size_t i = 0; i < bytesNum; ++i) {
        arr[i] = argT & 0xFF;
        argT >>= 8;
    }

    for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
        std::cout << static_cast<uint16_t>(*it);
        if (std::next(it) != arr.rend()) {
            std::cout << '.';
        }
    }

    std::cout << "\n";
}

// TODO: cover ref, && cases?
int main() {
    print_ip(int8_t{-1}); // 255
    print_ip(int16_t{0}); // 0.0
    print_ip(int32_t{2130706433}); // 127.0.0.1
    print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"}); // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

//    print_ip(std::make_tuple(123, 456, "789", 0.4f)); // we should get compilation error.

    return 0;
}
