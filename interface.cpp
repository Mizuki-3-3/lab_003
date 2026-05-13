#include <deque.hpp>
#include <list_seq.hpp>
#include <array_seq.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cctype>
#include <variant>
#include <cmath>   // для std::abs, std::round, std::roundf

using namespace ftxui;

// ============================ Глобальные объекты деков ============================
deque<list_seq, int>   my_deque_list_int;
deque<list_seq, float> my_deque_list_float;
deque<list_seq, char>  my_deque_list_char;

deque<array_seq, int>   my_deque_array_int;
deque<array_seq, float> my_deque_array_float;
deque<array_seq, char>  my_deque_array_char;

// ============================ Глобальные переменные интерфейса ============================
std::vector<std::string> display_items;
int selected_item = 0, selected_container = 0, selected_data_type = 0, selected_mode = 0;
int selected_map_func = 0, selected_reduce_func = 0, selected_where_func = 0;
std::string last_result;

// Списки для выпадающих меню
std::vector<std::string> container = {"list_seq", "array_seq"};
std::vector<std::string> data_type  = {"int", "float", "char"};
std::vector<std::string> mode = {"append", "prepend", "insert", "map", "reduce", "where", "sort", "concat", "find", "find subdeque", "merge"};

std::vector<std::string> map_func_for_int = {"x+1", "x*2", "x-1", "x/2", "x = 0"};
std::vector<std::string> map_func_for_float = {"x+1", "x*2", "x-1", "x/2", "x = 0"};
std::vector<std::string> map_func_for_char = {"Изменить регистр", "заменить следующим char", "заменить предыдущим char"};

std::vector<std::string> where_func_for_int = {"x mod 3 == 0", "x <= 0", "x >= 0", "трехзначные", "четырехзначные"};
std::vector<std::string> where_func_for_float = {"x - abs(x) != 0", "x <= 0", "x >= 0"};
std::vector<std::string> where_func_for_char = {"Только заглавные", "Только строчные", "только буквы", "только цифры"};

std::vector<std::string> reduce_func_for_int = {"min", "max", "sum", "product"};
std::vector<std::string> reduce_func_for_float = {"min", "max", "sum", "product", "среднее арифметическое"};
std::vector<std::string> reduce_func_for_char = {"кол-во гласных", "кол-во цифр", "кол-во согласных", "самый большой ascii"};

// ============================ Вспомогательные функции ============================
template<typename V>
V parse_value(const std::string& s, bool& ok) {
    ok = true;
    if constexpr (std::is_same_v<V, int>) {
        try {
            size_t pos;
            int val = std::stoi(s, &pos);
            ok = (pos == s.size());
            return val;
        } catch(...) { ok = false; return 0; }
    } else if constexpr (std::is_same_v<V, float>) {
        try {
            size_t pos;
            float val = std::stof(s, &pos);
            ok = (pos == s.size());
            return val;
        } catch(...) { ok = false; return 0.0f; }
    } else if constexpr (std::is_same_v<V, char>) {
        ok = !s.empty();
        return ok ? s[0] : '\0';
    }
    ok = false;
    return V{};
}

template<typename T>
std::string to_string_impl(const T& value) {
    if constexpr (std::is_same_v<T, char>) {
        return std::string(1, value);
    } else if constexpr (std::is_same_v<T, float>) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    } else {
        return std::to_string(value);
    }
}

// Удаление элемента по индексу из дека (создаём новый дек и перемещаем)
template<template<typename> class Container, typename T>
void erase_at(deque<Container, T>& deq, unsigned idx) {
    if (idx >= deq.size()) return;
    deque<Container, T> temp;
    for (unsigned i = 0; i < deq.size(); ++i) {
        if (i != idx) temp.push_back(deq[i]);
    }
    deq = std::move(temp); // предполагаем, что перемещающее присваивание корректно
}

// Обновить отображаемые элементы (текущий дек -> display_items)
void update_display_items() {
    display_items.clear();
    auto add_from_deque = [&](auto& deq) {
        for (unsigned i = 0; i < deq.size(); ++i)
            display_items.push_back(to_string_impl(deq[i]));
    };
    if (selected_container == 0) { // list_seq
        switch (selected_data_type) {
            case 0: add_from_deque(my_deque_list_int); break;
            case 1: add_from_deque(my_deque_list_float); break;
            case 2: add_from_deque(my_deque_list_char); break;
        }
    } else { // array_seq
        switch (selected_data_type) {
            case 0: add_from_deque(my_deque_array_int); break;
            case 1: add_from_deque(my_deque_array_float); break;
            case 2: add_from_deque(my_deque_array_char); break;
        }
    }
    if (selected_item >= (int)display_items.size() && !display_items.empty())
        selected_item = (int)display_items.size() - 1;
    if (display_items.empty()) selected_item = 0;
}

void delete_selected() {
    if (selected_item < 0 || selected_item >= (int)display_items.size()) return;
    unsigned idx = selected_item;
    try {
        if (selected_container == 0) {
            switch (selected_data_type) {
                case 0: erase_at(my_deque_list_int, idx); break;
                case 1: erase_at(my_deque_list_float, idx); break;
                case 2: erase_at(my_deque_list_char, idx); break;
            }
        } else {
            switch (selected_data_type) {
                case 0: erase_at(my_deque_array_int, idx); break;
                case 1: erase_at(my_deque_array_float, idx); break;
                case 2: erase_at(my_deque_array_char, idx); break;
            }
        }
        update_display_items();
    } catch (const index_out_of_range&) {
        last_result = "Ошибка: индекс вне диапазона";
    } catch (const empty_container&) {
        last_result = "Ошибка: контейнер пуст";
    } catch (const null_ptr&) {
        last_result = "Ошибка: потеря памяти";
    } catch (const std::exception& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (...) {
        last_result = "Неизвестная ошибка";
    }
}

// Парсинг дека из строки вида "1,2,3" или "a,b,c"
template<template<typename> class Container, typename T>
deque<Container, T> parse_deque_from_string(const std::string& str) {
    deque<Container, T> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ',')) {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (token.empty()) continue;
        bool ok;
        T val = parse_value<T>(token, ok);
        if (ok) result.push_back(val);
        else throw invalid_argument();
    }
    return result;
}

// Основная функция применения операции
template<template<typename> class Container, typename T>
void apply_modify_op(deque<Container, T>* deq, const std::string& val_str, int idx,
                     const std::string& second_deque_str) {
    if (!deq) return;
    try {
        switch (selected_mode) {
            case 0: { // append
                bool ok;
                T val = parse_value<T>(val_str, ok);
                if (!ok) throw invalid_argument();
                deq->push_back(val);
                update_display_items();
                break;
            }
            case 1: { // prepend
                bool ok;
                T val = parse_value<T>(val_str, ok);
                if (!ok) throw invalid_argument();
                deq->push_front(val);
                update_display_items();
                break;
            }
            case 2: { // insert
                if (idx < 0 || (unsigned)idx > deq->size()) throw index_out_of_range();
                bool ok;
                T val = parse_value<T>(val_str, ok);
                if (!ok) throw invalid_argument();
                deq->insert(val, idx);
                update_display_items();
                break;
            }
            case 3: { // map
                std::function<T(const T&)> f;
                if constexpr (std::is_same_v<T, int>) {
                    switch (selected_map_func) {
                        case 0: f = [](int x) { return x + 1; }; break;
                        case 1: f = [](int x) { return x * 2; }; break;
                        case 2: f = [](int x) { return x - 1; }; break;
                        case 3: f = [](int x) { return x / 2; }; break;
                        case 4: f = [](int)   { return 0; }; break;
                        default: f = [](int x) { return x; };
                    }
                } else if constexpr (std::is_same_v<T, float>) {
                    switch (selected_map_func) {
                        case 0: f = [](float x) { return x + 1.0f; }; break;
                        case 1: f = [](float x) { return x * 2.0f; }; break;
                        case 2: f = [](float x) { return x - 1.0f; }; break;
                        case 3: f = [](float x) { return x / 2.0f; }; break;
                        case 4: f = [](float)   { return 0.0f; }; break;
                        default: f = [](float x) { return x; };
                    }
                } else if constexpr (std::is_same_v<T, char>) {
                    switch (selected_map_func) {
                        case 0:
                            f = [](char c) {
                                if (std::isupper((unsigned char)c)) return (char)std::tolower((unsigned char)c);
                                else if (std::islower((unsigned char)c)) return (char)std::toupper((unsigned char)c);
                                return c;
                            }; break;
                        case 1: f = [](char c) { return c + 1; }; break;
                        case 2: f = [](char c) { return c - 1; }; break;
                        default: f = [](char c) { return c; };
                    }
                }
                auto new_deq = deq->map(f);
                *deq = std::move(*new_deq);
                delete new_deq;
                update_display_items();
                break;
            }
            case 4: { // reduce (исправлен порядок аргументов)
                std::string result_str;
                if constexpr (std::is_same_v<T, int>) {
                    switch (selected_reduce_func) {
                        case 0: {
                            int min_val = deq->reduce((*deq)[0], [](int a, int b) { return std::min(a, b); });
                            result_str = std::to_string(min_val);
                            break;
                        }
                        case 1: {
                            int max_val = deq->reduce((*deq)[0], [](int a, int b) { return std::max(a, b); });
                            result_str = std::to_string(max_val);
                            break;
                        }
                        case 2: {
                            int sum = deq->reduce(0, [](int a, int b) { return a + b; });
                            result_str = std::to_string(sum);
                            break;
                        }
                        case 3: {
                            int prod = deq->reduce(1, [](int a, int b) { return a * b; });
                            result_str = std::to_string(prod);
                            break;
                        }
                        default: result_str = "неизвестная функция reduce";
                    }
                } else if constexpr (std::is_same_v<T, float>) {
                    switch (selected_reduce_func) {
                        case 0: {
                            float min_val = deq->reduce((*deq)[0], [](float a, float b) { return std::min(a, b); });
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << min_val;
                            result_str = oss.str();
                            break;
                        }
                        case 1: {
                            float max_val = deq->reduce((*deq)[0], [](float a, float b) { return std::max(a, b); });
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << max_val;
                            result_str = oss.str();
                            break;
                        }
                        case 2: {
                            float sum = deq->reduce(0.0f, [](float a, float b) { return a + b; });
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << sum;
                            result_str = oss.str();
                            break;
                        }
                        case 3: {
                            float prod = deq->reduce(1.0f, [](float a, float b) { return a * b; });
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << prod;
                            result_str = oss.str();
                            break;
                        }
                        case 4: {
                            float sum = deq->reduce(0.0f, [](float a, float b) { return a + b; });
                            float avg = sum / deq->size();
                            std::ostringstream oss; oss << std::fixed << std::setprecision(2) << avg;
                            result_str = oss.str();
                            break;
                        }
                        default: result_str = "неизвестная функция reduce";
                    }
                } else if constexpr (std::is_same_v<T, char>) {
                    auto is_vowel = [](char c) {
                        char lower = (char)std::tolower((unsigned char)c);
                        return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
                    };
                    auto is_consonant = [is_vowel](char c) {
                        return std::isalpha((unsigned char)c) && !is_vowel(c);
                    };
                    auto is_digit_char = [](char c) {
                        return std::isdigit((unsigned char)c);
                    };
                    switch (selected_reduce_func) {
                        case 0: {
                            int count = deq->reduce(0, [is_vowel](int acc, char c) { return acc + (is_vowel(c) ? 1 : 0); });
                            result_str = std::to_string(count);
                            break;
                        }
                        case 1: {
                            int count = deq->reduce(0, [is_digit_char](int acc, char c) { return acc + (is_digit_char(c) ? 1 : 0); });
                            result_str = std::to_string(count);
                            break;
                        }
                        case 2: {
                            int count = deq->reduce(0, [is_consonant](int acc, char c) { return acc + (is_consonant(c) ? 1 : 0); });
                            result_str = std::to_string(count);
                            break;
                        }
                        case 3: {
                            int max_ascii = deq->reduce((int)(*deq)[0], [](int a, char c) { return std::max(a, (int)c); });
                            result_str = std::to_string(max_ascii);
                            break;
                        }
                        default: result_str = "неизвестная функция reduce";
                    }
                }
                last_result = "Результат reduce: " + result_str;
                break;
            }
            case 5: { // where
                std::function<bool(const T&)> pred;
                if constexpr (std::is_same_v<T, int>) {
                    switch (selected_where_func) {
                        case 0: pred = [](int x) { return x % 3 == 0; }; break;
                        case 1: pred = [](int x) { return x <= 0; }; break;
                        case 2: pred = [](int x) { return x >= 0; }; break;
                        case 3: pred = [](int x) { return (x >= 100 && x <= 999) || (x <= -100 && x >= -999); }; break;
                        case 4: pred = [](int x) { return (x >= 1000 && x <= 9999) || (x <= -1000 && x >= -9999); }; break;
                        default: pred = [](int) { return true; };
                    }
                } else if constexpr (std::is_same_v<T, float>) {
                    switch (selected_where_func) {
                        case 0: pred = [](float x) { return std::abs(x - std::roundf(x)) > 1e-6f; }; break;
                        case 1: pred = [](float x) { return x <= 0.0f; }; break;
                        case 2: pred = [](float x) { return x >= 0.0f; }; break;
                        default: pred = [](float) { return true; };
                    }
                } else if constexpr (std::is_same_v<T, char>) {
                    switch (selected_where_func) {
                        case 0: pred = [](char c) { return std::isupper((unsigned char)c) != 0; }; break;
                        case 1: pred = [](char c) { return std::islower((unsigned char)c) != 0; }; break;
                        case 2: pred = [](char c) { return std::isalpha((unsigned char)c) != 0; }; break;
                        case 3: pred = [](char c) { return std::isdigit((unsigned char)c) != 0; }; break;
                        default: pred = [](char) { return true; };
                    }
                }
                auto new_deq = deq->where(pred);
                *deq = std::move(*new_deq);
                delete new_deq;
                update_display_items();
                break;
            }
            case 6: { // sort
                deq->sort();
                update_display_items();
                break;
            }
            case 7: { // concat
                if (second_deque_str.empty()) throw invalid_argument();
                auto other_deq = parse_deque_from_string<Container, T>(second_deque_str);
                auto new_deq = deq->concat(other_deq);
                *deq = std::move(*new_deq);
                delete new_deq;
                update_display_items();
                break;
            }
            case 8: { // find
                bool ok;
                T val = parse_value<T>(val_str, ok);
                if (!ok) throw invalid_argument();
                unsigned pos = deq->find(val);
                if (pos == deq->size()) last_result = "Значение не найдено";
                else last_result = "Найдено на позиции: " + std::to_string(pos);
                break;
            }
            case 9: { // find subdeque
                if (second_deque_str.empty()) throw invalid_argument();
                auto sub = parse_deque_from_string<Container, T>(second_deque_str);
                size_t pos = deq->find_subsequence(sub);
                if (pos == deq->size()) last_result = "Поддек не найден";
                else last_result = "Поддек найден на позиции: " + std::to_string(pos);
                break;
            }
            case 10: { // merge (исправлен вызов – передан функтор сравнения)
                if (second_deque_str.empty()) throw invalid_argument("Не указан второй дек для merge");
                auto other = parse_deque_from_string<Container, T>(second_deque_str);
                auto new_deq = deq->merge(&other, std::less<T>());
                *deq = std::move(*new_deq);
                delete new_deq;
                update_display_items();
                break;
            }
            default:
                last_result = "Неизвестный режим";
        }
    } catch (const index_out_of_range& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (const invalid_argument& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (const empty_container& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (const null_ptr& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (const std::exception& e) {
        last_result = "Ошибка: " + std::string(e.what());
    } catch (...) {
        last_result = "Неизвестная ошибка";
    }
}

// ============================ UI ============================
void start_() {
    auto screen = ScreenInteractive::Fullscreen();

    std::string input_str;
    std::string input_idx;
    std::string input_second_deque;

    static int prev_container = selected_container;
    static int prev_data_type = selected_data_type;

    Component container_settings = Container::Vertical({
        Dropdown(container, &selected_container),
        Dropdown(data_type, &selected_data_type),
        Dropdown(mode, &selected_mode)
    });

    Component input_value = Input(&input_str, "значение") | border;
    Component input_index = Input(&input_idx, "индекс") | border;
    Component input_second = Input(&input_second_deque, "второй дек (элементы через запятую)") | border;

    Component mode_panels = Container::Tab({
        input_value,                // append
        input_value,                // prepend
        Container::Vertical({input_index, input_value}), // insert
        Container::Vertical({       // map
            Dropdown(map_func_for_int, &selected_map_func),
            Dropdown(map_func_for_float, &selected_map_func),
            Dropdown(map_func_for_char, &selected_map_func)
        }),
        Container::Vertical({       // reduce
            Dropdown(reduce_func_for_int, &selected_reduce_func),
            Dropdown(reduce_func_for_float, &selected_reduce_func),
            Dropdown(reduce_func_for_char, &selected_reduce_func)
        }),
        Container::Vertical({       // where
            Dropdown(where_func_for_int, &selected_where_func),
            Dropdown(where_func_for_float, &selected_where_func),
            Dropdown(where_func_for_char, &selected_where_func)
        }),
        input_value,                // sort (не требует ввода)
        input_second,               // concat
        input_value,                // find
        input_second,               // find subdeque
        input_second                // merge
    }, &selected_mode);

    auto menu = Menu(&display_items, &selected_item);
    auto display_window = menu | Renderer([&](Element inner) {
        return window(text("Список"), inner) | size(HEIGHT, LESS_THAN, 20);
    });

    auto status_line = Renderer([&] {
        return text(last_result) | color(Color::YellowLight);
    });

    Component main_content = Container::Vertical({
        container_settings,
        mode_panels,
        display_window,
        status_line
    });

    auto get_current_deque = [&]() -> std::variant<
        deque<list_seq, int>*, deque<list_seq, float>*, deque<list_seq, char>*,
        deque<array_seq, int>*, deque<array_seq, float>*, deque<array_seq, char>*
    > {
        if (selected_container == 0) {
            switch (selected_data_type) {
                case 0: return &my_deque_list_int;
                case 1: return &my_deque_list_float;
                case 2: return &my_deque_list_char;
            }
        } else {
            switch (selected_data_type) {
                case 0: return &my_deque_array_int;
                case 1: return &my_deque_array_float;
                case 2: return &my_deque_array_char;
            }
        }
        return &my_deque_list_int;
    };

    auto event_handler = CatchEvent(main_content, [&](Event event) {
        if (event == Event::Return) {
            int idx = -1;
            if (!input_idx.empty()) {
                try { idx = std::stoi(input_idx); }
                catch (...) { idx = -1; }
            }
            auto deq_var = get_current_deque();
            std::visit([&](auto* deq) {
                if (!deq) return;
                using DeqType = std::decay_t<decltype(*deq)>;
                using T = typename DeqType::value_type; // предполагаем, что в deque есть value_type
                apply_modify_op(deq, input_str, idx, input_second_deque);
                input_str.clear();
                input_idx.clear();
                input_second_deque.clear();
            }, deq_var);
            return true;
        } else if (event == Event::Delete) {
            delete_selected();
            update_display_items();
            return true;
        }
        return false;
    });

    auto final_renderer = Renderer(event_handler, [&] {
        if (prev_container != selected_container || prev_data_type != selected_data_type) {
            prev_container = selected_container;
            prev_data_type = selected_data_type;
            update_display_items();
        }
        return hbox({
            vbox({container_settings->Render()}) | vcenter,
            separator(),
            vbox({mode_panels->Render()}) | vcenter,
            separator(),
            vbox({
                text("Элементы") | center,
                menu->Render() | vscroll_indicator | frame | size(HEIGHT, GREATER_THAN, 10),
                separator(),
                status_line->Render() | center
            }) | flex
        }) | border | bgcolor(LinearGradient().Angle(180)
                              .Stop(Color::DeepSkyBlue1, 0.f)
                              .Stop(Color::Cyan2, 1.f));
    });

    screen.Loop(final_renderer);
}

int main() {
    start_();
    return 0;
}