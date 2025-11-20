#include "formatting.hpp"

std::string format(const std::string& fmt) {
    std::string out;
    for (size_t i = 0; i < fmt.size(); ++i) {
        if (fmt[i] == '{') {
            size_t j = i + 1;
            if (j >= fmt.size()) 
                throw FormatError("Не закрыта открывающая скобка '{'");
            if (!isdigit(fmt[j])) 
                throw FormatError("Ожидается цифра после '{'");
            while (j < fmt.size() && isdigit(fmt[j])) ++j;
            if (j >= fmt.size() || fmt[j] != '}') 
                throw FormatError("Ожидалась закрывающая скобка '}'");
            throw FormatError("Неверный индекс");
        } else if (fmt[i] == '}') 
            throw FormatError("Скобка '}' ничего не закрывает");
        else 
            out += fmt[i];  
    }
    return out;
}

std::string format(const std::string& fmt, const std::vector<std::string>& values) {
    std::string out;
    for (size_t i = 0; i < fmt.size(); ++i) {
        if (fmt[i] == '{') {
            size_t j = i + 1;
            if (j >= fmt.size()) 
                throw FormatError("Не закрыта открывающая скобка '{'");
            if (!isdigit(fmt[j])) 
                throw FormatError("Ожидается цифра после '{'");
            size_t num_start = j;
            while (j < fmt.size() && isdigit(fmt[j])) ++j;
            if (j >= fmt.size() || fmt[j] != '}') 
                throw FormatError("Ожидалась закрывающая скобка '}'");
            size_t index = std::stoul(fmt.substr(num_start, j - num_start));
            if (index >= values.size()) 
                throw FormatError("Неверный индекс");
            out += values[index];
            i = j;
        } else if (fmt[i] == '}') 
            throw FormatError("Скобка '}' ничего не закрывает");
        else
            out += fmt[i];
    }
    return out;
}
