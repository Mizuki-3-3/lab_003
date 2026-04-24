#include <string>
#include <memory>
#include <cctype>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>  // используем современный API
#include "list_seq.hpp"
#include "array_seq.hpp"
#include "Mutability.hpp"

template<typename T>
std::string to_string_impl(const T& value);

template<typename T>
T parse_value(const std::string& s, bool& ok);

template<>
int parse_value<int>(const std::string& s, bool& ok);
template<>
float parse_value<float>(const std::string& s, bool& ok);
template<>
char parse_value<char>(const std::string& s, bool& ok);
template<>
std::string parse_value<std::string>(const std::string& s, bool& ok);

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
ftxui::Element draw_list(list_seq<Mutability::Mutable, T>& list);

template <typename T>
ftxui::Element draw_array(array_seq<Mutability::Mutable, T>& array);
template<typename Seq, typename T>
void apply_to_seq(Seq& seq, int op, const std::string& val_str, int index, int func_choice, std::string& result_msg);
void apply_operation(int cont, int dtype, int op,
                     const std::string& val_str, int index, int func_choice);
void start_();