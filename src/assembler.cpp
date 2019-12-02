/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
DOESN'T WORK WITH 32-BIT MINGW/32-BIT GCC
*/

#include "assembler.hpp"

std::map<std::string, int> LABEL_MAP;
int CURRENT_LABEL_VALUE = 0x0001;

/* Remove ws from beginning and end of line*/
std::string trim(std::string s) {
    std::size_t s_begin = s.find_first_not_of(' ');
    if(s_begin < s.size()) {
        std::size_t s_end   = s.find_last_not_of(' ');
        std::string s_trimmed = s.substr(s_begin, s_end - s_begin + 1);
        if(s_trimmed == "") {
            std::cout << "trimmed " << s << " to nothing" << std::endl;
        }
        return s_trimmed;
    }
    return "";
}

/* check if line is a label */
bool is_label(std::string line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    while(++pos != line_trimmed.size() && line_trimmed[pos] != ':');
    if(line_trimmed[pos] != ':') {
        return false;
    }
    return true;
}

/* gets just the label name before the ':' */
std::string trim_label(std::string label) {
    int pos = 0;
    std::string label_no_ws = trim(label);
    std::string label_trimmed = "";
    while(pos != label_no_ws.size() && label_no_ws[pos] != ':') {
        label_trimmed += label_no_ws[pos];
        pos++;
    }
    return label_trimmed;
}

/* checks if str is a valid opcode mnemonic */
bool is_opcode(std::string str) {
    for(std::string s : opcode_vector) {
        if(str == s) return true;
    }
    return false;
}

/* check if line is a comment */
bool is_comment(std::string line) {
    if(trim(line)[0] != '#') {
        return false;
    } 
    return true;
} 

/* check if str is a register */
bool is_register(std::string str) {
    if(str_to_reg.count(str) == 0) {
        return false;
    }
    return true;
}

/* check if str is an instruction */
bool is_instruction(std::string line) {
    bool prop = is_opcode(trim(line.substr(0, 3)));
    if(prop) {
        return true;
    }
    return false;
}

/* check if str is an integer */
bool is_int(std::string str) {
    int num;
    std::istringstream is(str);
    if(is >> num) {
        return true;
    }
    return false;
}

/* check if arg is a pointer */
bool is_pointer(std::string str) {
    if(str[0] == '[' && str[str.size() - 1] == ']') {
        if(is_int(str.substr(0, str.size() - 1))) {
            return true;
        }
    }
    return false;
}

/* convert str to an integer if possible */
int to_int(std::string str) {
    if(is_int(str)) {
        int number;
        std::istringstream is(str);
        is >> number;
        return number;
    }
    std::cout << "ERROR: Tried to convert " << str << "to an int." << std::endl;
    exit(1);
}

/* makes a vector with the opcode and its operands */
vector<std::string> parse_instruction(std::string line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    std::string op = "";
    while(line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t') {
        op += line_trimmed[pos++];
    }
    if(!is_opcode(op)) {
        std::cout << "Invalid Instruction: " << op << std::endl;
        exit(1);
    }
    while(line_trimmed[pos] == ' ' || line_trimmed[pos] == '\t') {
        pos++;
    }
    std::string arg0 = "";
    std::string arg1 = "";
    while(line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t' && line_trimmed[pos] != ',' && pos != line_trimmed.size()) {
        arg0 += line_trimmed[pos++];
    }
    while(line_trimmed[pos] == ' ' || line_trimmed[pos] == '\t' || line_trimmed[pos] == ',' && pos != line_trimmed.size()) {
        pos++;
    }
    while(pos != line_trimmed.size() && line_trimmed[pos] != ' ' && line_trimmed[pos] != '\t' && pos != line_trimmed.size()) {
        if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
            arg1 += line_trimmed[++pos];
            pos++;
        }
        if(line_trimmed[pos] != '\'') {
            arg1 += line_trimmed[pos++];
        } else {
            pos++;
        }
    }
    if(!is_int(arg1) && !is_pointer(arg1) && !is_register(arg1) && pos != line_trimmed.size()) {
        while(pos != line_trimmed.size() && (line_trimmed[pos-1] != '0' && line_trimmed[pos-2] != '\\')) {
            if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
                arg1 += line_trimmed[++pos];
                pos++;
            } 
            if(line_trimmed[pos] != '\'') {    
                arg1 += line_trimmed[pos++];
            } else {
                pos++;
            }
        }
    }
    vector<std::string> instruction_plus_args = {op, arg0, arg1};
    //std::cout << "op: " << op << " arg0: " << arg0 << " arg1: " << arg1 << std::endl;
    return instruction_plus_args;
}

/* makes a vector with the opcode, and operands, paired with their types */
vector<pair<std::string, ARG_TYPE>> parse_arg_types(vector<std::string> arg_vector) {

    vector<pair<std::string, ARG_TYPE>> arg_type_vec;

    for(std::string arg : arg_vector) {
        //std::cout << "\"" << arg << "\"" << std::endl;
        if(is_opcode(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, OPCODE));
        } else if(is_int(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, INTEGER));
        } else if(is_pointer(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, POINTER));
        } else if(is_register(arg)) {
            //std::cout << "\"" << arg << "\"" << std::endl;
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, REGISTER));
        } else {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, STRING));
        }
    }

    return arg_type_vec;
}


/* peices things together for machine code instruction */
std::string builder(vector<pair<std::string, ARG_TYPE>> instr, int A, int reg, int integer, int str, int ptr) {
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
                B = integer;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            case REGISTER:
                B = reg;
                C = str_to_reg[instr[1].first];
                D = str_to_reg[instr[2].first];
                break;
            case STRING:
                B = str;
                C = 0x0;
                if(A != 0xD) {
                    D = str_to_reg[instr[1].first];
                    str_arg = instr[2].first;
                    is_str_arg = true;
                } else {
                    D = 0x0;
                    str_arg = instr[1].first;
                    if(LABEL_MAP.find(str_arg)->second == 0) {
                        LABEL_MAP.insert({str_arg, CURRENT_LABEL_VALUE});
                        int_arg = CURRENT_LABEL_VALUE++;
                    } else {
                        int_arg = LABEL_MAP.find(str_arg)->second;
                    }
                    is_int_arg = true;
                }
                break;
            case POINTER:
                B = ptr;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            default:
                std::cout << "Something wrong in general" << std::endl;
                exit(1);
        }
    }
    int inst = (A << 12) | (B << 8) | (C << 4) | (D << 0);
    std::ostringstream os;
    os << "0x" << std::hex << inst << '\n';
    if(is_int_arg) {
        os << "0x" << int_arg << '\n';
    }
    if(is_str_arg) {
        os << str_arg << '\n';
    }
    return os.str();
}

/* Generates machine code from the assembly file: filename */
std::ostringstream gen_machine_code(std::string filename) {
    std::ostringstream os;
    std::ifstream file(filename); 
    std::string line = "";
    int line_number = 1;
    while(std::getline(file, line)) {        
        std::cout << line << std::endl;
        if(line != "") {
            std::string line_trimmed = trim(line);
            if(is_instruction(line_trimmed)) {
                vector<std::string> instruction_vector = parse_instruction(line);
                auto arg_type_vector = parse_arg_types(instruction_vector);
                std::string op = arg_type_vector[0].first;
                int op_bits  = op_to_int[op][0];
                int reg_bits = op_to_int[op][1];
                int int_bits = op_to_int[op][2];
                int str_bits = op_to_int[op][3];
                int ptr_bits = op_to_int[op][4];
                os << builder(arg_type_vector, op_bits, reg_bits, int_bits, str_bits, ptr_bits);
            } else if(is_comment(line)) {
                os << line << '\n';
            } else if(is_label(line)) {
                std::string label = trim_label(line);
                os << "#" << label << '\n';
                os << "0x" << std::hex << 0xDF00 << '\n';
                if(LABEL_MAP.find(label)->second == 0) {
                    LABEL_MAP.insert({trim_label(line), CURRENT_LABEL_VALUE});
                    os << "0x" << std::hex << CURRENT_LABEL_VALUE++ << '\n';
                } else {
                    os << "0x" << std::hex << LABEL_MAP.find(label)->second << '\n';
                }
            } else {
                std::cout << "Invalid Line: ";
                std::cout << line;
                std::cout << " @" << line_number << std::endl;
                exit(1); 
            }
            ++line_number;
        }
    }

    return os;
}

int main() {
    std::cout << "Filename: ";
    std::string filename = "";
    std::cin >> filename;
    std::ostringstream machine_code = gen_machine_code(filename);
    std::string ofilename = filename + ".inst";
    std::ofstream out_file(ofilename);
    out_file << machine_code.str();
    std::cout << std::endl;
    std::cout << machine_code.str() << std::endl;
    std::cout << "Program Compiled Successfully :)\n" << std::endl;
    return 0;
}