#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

#include <vector>
#include <string>
#include <variant>
#include <memory>
#include <functional>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

#include "deque.hpp"
#include "array_seq.hpp"
#include "list_seq.hpp"
#include "errors.hpp"

// ============================================================
// Типовые алиасы для трёх видов контейнеров-обёрток
// ============================================================
using DequeArrInt   = deque<array_seq, int,   array_seq, 8>;
using DequeArrFloat = deque<array_seq, float, array_seq, 8>;
using DequeArrChar  = deque<array_seq, char,  array_seq, 8>;

using DequeLstInt   = deque<list_seq, int,   list_seq, 8>;
using DequeLstFloat = deque<list_seq, float, list_seq, 8>;
using DequeLstChar  = deque<list_seq, char,  list_seq, 8>;

using DequeVecInt   = deque<std::vector, int,   std::vector, 8>;
using DequeVecFloat = deque<std::vector, float, std::vector, 8>;
using DequeVecChar  = deque<std::vector, char,  std::vector, 8>;

using AnyDeque = std::variant<
    DequeArrInt,   DequeArrFloat,   DequeArrChar,
    DequeLstInt,   DequeLstFloat,   DequeLstChar,
    DequeVecInt,   DequeVecFloat,   DequeVecChar
>;

enum class DataType { INT, FLOAT, CHAR };
enum class ContainerType { ARRAY_SEQ, LIST_SEQ, VECTOR };

// ============================================================
// Вспомогательные функции для работы с AnyDeque
// ============================================================
struct DequeInfo {
    std::string name;
    DataType dtype;
    ContainerType ctype;
    AnyDeque deque;
};

std::string anyDequeToString(const AnyDeque& d) {
    return std::visit([](const auto& dq) -> std::string {
        using T = typename std::decay_t<decltype(dq)>::value_type;
        std::ostringstream oss;
        bool first = true;
        for (auto it = dq.begin(); it != dq.end(); ++it) {
            if (!first) oss << " ";
            first = false;
            if constexpr (std::is_same_v<T, char>) {
                oss << "'" << *it << "'";
            } else {
                oss << *it;
            }
        }
        return oss.str();
    }, d);
}

size_t anyDequeSize(const AnyDeque& d) {
    return std::visit([](const auto& dq) { return dq.size(); }, d);
}

std::string anyDequeElementAt(const AnyDeque& d, size_t idx) {
    return std::visit([idx](const auto& dq) -> std::string {
        using T = typename std::decay_t<decltype(dq)>::value_type;
        std::ostringstream oss;
        const T& val = dq[idx];
        if constexpr (std::is_same_v<T, char>) {
            oss << "'" << val << "'";
        } else {
            oss << val;
        }
        return oss.str();
    }, d);
}

// ============================================================
// Парсер выражений для map/where
// ============================================================
template<typename T>
T evalMapExpr(const std::string& expr, T x) {
    std::string e = expr;
    e.erase(std::remove_if(e.begin(), e.end(), ::isspace), e.end());

    size_t xpos = e.find('x');
    if (xpos == std::string::npos) {
        std::istringstream iss(e);
        T val; iss >> val;
        return val;
    }

    std::string left = e.substr(0, xpos);
    std::string rest = e.substr(xpos + 1);

    T leftVal = x;
    if (!left.empty()) {
        std::istringstream iss(left);
        iss >> leftVal;
    }

    if (rest.empty()) return leftVal;

    char op = rest[0];
    std::string numStr = rest.substr(1);
    T rightVal{};
    if (!numStr.empty()) {
        std::istringstream iss(numStr);
        iss >> rightVal;
    }

    switch (op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return rightVal == T(0) ? T(0) : leftVal / rightVal;
        default:  return leftVal;
    }
}

template<typename T>
bool evalWhereExpr(const std::string& expr, T x) {
    std::string e = expr;
    e.erase(std::remove_if(e.begin(), e.end(), ::isspace), e.end());

    auto tryOp = [&](const std::string& opStr) -> int {
        size_t pos = e.find(opStr);
        if (pos == std::string::npos) return -1;
        return static_cast<int>(pos);
    };

    struct OpInfo { std::string sym; int code; };
    std::vector<OpInfo> ops = {
        {"<=", 1}, {">=", 2}, {"!=", 3}, {"==", 4}, {"<", 5}, {">", 6}
    };

    for (const auto& op : ops) {
        int pos = tryOp(op.sym);
        if (pos != -1) {
            std::string left = e.substr(0, pos);
            std::string right = e.substr(pos + op.sym.size());

            auto parseVal = [&](const std::string& s) -> T {
                if (s == "x") return x;
                std::istringstream iss(s);
                T v; iss >> v;
                return v;
            };

            T l = parseVal(left);
            T r = parseVal(right);

            switch (op.code) {
                case 1: return l <= r;
                case 2: return l >= r;
                case 3: return l != r;
                case 4: return l == r;
                case 5: return l <  r;
                case 6: return l >  r;
            }
        }
    }
    return false;
}

// ============================================================
// Применение операций к AnyDeque
// ============================================================
struct OpResult {
    bool ok = false;
    std::string message;
    std::string answer;
    AnyDeque resultDeque;
    bool hasResultDeque = false;
};

OpResult applyOperation(AnyDeque& d, const std::string& mode,
                        const std::string& valueStr,
                        const std::string& indexStr,
                        DataType dtype) {
    OpResult res;
    try {
        if (mode == "push_back") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                std::istringstream iss(valueStr);
                T v;
                if constexpr (std::is_same_v<T, char>) {
                    if (valueStr.size() >= 1) v = valueStr[0];
                    else throw invalid_argument("empty char");
                } else {
                    iss >> v;
                }
                dq.push_back(v);
            }, d);
            res.ok = true;
            res.message = "push_back OK";
        }
        else if (mode == "push_front") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                std::istringstream iss(valueStr);
                T v;
                if constexpr (std::is_same_v<T, char>) {
                    if (valueStr.size() >= 1) v = valueStr[0];
                    else throw invalid_argument("empty char");
                } else {
                    iss >> v;
                }
                dq.push_front(v);
            }, d);
            res.ok = true;
            res.message = "push_front OK";
        }
        else if (mode == "insert") {
            size_t idx = std::stoul(indexStr);
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                std::istringstream iss(valueStr);
                T v;
                if constexpr (std::is_same_v<T, char>) {
                    if (valueStr.size() >= 1) v = valueStr[0];
                    else throw invalid_argument("empty char");
                } else {
                    iss >> v;
                }
                auto it = dq.begin();
                for (size_t i = 0; i < idx && it != dq.end(); ++i) ++it;
                dq.insert(it, v);
            }, d);
            res.ok = true;
            res.message = "insert OK at " + std::to_string(idx);
        }
        else if (mode == "find") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                std::istringstream iss(valueStr);
                T v;
                if constexpr (std::is_same_v<T, char>) {
                    if (valueStr.size() >= 1) v = valueStr[0];
                    else throw invalid_argument("empty char");
                } else {
                    iss >> v;
                }
                auto it = dq.find(v);
                size_t idx = 0;
                for (auto b = dq.begin(); b != it; ++b) ++idx;
                res.answer = "found at index " + std::to_string(idx);
            }, d);
            res.ok = true;
        }
        else if (mode == "map") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                auto* newDq = dq.map([&](const T& x) {
                    return evalMapExpr<T>(valueStr, x);
                });
                res.resultDeque = std::move(*newDq);
                res.hasResultDeque = true;
                delete newDq;
            }, d);
            res.ok = true;
            res.message = "map OK";
        }
        else if (mode == "where") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                auto* newDq = dq.where([&](const T& x) {
                    return evalWhereExpr<T>(valueStr, x);
                });
                res.resultDeque = std::move(*newDq);
                res.hasResultDeque = true;
                delete newDq;
            }, d);
            res.ok = true;
            res.message = "where OK";
        }
        else if (mode == "reduce") {
            std::visit([&](auto& dq) {
                using T = typename std::decay_t<decltype(dq)>::value_type;
                std::istringstream iss(valueStr);
                T init;
                if constexpr (std::is_same_v<T, char>) {
                    if (valueStr.size() >= 1) init = valueStr[0];
                    else throw invalid_argument("empty char");
                } else {
                    iss >> init;
                }
                T acc = dq.reduce(init, [](T a, T b) { return a + b; });
                std::ostringstream oss;
                if constexpr (std::is_same_v<T, char>) {
                    oss << "'" << acc << "'";
                } else {
                    oss << acc;
                }
                res.answer = oss.str();
            }, d);
            res.ok = true;
        }
    }
    catch (const std::exception& e) {
        res.ok = false;
        res.message = std::string("ERROR: ") + e.what();
    }
    return res;
}

// ============================================================
// Создание деков
// ============================================================
AnyDeque makeEmptyDeque(DataType dt, ContainerType ct) {
    if (ct == ContainerType::ARRAY_SEQ) {
        switch (dt) {
            case DataType::INT:   return DequeArrInt();
            case DataType::FLOAT: return DequeArrFloat();
            case DataType::CHAR:  return DequeArrChar();
        }
    } else if (ct == ContainerType::LIST_SEQ) {
        switch (dt) {
            case DataType::INT:   return DequeLstInt();
            case DataType::FLOAT: return DequeLstFloat();
            case DataType::CHAR:  return DequeLstChar();
        }
    } else {
        switch (dt) {
            case DataType::INT:   return DequeVecInt();
            case DataType::FLOAT: return DequeVecFloat();
            case DataType::CHAR:  return DequeVecChar();
        }
    }
    return DequeVecInt();
}

AnyDeque copyDeque(const AnyDeque& src) {
    return std::visit([](const auto& dq) -> AnyDeque {
        return std::decay_t<decltype(dq)>(dq);
    }, src);
}

AnyDeque concatDeques(const AnyDeque& a, const AnyDeque& b) {
    return std::visit([&](const auto& da) -> AnyDeque {
        using DA = std::decay_t<decltype(da)>;
        const auto& db = std::get<DA>(b);
        auto* res = da.concat(db);
        AnyDeque r = std::move(*res);
        delete res;
        return r;
    }, a);
}

AnyDeque subDeque(const AnyDeque& a, size_t start, size_t end) {
    return std::visit([start, end](const auto& da) -> AnyDeque {
        auto* res = da.subdeque(start, end);
        AnyDeque r = std::move(*res);
        delete res;
        return r;
    }, a);
}

AnyDeque mergeDeques(const AnyDeque& a, const AnyDeque& b, DataType dt) {
    return std::visit([&](const auto& da) -> AnyDeque {
        using DA = std::decay_t<decltype(da)>;
        using T = typename DA::value_type;
        const auto& db = std::get<DA>(b);
        auto* res = da.merge(&db, std::less<T>());
        AnyDeque r = std::move(*res);
        delete res;
        return r;
    }, a);
}

// ============================================================
// Главное приложение
// ============================================================
using namespace ftxui;

class DequeDemoApp {
public:
    DequeDemoApp() {
        // Режимы работы — теперь просто std::vector<std::string>
        modes = {"push_back", "push_front", "insert", "find",
                 "map", "where", "reduce"};

        // Типы данных и контейнеров — тоже просто строки
        dtLabels = {"int", "float", "char"};
        ctLabels = {"array_seq", "list_seq", "vector"};
        createModeLabels = {
            "make empty", "copy", "concat",
            "subdeque", "merge"
        };

        // Левая панель
        dequeListMenu = Menu(&dequeNames, &selectedDequeIdx);

        makeNewBtn = Button("make new deque", [this] {
            showCreateMenu = true;
            tabIndex = 1;
            createModeIdx = 0;
            createNameInput = "";
            createDtIdx = 0;
            createCtIdx = 0;
            createSrc1Idx = 0;
            createSrc2Idx = 0;
            createStartStr = "0";
            createEndStr = "0";
            createError = "";
        }, ButtonOption::Border());

        leftPanel = Container::Vertical({
            dequeListMenu,
            makeNewBtn,
        });

        leftContainer = Renderer(leftPanel, [this] {
            auto title = text(" existed deques ") | bold | center;
            auto sep = separator();
            auto list = dequeListMenu->Render() | vscroll_indicator | yframe | flex;
            auto btn = makeNewBtn->Render();
            return vbox({
                title | border,
                sep,
                list,
                sep,
                btn,
            }) | size(WIDTH, EQUAL, 30);
        });

        // Правая панель
        modeDropdown = Dropdown(&modes, &selectedModeIdx);

        inputValue = Input(&inputValueStr, "value");
        inputIndex = Input(&inputIndexStr, "index");
        inputExpr  = Input(&inputExprStr, "expression (e.g. x+5, x>3)");
        inputInit  = Input(&inputInitStr, "initial value");

        executeBtn = Button("execute", [this] {
            if (deques.empty()) {
                lastError = "no deque selected";
                return;
            }
            auto& info = deques[selectedDequeIdx];
            std::string mode = modes[selectedModeIdx];

            std::string val = inputValueStr;
            std::string idx = inputIndexStr;

            if (mode == "reduce") val = inputInitStr;
            if (mode == "map" || mode == "where") val = inputExprStr;

            auto res = applyOperation(info.deque, mode, val, idx, info.dtype);
            if (res.ok) {
                lastError = res.message.empty() ? "OK" : res.message;
                lastAnswer = res.answer;
                if (res.hasResultDeque) {
                    DequeInfo ni;
                    ni.name = info.name + "_" + mode + "_result";
                    ni.dtype = info.dtype;
                    ni.ctype = info.ctype;
                    ni.deque = std::move(res.resultDeque);
                    deques.push_back(std::move(ni));
                    refreshDequeNames();
                }
            } else {
                lastError = res.message;
                lastAnswer = "";
            }
        }, ButtonOption::Border());

        rightContainer = Container::Vertical({
            modeDropdown,
            inputValue,
            inputIndex,
            inputExpr,
            inputInit,
            executeBtn,
        });

        rightRenderer = Renderer(rightContainer, [this] {
            Elements elements;

            elements.push_back(
                hbox({
                    text(" mode: ") | bold,
                    modeDropdown->Render(),
                }) | border
            );

            if (deques.empty()) {
                elements.push_back(
                    text(" (no deque selected) ") | center | dim
                );
                return vbox(std::move(elements));
            }

            auto& info = deques[selectedDequeIdx];

            elements.push_back(
                text(" deque: " + info.name) | bold | center
            );

            std::string mode = modes[selectedModeIdx];

            if (mode == "push_back" || mode == "push_front") {
                elements.push_back(
                    hbox({
                        text(" value: ") | size(WIDTH, EQUAL, 10),
                        inputValue->Render() | flex,
                    })
                );
            }
            else if (mode == "insert") {
                elements.push_back(
                    hbox({
                        text(" value: ") | size(WIDTH, EQUAL, 10),
                        inputValue->Render() | flex,
                        text(" index: ") | size(WIDTH, EQUAL, 10),
                        inputIndex->Render() | flex,
                    })
                );
            }
            else if (mode == "find") {
                elements.push_back(
                    hbox({
                        text(" value: ") | size(WIDTH, EQUAL, 10),
                        inputValue->Render() | flex,
                    })
                );
            }
            else if (mode == "map" || mode == "where") {
                elements.push_back(
                    hbox({
                        text(" expr: ") | size(WIDTH, EQUAL, 10),
                        inputExpr->Render() | flex,
                    })
                );
            }
            else if (mode == "reduce") {
                elements.push_back(
                    hbox({
                        text(" init: ") | size(WIDTH, EQUAL, 10),
                        inputInit->Render() | flex,
                        text(" | answer: ") | size(WIDTH, EQUAL, 12),
                        text(" " + lastAnswer + " ") | flex | border,
                    })
                );
            }

            elements.push_back(executeBtn->Render() | center);

            if (!lastError.empty()) {
                bool isError = lastError.find("ERROR") != std::string::npos
                            || lastError.find("no deque") != std::string::npos;
                auto style = isError ? color(Color::Red) : color(Color::Green);
                elements.push_back(
                    text(" " + lastError + " ") | style | center
                );
            }

            elements.push_back(separator());

            Elements items;
            size_t sz = anyDequeSize(info.deque);
            for (size_t i = 0; i < sz; ++i) {
                std::string elem = anyDequeElementAt(info.deque, i);
                items.push_back(
                    text(" " + std::to_string(i + 1) + ". " + elem)
                );
            }
            if (items.empty()) {
                items.push_back(text(" (empty) ") | dim);
            }

            elements.push_back(
                vbox(std::move(items)) | vscroll_indicator | yframe | flex
            );

            return vbox(std::move(elements)) | border;
        });

        // Меню создания дека
        createModeMenu = Menu(&createModeLabels, &createModeIdx);
        createNameInputComp = Input(&createNameInput, "deque name");
        createStartInput = Input(&createStartStr, "start");
        createEndInput   = Input(&createEndStr, "end");
        createDtMenu = Menu(&dtLabels, &createDtIdx);
        createCtMenu = Menu(&ctLabels, &createCtIdx);
        createSrc1Menu = Menu(&dequeNames, &createSrc1Idx);
        createSrc2Menu = Menu(&dequeNames, &createSrc2Idx);

        createConfirmBtn = Button("confirm", [this] {
            try {
                if (createNameInput.empty()) {
                    createError = "name is empty";
                    return;
                }
                for (const auto& d : deques) {
                    if (d.name == createNameInput) {
                        createError = "name already exists";
                        return;
                    }
                }

                DequeInfo ni;
                ni.name = createNameInput;
                std::string mode = createModeLabels[createModeIdx];

                if (mode == "make empty") {
                    DataType dt = static_cast<DataType>(createDtIdx);
                    ContainerType ct = static_cast<ContainerType>(createCtIdx);
                    ni.dtype = dt;
                    ni.ctype = ct;
                    ni.deque = makeEmptyDeque(dt, ct);
                }
                else if (mode == "copy") {
                    if (deques.empty()) { createError = "no deques to copy"; return; }
                    const auto& src = deques[createSrc1Idx];
                    ni.dtype = src.dtype;
                    ni.ctype = src.ctype;
                    ni.deque = copyDeque(src.deque);
                }
                else if (mode == "concat") {
                    if (deques.size() < 2) { createError = "need >= 2 deques"; return; }
                    const auto& a = deques[createSrc1Idx];
                    const auto& b = deques[createSrc2Idx];
                    if (a.dtype != b.dtype || a.ctype != b.ctype) {
                        createError = "types must match";
                        return;
                    }
                    ni.dtype = a.dtype;
                    ni.ctype = a.ctype;
                    ni.deque = concatDeques(a.deque, b.deque);
                }
                else if (mode == "subdeque") {
                    if (deques.empty()) { createError = "no deques"; return; }
                    const auto& src = deques[createSrc1Idx];
                    size_t start = std::stoul(createStartStr);
                    size_t end = std::stoul(createEndStr);
                    ni.dtype = src.dtype;
                    ni.ctype = src.ctype;
                    ni.deque = subDeque(src.deque, start, end);
                }
                else if (mode == "merge") {
                    if (deques.size() < 2) { createError = "need >= 2 deques"; return; }
                    const auto& a = deques[createSrc1Idx];
                    const auto& b = deques[createSrc2Idx];
                    if (a.dtype != b.dtype || a.ctype != b.ctype) {
                        createError = "types must match";
                        return;
                    }
                    ni.dtype = a.dtype;
                    ni.ctype = a.ctype;
                    ni.deque = mergeDeques(a.deque, b.deque, a.dtype);
                }

                deques.push_back(std::move(ni));
                refreshDequeNames();
                showCreateMenu = false;
                tabIndex = 0;
                createError = "";
            }
            catch (const std::exception& e) {
                createError = std::string("ERROR: ") + e.what();
            }
        }, ButtonOption::Border());

        createCancelBtn = Button("cancel", [this] {
            showCreateMenu = false;
            tabIndex = 0;
            createError = "";
        }, ButtonOption::Border());

        createContainer = Container::Vertical({
            createModeMenu,
            createNameInputComp,
            createDtMenu,
            createCtMenu,
            createSrc1Menu,
            createSrc2Menu,
            createStartInput,
            createEndInput,
            createConfirmBtn,
            createCancelBtn,
        });

        createRenderer = Renderer(createContainer, [this] {
            Elements elements;
            elements.push_back(text(" create new deque ") | bold | center | border);

            elements.push_back(
                hbox({text(" method: "), createModeMenu->Render()})
            );
            elements.push_back(
                hbox({text(" name:   "), createNameInputComp->Render() | flex})
            );

            std::string mode = createModeLabels[createModeIdx];

            if (mode == "make empty") {
                elements.push_back(text(" data type: ") | bold);
                elements.push_back(createDtMenu->Render());
                elements.push_back(text(" container: ") | bold);
                elements.push_back(createCtMenu->Render());
            }
            else if (mode == "copy") {
                elements.push_back(text(" source deque: ") | bold);
                if (!dequeNames.empty())
                    elements.push_back(createSrc1Menu->Render());
                else
                    elements.push_back(text(" (none) ") | dim);
            }
            else if (mode == "concat" || mode == "merge") {
                elements.push_back(text(" first deque: ") | bold);
                if (!dequeNames.empty())
                    elements.push_back(createSrc1Menu->Render());
                elements.push_back(text(" second deque: ") | bold);
                if (!dequeNames.empty())
                    elements.push_back(createSrc2Menu->Render());
                if (mode == "merge") {
                    elements.push_back(
                        text(" (uses default std::less comparator) ") | dim
                    );
                }
            }
            else if (mode == "subdeque") {
                elements.push_back(text(" source deque: ") | bold);
                if (!dequeNames.empty())
                    elements.push_back(createSrc1Menu->Render());
                elements.push_back(
                    hbox({
                        text(" start: "), createStartInput->Render() | flex,
                        text(" end:   "), createEndInput->Render() | flex,
                    })
                );
            }

            if (!createError.empty()) {
                elements.push_back(text(" " + createError + " ") | color(Color::Red) | center);
            }

            elements.push_back(
                hbox({
                    createConfirmBtn->Render(),
                    text(" "),
                    createCancelBtn->Render(),
                }) | center
            );

            return vbox(std::move(elements)) | border | size(WIDTH, EQUAL, 50);
        });

        // Resizable split
        auto resizable = ResizableSplitLeft(leftContainer, rightContainer, &leftWidth);

        mainRenderer = Renderer(resizable, [this] {
            return hbox({
                leftContainer->Render(),
                separatorDouble(),
                (showCreateMenu? createRenderer->Render(): rightRenderer->Render()) | flex,
            }) | border;
        });

        root = Container::Tab({mainRenderer, createRenderer}, &tabIndex);
    }

    Component getRoot() { return root; }

private:
    void refreshDequeNames() {
        dequeNames.clear();
        for (const auto& d : deques) {
            dequeNames.push_back(d.name);
        }
        if (selectedDequeIdx >= static_cast<int>(dequeNames.size()) && !dequeNames.empty())
            selectedDequeIdx = 0;
    }

    // Состояние
    std::vector<DequeInfo> deques;
    std::vector<std::string> dequeNames;
    int selectedDequeIdx = 0;

    std::vector<std::string> modes;
    int selectedModeIdx = 0;

    std::vector<std::string> dtLabels;
    std::vector<std::string> ctLabels;
    std::vector<std::string> createModeLabels;

    std::string inputValueStr;
    std::string inputIndexStr;
    std::string inputExprStr;
    std::string inputInitStr;

    std::string lastError;
    std::string lastAnswer;

    int leftWidth = 30;
    bool showCreateMenu = false;
    int tabIndex = 0;

    int createModeIdx = 0;
    std::string createNameInput;
    int createDtIdx = 0;
    int createCtIdx = 0;
    int createSrc1Idx = 0;
    int createSrc2Idx = 0;
    std::string createStartStr;
    std::string createEndStr;
    std::string createError;

    Component dequeListMenu;
    Component makeNewBtn;
    Component leftPanel;
    Component leftContainer;

    Component modeDropdown;
    Component inputValue;
    Component inputIndex;
    Component inputExpr;
    Component inputInit;
    Component executeBtn;
    Component rightContainer;
    Component rightRenderer;

    Component createModeMenu;
    Component createNameInputComp;
    Component createDtMenu;
    Component createCtMenu;
    Component createSrc1Menu;
    Component createSrc2Menu;
    Component createStartInput;
    Component createEndInput;
    Component createConfirmBtn;
    Component createCancelBtn;
    Component createContainer;
    Component createRenderer;

    Component mainRenderer;
    Component root;
};
