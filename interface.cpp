#include <string>
#include <memory>
#include <cctype>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp> 
#include "include/list_seq.hpp"
#include "include/array_seq.hpp"
#include "include/Mutability.hpp"

list_seq<Mutability::Mutable, int> my_list_i;
array_seq<Mutability::Mutable, int> my_array_i;
list_seq<Mutability::Mutable, float> my_list_f;
array_seq<Mutability::Mutable, float> my_array_f;
list_seq<Mutability::Mutable, char> my_list_c;
array_seq<Mutability::Mutable, char> my_array_c;
list_seq<Mutability::Mutable, std::string> my_list_s;
array_seq<Mutability::Mutable, std::string> my_array_s;

std::vector<std::string> container = {"list", "array", "bit sequence"};
std::vector<std::string> data_type  = {"int", "float", "char", "string"};
std::vector<std::string> mode = {"append", "prepend", "insert", "map", "reduce", "where"};

std::vector<std::string> functions_map = {"x+1", "x*2", "x*x"};
std::vector<std::string> functions_where = {"x>0", "x%2==0", "x<5"};
std::vector<std::string> functions_reduce = {"sum", "product", "min", "max"};

std::string last_reduce_result;

template<typename T>
std::string to_string_impl(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
        return value;
    } else if constexpr (std::is_same_v<T, char>) {
        return std::string(1, value);
    } else {
        return std::to_string(value);
    }
}

template<typename T>
T parse_value(const std::string& s, bool& ok);

template<>
int parse_value<int>(const std::string& s, bool& ok) {
    try {
        size_t pos;
        int val = std::stoi(s, &pos);
        ok = (pos == s.size());
        return val;
    } catch(...) { ok = false; return 0; }
}

template<>
float parse_value<float>(const std::string& s, bool& ok) {
    try {
        size_t pos;
        float val = std::stof(s, &pos);
        ok = (pos == s.size());
        return val;
    } catch(...) { ok = false; return 0.0f; }
}

template<>
char parse_value<char>(const std::string& s, bool& ok) {
    ok = !s.empty();
    return ok ? s[0] : '\0';
}

template<>
std::string parse_value<std::string>(const std::string& s, bool& ok) {
    ok = true;
    return s;
}

class ElementArray {
    std::unique_ptr<ftxui::Element[]> data_;
    size_t size_;
public:
    explicit ElementArray(size_t size) : data_(new ftxui::Element[size]), size_(size) {}
    ftxui::Element& operator[](size_t i) { return data_[i]; }
    const ftxui::Element& operator[](size_t i) const { return data_[i]; }
    ftxui::Element* begin() { return data_.get(); }
    ftxui::Element* end()   { return data_.get() + size_; }
    const ftxui::Element* begin() const { return data_.get(); }
    const ftxui::Element* end()   const { return data_.get() + size_; }
    size_t size() const { return size_; }
};

template <typename T>
ftxui::Element draw_list(list_seq<Mutability::Mutable, T>& list) {
    using namespace ftxui;
    if (list.size() == 0) return text("пусто") | border | center;
    ElementArray elements(list.size());
    size_t idx = 0;
    for (auto& val : list) elements[idx++] = text(to_string_impl(val)) | border;
    return hbox(elements);
}

template <typename T>
ftxui::Element draw_array(array_seq<Mutability::Mutable, T>& array) {
    using namespace ftxui;
    if (array.size() == 0) return text("пусто") | border | center;
    ElementArray elements(array.size());
    size_t idx = 0;
    for (auto& val : array) elements[idx++] = text(to_string_impl(val)) | border;
    return hbox(elements);
}

template<typename Seq, typename T>
void apply_to_seq(Seq& seq, int op, const std::string& val_str, int index, int func_choice, std::string& result_msg) {
    bool ok;
    T val = parse_value<T>(val_str, ok);
    if (!ok && (op == 0 || op == 1 || op == 2)) { result_msg = "Ошибка ввода значения"; return; }

    switch (op) {
        case 0: seq.append(val); break;
        case 1: seq.prepend(val); break;
        case 2: seq.insert(val, index); 

        break;
        case 3: { // map
            auto f = [func_choice](const T& x) -> T {
                if constexpr (std::is_same_v<T, int>) {
                    if (func_choice == 0) return x + 1;
                    if (func_choice == 1) return x * 2;
                    if (func_choice == 2) return x * x;
                } else if constexpr (std::is_same_v<T, float>) {
                    if (func_choice == 0) return x + 1.0f;
                    if (func_choice == 1) return x * 2.0f;
                    if (func_choice == 2) return x * x;
                } else if constexpr (std::is_same_v<T, char>) {
                    if (func_choice == 0) return x + 1;
                    if (func_choice == 1) return x * 2;
                    if (func_choice == 2) return x * x;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    if (func_choice == 0) return x + "1";
                    if (func_choice == 1) return x + x;
                    if (func_choice == 2) return x + x;
                }
                return x;
            };
            seq.map(f);
            break;
        }
        case 4: { // reduce
            if (seq.size() == 0) { result_msg = "Пустая последовательность"; return; }
            auto f = [func_choice](const T& a, const T& b) -> T {
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
                    if (func_choice == 0) return a + b;
                    if (func_choice == 1) return a * b;
                    if (func_choice == 2) return (a < b) ? a : b;
                    if (func_choice == 3) return (a > b) ? a : b;
                } else if constexpr (std::is_same_v<T, char>) {
                    if (func_choice == 0) return a + b;
                    if (func_choice == 1) return a * b;
                    if (func_choice == 2) return (a < b) ? a : b;
                    if (func_choice == 3) return (a > b) ? a : b;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    if (func_choice == 0) return a + b;
                    if (func_choice == 2) return (a < b) ? a : b;
                    if (func_choice == 3) return (a > b) ? a : b;
                }
                return a;
            };
            T res = seq.reduce(f, seq[0]);
            result_msg = "Результат: " + to_string_impl(res);
            return;
        }
        case 5: {
            auto pred = [func_choice](const T& x) -> bool {
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
                    if (func_choice == 0) return x > 0;
                    if (func_choice == 1) return static_cast<int>(x) % 2 == 0;
                    if (func_choice == 2) return x < 5;
                } else if constexpr (std::is_same_v<T, char>) {
                    if (func_choice == 0) return x > 0;
                    if (func_choice == 1) return static_cast<int>(x) % 2 == 0;
                    if (func_choice == 2) return x < 5;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    if (func_choice == 0) return !x.empty();
                    if (func_choice == 1) return x.find('a') != std::string::npos;
                    if (func_choice == 2) return x.size() < 5;
                }
                return false;
            };
            seq.where(pred);
            break;
        }
    }
    result_msg = "Готово";
}

void apply_operation(int cont, int dtype, int op,
                     const std::string& val_str, int index, int func_choice) {
    if (dtype == 0) { // int
        if (cont == 0)
            apply_to_seq<list_seq<Mutability::Mutable, int>, int>(my_list_i, op, val_str, index, func_choice, last_reduce_result);
        else
            apply_to_seq<array_seq<Mutability::Mutable, int>, int>(my_array_i, op, val_str, index, func_choice, last_reduce_result);
    } else if (dtype == 1) { // float
        if (cont == 0)
            apply_to_seq<list_seq<Mutability::Mutable, float>, float>(my_list_f, op, val_str, index, func_choice, last_reduce_result);
        else
            apply_to_seq<array_seq<Mutability::Mutable, float>, float>(my_array_f, op, val_str, index, func_choice, last_reduce_result);
    } else if (dtype == 2) { // char
        if (cont == 0)
            apply_to_seq<list_seq<Mutability::Mutable, char>, char>(my_list_c, op, val_str, index, func_choice, last_reduce_result);
        else
            apply_to_seq<array_seq<Mutability::Mutable, char>, char>(my_array_c, op, val_str, index, func_choice, last_reduce_result);
    } else if (dtype == 3) { // string
        if (cont == 0)
            apply_to_seq<list_seq<Mutability::Mutable, std::string>, std::string>(my_list_s, op, val_str, index, func_choice, last_reduce_result);
        else
            apply_to_seq<array_seq<Mutability::Mutable, std::string>, std::string>(my_array_s, op, val_str, index, func_choice, last_reduce_result);
    }
}

void start_() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    int selected_container = 0, selected_data_type = 0, selected_mode = 0, func_choice = 0;
    std::string input_value;
    std::string input_value2;
    std::string input_index;

    auto input_val_append = Input(&input_value, "значение")|border;
    auto input_val_prepend = Input(&input_value, "значение")|border;
    auto insert_input = Container::Vertical({Input(&input_value2, "значение")|border, Input(&input_index, "индекс")|border,});
    auto input_val2 = Input(&input_value2, "значение")|border;
    auto input_idx = Input(&input_index, "индекс")|border;

    auto func_map   = Dropdown(&functions_map,   &func_choice);
    auto func_reduce= Dropdown(&functions_reduce,&func_choice);
    auto func_where = Dropdown(&functions_where, &func_choice);
    auto execute = Button("Выполнить", [&] {
        int idx = 0;
        bool idx_ok = true;
        if (selected_mode == 2) { // insert
            try { idx = std::stoi(input_index); } catch(...) { idx_ok = false; }
            if (!idx_ok) { last_reduce_result = "Индекс должен быть числом"; return; }
        }
        try{apply_operation(selected_container, selected_data_type, selected_mode,
                        (selected_mode == 2) ? input_value2 : input_value,
                        idx, func_choice);}
        catch(const exception_& e){
            last_reduce_result = std::string("Ошибка: ") + e.what();
        } 
        catch (const std::exception& e) {
            last_reduce_result = std::string("Ошибка: ") + e.what();
        }
        catch (...) {
            last_reduce_result = "Неизвестная ошибка";
        }
        if (selected_mode != 3 && selected_mode != 4 && selected_mode != 5) {
            input_value.clear();
            input_value2.clear();
            input_index.clear();
        }
    });

    auto input_panel = Container::Tab({
        input_val_append,
        input_val_prepend,
        Container::Vertical({input_val2, input_idx}),
        func_map,
        func_reduce,
        func_where,
    }, &selected_mode);

    auto menu_ = Container::Vertical({
        Dropdown(&container, &selected_container),
        Dropdown(&data_type, &selected_data_type),
        Dropdown(&mode, &selected_mode)
    });

    auto display = Renderer([&] {
        if (selected_container == 0) { // list
            switch (selected_data_type) {
                case 0: return draw_list(my_list_i);
                case 1: return draw_list(my_list_f);
                case 2: return draw_list(my_list_c);
                case 3: return draw_list(my_list_s);
                default: return text("unknown type") | center;
            }
        } else if (selected_container == 1) { // array
            switch (selected_data_type) {
                case 0: return draw_array(my_array_i);
                case 1: return draw_array(my_array_f);
                case 2: return draw_array(my_array_c);
                case 3: return draw_array(my_array_s);
                default: return text("unknown type") | center;
            }
        } else {
            return text("bit sequence засекречен о^о") | center;
        }
    });

    auto reduce_result = Renderer([&] {
        if (selected_mode == 4 && !last_reduce_result.empty())
            return hbox({text(last_reduce_result) | center | border});
        else
            return hbox({text("")});
    });

    
    auto main_container = Container::Vertical({
        menu_,
        input_panel,
        execute,
        display,
        reduce_result
    });

    auto all = Renderer(main_container, [&] {
        return hbox({
            menu_->Render() | vcenter, separator(),
            vbox({input_panel->Render(), 
            execute->Render()})| vcenter,
            separator(),
            display->Render() | center,
            reduce_result->Render() | hcenter
        }) | border | bgcolor(Color::Blue) | flex;
    });

    screen.Loop(all);
}
