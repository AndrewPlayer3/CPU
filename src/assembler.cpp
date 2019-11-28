#include "assembler.hpp"

std::string trim(std::string s) {
    std::size_t s_begin = s.find_first_not_of(" \t");
    std::size_t s_end   = s.find_last_not_of(" \t");
    std::string s_trimmed = s.substr(s_begin, s_end - s_begin + 1);
    if(s_trimmed == "") {
        std::cout << "trimmed " << s << " to nothing" << std::endl;
    }
    return s_trimmed;
}

bool is_label(std::string str) {
    if(str[str.size() - 1] != ':') {
        return false;
    }
    return true;
}

bool is_opcode(std::string str) {
    if(str_to_op.find(str)->second == 0) {
        return false;
    }
    return true;
}

bool is_operand(std::string str) {
    if(str[str.size() - 1] != ',') {
        return false;
    }
    return true;
}

int main() {
    
    std::string m = "jmp";

    OPCODE_MNEMONIC op = str_to_op[m];

    switch(op) {
        case MOV:
            std::cout << "mov" << std::endl;
            break;
    }

    std::string m_ws = "    klep,   ";

    std::string m_ws_trimmed = trim(m_ws);

    OPCODE_MNEMONIC m_ws_op = str_to_op[m_ws_trimmed];

    switch(m_ws_op) {
        case MOV:
            std::cout << "mov" << std::endl;
            break;
    }

    Lexer l(m_ws);
    l.print_lexed();

    if(is_opcode(m_ws_trimmed)) {
        std::cout << m_ws_trimmed << " is an opcode." << std::endl;
    } else if(is_label(m_ws_trimmed)) {
        std::cout << m_ws_trimmed << " is a label." << std::endl;
    } else if (is_operand(m_ws_trimmed)) {
        std::cout << m_ws_trimmed << " is an operand." << std::endl;
    } else {
        std::cout << m_ws_trimmed << " is an invalid token." << std::endl;
    }

    std::cout << str_to_op[m_ws_trimmed] << std::endl;

    return 0;
}