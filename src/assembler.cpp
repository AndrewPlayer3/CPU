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

bool is_register(std::string str) {
    if(str_to_reg.find(str)->second == 0) {
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

bool is_instruction(std::string line) {
    bool prop = str_to_op.find(trim(line.substr(0, 3)))->second != 0;
    if(prop) {
        return true;
    }
    return false;
}

bool is_comment(std::string line) {
    if(trim(line)[0] == '#') {
        return true;
    }
    return false;
}

bool is_int(std::string str) {
    int num;
    std::istringstream is(str);
    if(is >> num) {
        return true;
    }
    return false;
}

bool is_pointer(std::string arg) {
    if(arg[0] == '[' && arg[arg.size() - 1] == ']') {
        if(is_int(arg.substr(0, arg.size() - 1))) {
            return true;
        }
    }
    return false;
}

int to_int(std::string str) {
    int number;
    std::istringstream is(str);
    is >> number;
    return number;
}

vector<std::string> parseInstruction(std::string line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    std::string op = "";
    while(line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t') {
        op += line_trimmed[pos++];
    }
    if(str_to_op.find(op)->second == 0) {
        std::cout << "Invalid Instruction: " << op << std::endl;
        exit(1);
    }
    while(line_trimmed[pos] == ' ' || line_trimmed[pos] == '\t') {
        pos++;
    }
    std::string arg0 = "";
    std::string arg1 = "";
    while(line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t' && line_trimmed[pos] != ',') {
        arg0 += line_trimmed[pos++];
    }
    while(line_trimmed[pos] == ' ' || line_trimmed[pos] == '\t' || line_trimmed[pos] == ',') {
        pos++;
    }
    while(pos != line_trimmed.size() && line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t') {
        arg1 += line_trimmed[pos++];
    }
    vector<std::string> instruction_plus_args = {op, arg0, arg1};
    return instruction_plus_args;
}

vector<pair<std::string, ARG_TYPE>> parseArgTypes(vector<std::string> arg_vector) {

    vector<pair<std::string, ARG_TYPE>> arg_type_vec;

    for(std::string arg : arg_vector) {
        if(str_to_op.find(arg)->second != 0) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, OPCODE));
        } else if(is_int(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, INTEGER));
        } else if(is_pointer(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, POINTER));
        } else if(is_register(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, REGISTER));
        } else {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, STRING));
        }
    }

    return arg_type_vec;
}

std::string mov(vector<pair<std::string, ARG_TYPE>> instr) {
    int A = 0xE;
    int B = 0x0;
    int C = 0x0;
    int D = 0x0;
    bool is_int_arg = false;
    int int_arg = 0x0;
    bool is_str_arg = false;
    std::string str_arg = "";
    if(instr.size() == 3) {
        switch(instr[2].second) {
            case INTEGER:
                B = 0x0;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            case REGISTER:
                B = 0x1;
                C = str_to_reg[instr[1].first];
                D = str_to_reg[instr[2].first];
                break;
            case STRING:
                B = 0x3;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                str_arg = instr[2].first;
                is_str_arg = true;
                break;
            case POINTER:
                B = 0xF;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            default:
                std::cout << "Something wrong with mov" << std::endl;
                exit(1);
        }
    }
    int inst = (A << 12) | (B << 8) | (C << 4) | (D << 0);
    std::ostringstream os;
    os << std::hex << inst << '\n';
    if(is_int_arg) {
        os << int_arg << '\n';
    }
    if(is_str_arg) {
        os << str_arg << '\n';
    }
    return os.str();
} 

int main() {
    
    std::string instruction_line = "mov r10, string";

    if(is_instruction(instruction_line)) {
        vector<std::string> instruction_vector = parseInstruction(instruction_line);
        auto arg_type_vector = parseArgTypes(instruction_vector);
        std::string machine_code = mov(arg_type_vector);
        std::cout << machine_code;
    } else if(is_comment(instruction_line)) {
        std::cout << "comment" << std::endl;
    } else if(is_label(instruction_line)) {
        std::cout << "label" << std::endl;
    } else {
        std::cout << "invalid line" << std::endl;
    }

    return 0;
}