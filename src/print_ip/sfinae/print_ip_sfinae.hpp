#ifndef PRINT_IP_SFINAE_HPP
#define PRINT_IP_SFINAE_HPP

#include <iostream>
#include <type_traits>
#include <string>
#include <tuple>
#include <utility>

/**
 * @brief Вывод IP-адреса для целочисленных типов
 *
 * Число выводится побайтово, начиная со старшего байта.
 * Байты разделяются точкой.
 *
 * @tparam T Целочисленный тип (int8_t, int16_t, int32_t, int64_t и т.д.)
 * @param value Число для вывода
 */
template <typename T>
auto print_ip(T value) -> std::enable_if_t<std::is_integral_v<T>, void>
{
    constexpr size_t byte_count = sizeof(T);

    for (size_t i = 0; i < byte_count; ++i)
    {
        if (i > 0)
        {
            std::cout << ".";
        }

        std::cout << static_cast<unsigned int>((value >> (8 * (byte_count - 1 - i))) & 0xFF);
    }

    std::cout << std::endl;
}

/**
 * @brief Вывод IP-адреса для строк
 *
 * Строка выводится без изменений.
 *
 * @tparam T Должен быть std::string
 * @param value Строка для вывода
 */
template <typename T>
auto print_ip(const T &value) -> std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, void>
{
    std::cout << value << std::endl;
}

/**
 * @brief Проверка, является ли тип контейнером
 *
 * Основной шаблон возвращает false_type.
 * Специализация активируется для типов, имеющих методы begin() и end().
 * std::string исключается.
 *
 * @tparam T Проверяемый тип
 */
template <typename, typename = void>
struct is_container : std::false_type
{
};

template <typename T>
struct is_container<T, std::void_t<
                           decltype(std::declval<T &>().begin()),
                           decltype(std::declval<T &>().end())>> : std::negation<std::is_same<std::decay_t<T>, std::string>>
{
};

/**
 * @brief Вывод IP-адреса для контейнеров
 *
 * Элементы контейнера выводятся последовательно через точку.
 * Поддерживаются std::vector, std::list, std::set, std::map и другие.
 *
 * @tparam T Тип контейнера (должен иметь методы begin() и end())
 * @param container Контейнер для вывода
 */
template <typename T, typename = std::enable_if_t<is_container<T>::value>>
void print_ip(const T &container)
{
    auto it = container.begin();
    const auto end = container.end();

    if (it != end)
    {
        std::cout << *it;
        ++it;
    }

    for (; it != end; ++it)
    {
        std::cout << "." << *it;
    }

    std::cout << std::endl;
}

#endif