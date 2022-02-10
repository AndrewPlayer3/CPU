/*
    Andrew Player 2019
*/

#include "assembler.hpp"

/* This is an assembler that takes instr assembly and */
/* turns it into valid instr machine code.            */

/* This holds all current labels and the integers   */
/* that they get converted to in the machine code   */
/* this is global for use between functions without */
/* needed to pass it around                         */
std::map<std::string, int> LABEL_MAP;
int CURRENT_LABEL_VALUE = 0x0001;

/* Takes string for register enum */
map<std::string, REGISTERS> str_to_reg = {
    {"r0",  R0 },
    {"r1",  R1 },
    {"r2",  R2 },
    {"r3",  R3 },
    {"r4",  R4 },
    {"r5",  R5 },
    {"r6",  R6 },
    {"r7",  R7 },
    {"r8",  R8 },
    {"r9",  R9 },
    {"r10", R10},
    {"r11", R11},
    {"r12", R12},
    {"r13", R13},
    {"r14", R14},
    {"r15", R15},
};

/* map from opcode to vector of the digits that can be assembled                  */
/* the last three digits are the possible second digits (aka A*CD)                */
/* when the last operand is a register, integer, string, or pointer, respectively */
map<std::string, vector<int>> op_to_int = {
    {"mov", {0xE, 0x1, 0x0, 0x3, 0xF}},
    {"ext", {0x0, 0x0, 0x0, 0x0, 0x0}},
    {"run", {0xD, 0x9, 0x9, 0x9, 0x0}},
    {"ret", {0xD, 0x8, 0x8, 0x8, 0x8}},
    {"lnk", {0xF, 0xF, 0xF, 0xF, 0x0}},
    {"psh", {0xE, 0xB, 0xB, 0xB, 0x0}},
    {"pop", {0xE, 0xC, 0xC, 0xC, 0x0}},
    {"psa", {0xE, 0xD, 0xD, 0xD, 0x0}},
    {"ppa", {0xE, 0xE, 0xE, 0xE, 0x0}},
    {"mem", {0xE, 0x6, 0x6, 0x6, 0x0}},
    {"jmp", {0xD, 0x0, 0x0, 0x0, 0x0}},
    {"je" , {0xD, 0x2, 0x2, 0x2, 0x0}},
    {"jl" , {0xD, 0x3, 0x3, 0x3, 0x0}},
    {"jg" , {0xD, 0x4, 0x4, 0x4, 0x0}},
    {"jle", {0xD, 0x5, 0x5, 0x5, 0x0}},
    {"jge", {0xD, 0x6, 0x6, 0x6, 0x0}},
    {"cmp", {0xD, 0x1, 0x1, 0x1, 0x0}},
    {"cms", {0xD, 0x7, 0x7, 0x7, 0x0}},
    {"add", {0xA, 0x0, 0x5, 0x0, 0x0}},
    {"sub", {0xA, 0x1, 0x6, 0x0, 0x0}},
    {"mul", {0xA, 0x2, 0x7, 0x0, 0x0}},
    {"mod", {0xA, 0x3, 0x8, 0x0, 0x0}},
    {"div", {0xA, 0x4, 0x9, 0x0, 0x0}},
    {"and", {0xB, 0x0, 0x6, 0x0, 0x0}},
    {"or" , {0xB, 0x1, 0x7, 0x0, 0x0}},
    {"xor", {0xB, 0x4, 0xB, 0x0, 0x0}},
    {"rsh", {0xB, 0x3, 0x9, 0x0, 0x0}},
    {"lsh", {0xB, 0x2, 0x8, 0x0, 0x0}},
    {"not", {0xB, 0x5, 0xA, 0x0, 0x0}},
    {"ini", {0xE, 0x2, 0x2, 0x2, 0x0}},
    {"ins", {0xE, 0x8, 0x8, 0x8, 0x0}},
    {"out", {0xF, 0x0, 0x0, 0x0, 0x7}},
    {"put", {0xF, 0x3, 0x3, 0x3, 0x0}},
    {"pln", {0xF, 0x4, 0x4, 0x4, 0x0}},
    {"mdp", {0xF, 0x1, 0x1, 0x1, 0x0}},
    {"rdp", {0xF, 0x2, 0x2, 0x2, 0x0}},
    {"nop", {0x0, 0x0, 0x0, 0x0, 0x0}}
};

/* Map to turn tags to their machine code ints */
map<std::string, int> tag_to_int {
    {".main"    , 0xFF00},
    {".function", 0xFF11},
    {".global"  , 0xFF22},
    {".filler"  , 0xFF33},
    {".filler"  , 0xFF44},
    {".filler"  , 0xFF55}
};

/* Remove ws from beginning and end of line*/
std::string trim(const std::string& s) {
    std::size_t s_begin = s.find_first_not_of(" \t");
    if(s_begin != std::string::npos) {
        std::size_t s_end   = s.find_last_not_of(" \t");
        std::string s_trimmed = s.substr(s_begin, s_end - s_begin + 1);
        if(s_trimmed == "") {
            std::cout << "trimmed " << s << " to nothing" << std::endl;
        }
        return s_trimmed;
    }
    return "";
}

bool is_tag(const std::string& line) {
    std::string line_trimmed = trim(line);
    if(line_trimmed[0] != '.') return false;
    return true;
}

/* gets just the label name before the ':' */
std::string trim_label(const std::string& label) {
    std::size_t pos = 0;
    std::string label_no_ws = trim(label);
    std::string label_trimmed = "";
    while(pos != label_no_ws.size() && label_no_ws[pos] != ':') {
        label_trimmed += label_no_ws[pos];
        pos++;
    }
    return label_trimmed;
}

bool is_label(const std::string& line) {
    std::size_t pos = 0;
    std::string line_trimmed = trim(line);
    while(++pos != line_trimmed.size() && line_trimmed[pos] != ':');
    if(line_trimmed[pos] != ':') return false;
    while(iswspace(line_trimmed[++pos]));
    if(line_trimmed[pos] != '#' && line_trimmed[pos] != '\0') return false;
    return true;
}

/* checks if str is a valid opcode mnemonic */
bool is_opcode(const std::string& str) {
    if(op_to_int.find(str) == op_to_int.end())
        return false;
    return true;
}

/* returns true if line is a comment */
bool is_comment(const std::string& line) {
    if(line.size() > 0 && trim(line).size() > 0 && trim(line)[0] != '#')
        return false;
    return true;
}

/* returns true if line is a register */
bool is_register(const std::string& str) {
    if(str == "") return false;
    if(str_to_reg.find(str) == str_to_reg.end()) return false;
    return true;
}

/* returns true if line is an instruction */
bool is_instruction(const std::string& line) {
    bool prop = is_opcode(trim(line.substr(0, 3)));
    if(prop) return true;
    return false;
}

/* returns true if the string represents a pointer */
bool is_pointer(const std::string& str) {
    std::string str_trimmed = trim(str);
    if(str_trimmed[0] == '[' && str_trimmed[str_trimmed.size() - 1] == ']'
    && is_int(str_trimmed.substr(1, str_trimmed.size() - 2)))
        return true;
    return false;
}

/* returns true if line is an integer */
bool is_int(const std::string& str) {
    int num;
    std::istringstream is(str);
    if(is >> num) return true;
    return false;
}

/* convert str to an integer if possible */
/* exits with code 1 if not              */
int to_int(const std::string& str) {
    if(is_int(str)) {
        int number;
        std::istringstream is(str);
        is >> number;
        return number;
    }
    std::cout << "ERROR: Tried to convert " << str << "to an int." << std::endl;
    exit(1);
}

/* makes a vector with the opcode and its operands from a givin line */
vector<std::string> parse_instruction(const std::string& line) {
    std::size_t pos = 0;
    std::string op = "";
    std::string arg0 = "";
    std::string arg1 = "";
    std::string line_trimmed = trim(line);
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) op += line_trimmed[pos++];
    while(pos != line_trimmed.size() && iswspace(line_trimmed[pos])) pos++;
    while(pos != line_trimmed.size()
    && (!iswspace(line_trimmed[pos]))
    && line_trimmed[pos] != ',') {
        arg0 += line_trimmed[pos++];
    }
    while(pos != line_trimmed.size()
    && (iswspace(line_trimmed[pos])
    || line_trimmed[pos] == ',')) {
        pos++;
    }
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) {
        if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
            arg1 += line_trimmed[++pos];
            pos++;
        }
        if(line_trimmed[pos] != '\'') arg1 += line_trimmed[pos++];
        else pos++;
    }
    if(!is_int(arg1) && !is_register(arg1) && pos != line_trimmed.size()) {
        while(pos != line_trimmed.size()
        && (line_trimmed[pos-1] != '0' && line_trimmed[pos-2] != '\\')) {
            if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
                arg1 += line_trimmed[++pos];
                pos++;
            }
            if(line_trimmed[pos] != '\'') arg1 += line_trimmed[pos++];
            else pos++;
        }
    }
    vector<std::string> instruction_plus_args = {op, arg0, arg1};
    return instruction_plus_args;
}

/* makes a vector with the opcode, and operands, paired with their types */
/* NOTE: This has no error handling, only pass it validated instructions */
vector<pair<std::string, ARG_TYPE>> parse_arg_types(const vector<std::string>& arg_vector) {
    /* P.S. ARG_TYPE = {OPCODE, INTEGER, REGISTER, STRING} */
    vector<pair<std::string, ARG_TYPE>> arg_type_vec;
    for(std::string arg : arg_vector) {
        if(is_opcode(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, OPCODE));
        } else if(is_int(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, INTEGER));
        } else if(is_register(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, REGISTER));
        } else if(is_pointer(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, POINTER));
        } else if(arg == "") {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, EMPTY));
        } else {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, STRING));
        }
    }
    // If the second argument is empty swap it with the first one
    // This is so we can always switch off of the second argument
    if(arg_type_vec[2].second == EMPTY) {
        auto arg = arg_type_vec[1].first;
        auto argt = arg_type_vec[1].second;
        arg_type_vec[2].first = arg;
        arg_type_vec[2].second = argt;
        arg_type_vec[1].first = "";
        arg_type_vec[1].second = EMPTY;
    }
    return arg_type_vec;
}

/* takes in an instruction (arg_type) vector along with its corresponding    */
/* opcode's information then generates the machine code for that instruction */
std::string builder(const vector<pair<std::string, ARG_TYPE>>& instr) {
    std::string op = instr[0].first;
    int op_bits  = op_to_int[op][0];
    int reg_bits = op_to_int[op][1];
    int int_bits = op_to_int[op][2];
    int str_bits = op_to_int[op][3];
    int ptr_bits = op_to_int[op][4];
    int B = 0x0, C = 0x0, D = 0x0;
    bool is_int_arg = false;
    int int_arg = 0x0;
    bool is_str_arg = false;
    int ptr_arg = 0x0;
    bool is_ptr_arg = false;
    std::string str_arg = "";
    if(instr.size() == 3) {
        switch(instr[2].second) {

            case INTEGER:
                // For the ptr cases here and in register this is not robust
                // and a better solution should be found.
                if(instr[1].second == POINTER) {
                    B = 0x7;
                    std::istringstream is(instr[1].first.substr(1, instr[1].first.size() - 2));
                    is >> std::hex >> ptr_arg;
                    is_ptr_arg = true;
                } else {
                    B = int_bits;
                    D = str_to_reg[instr[1].first];
                }
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;

            case REGISTER:
                if(instr[1].second == POINTER) {
                    B = 0x6;
                    D = str_to_reg[instr[2].first];
                    std::istringstream is(instr[1].first.substr(1, instr[1].first.size() - 2));
                    is >> std::hex >> ptr_arg;
                    is_ptr_arg = true;
                } else {
                    B = reg_bits;
                    C = str_to_reg[instr[1].first];
                    D = str_to_reg[instr[2].first];
                }
                break;

            case STRING:
                B = str_bits;
                if(op_bits != 0xD) {
                    D = str_to_reg[instr[1].first];
                    str_arg = instr[2].first;
                    is_str_arg = true;
                } else {
                    str_arg = instr[2].first;
                    if(LABEL_MAP.find(str_arg) == LABEL_MAP.end()) {
                        LABEL_MAP.insert({str_arg, CURRENT_LABEL_VALUE});
                        int_arg = CURRENT_LABEL_VALUE++;
                    } else {
                        int_arg = LABEL_MAP.find(str_arg)->second;
                    }
                    is_int_arg = true;
                }
                break;

            case POINTER:
                if(instr[1].second == POINTER) {
                    B = 0x5;
                    std::istringstream is(instr[1].first.substr(1, instr[1].first.size() - 2));
                    is >> std::hex >> ptr_arg;
                    is_ptr_arg = true;
                } else {
                    B = ptr_bits;
                            D = str_to_reg[instr[1].first];
                }
                {std::istringstream is(instr[2].first.substr(1, instr[2].first.size() - 2));
                is >> std::hex >> int_arg;}
                is_int_arg = true;
                break;

            case EMPTY: // Only ret should end up here. 🤞
                B = reg_bits;
                break;

            default:
                std::cout << "Builder function didn't match a case somehow." << std::endl;
                exit(1);
        }
    }
    int inst = (op_bits << 12) | (B << 8) | (C << 4) | (D << 0);
    std::ostringstream os;
    os << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << inst << '\n';
    if(is_ptr_arg) os << "0x" << std::setfill('0') << std::setw(4) << std::right << ptr_arg << '\n';
    if(is_int_arg) os << "0x" << std::setfill('0') << std::setw(4) << std::right << int_arg << '\n';
    if(is_str_arg && str_arg != "") os << str_arg << '\n';
    return os.str();
}

/* Generates machine code from the assembly file: filename  */
/* returns an ostringstream with the generated machine code */
std::ostringstream gen_machine_code(const std::string& filename) {
    std::ostringstream os;
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cout << "\nError opening file: " << filename << std::endl;
        exit(1);
    }
    std::string line = "";
    int line_number = 1;
    while(std::getline(file, line)) {
        if(line != "") {
            std::string line_trimmed = trim(line);
            // If its an instruction build the machine code
            if(is_instruction(line_trimmed) && !is_label(line_trimmed)) {
                vector<std::string> instruction_vector = parse_instruction(line);
                std::vector<std::pair<std::string, ARG_TYPE>> arg_type_vector = parse_arg_types(instruction_vector);
                if(arg_type_vector[0].first == "lnk") {
                    os << gen_machine_code(arg_type_vector[2].first).str();
                } else {
                    os << builder(arg_type_vector);
                }
            }
            // If its a comment there is nothing to do
            else if(is_comment(line)) {
                // Do Nothing :D
            }
            // If its a label find/create the label number and insert it
            else if(is_label(line)) {
                std::string label = trim_label(line);
                os << "# Label: " << label << '\n';
                os << "0x" << std::hex << 0xDF00 << '\n';
                if(LABEL_MAP.find(label) == LABEL_MAP.end()) {
                    LABEL_MAP.insert({trim_label(line), CURRENT_LABEL_VALUE});
                    os << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << CURRENT_LABEL_VALUE++ << '\n';
                } else {
                    os << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << LABEL_MAP.find(label)->second << '\n';
                }
            }
            // If its a tag find/create the tag number and insert it
            else if(is_tag(line)) {
                vector<std::string> args = parse_instruction(line_trimmed);
                if(tag_to_int.find(args[0]) != tag_to_int.end()) {
                    if(args[0] == ".main") {
                        os << "<<<===Main===>>> " << '\n';
                    } else {
                        os << "# Function: " << args[1] << '\n';
                    }
                    os << "0x" << std::hex << tag_to_int[args[0]] << '\n';
                    if(args[1] != "") {
                        os << "0x" << std::hex << 0xDF00 << '\n';
                        if(LABEL_MAP.find(args[1]) == LABEL_MAP.end()) {
                            LABEL_MAP.insert({args[1], CURRENT_LABEL_VALUE});
                            os << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << CURRENT_LABEL_VALUE++ << '\n';
                        } else {
                            os << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << LABEL_MAP.find(args[1])->second << '\n';
                        }
                    }
                } else {
                    std::cout << "Invalid Tag: ";
                    std::cout << line_trimmed;
                    std::cout << " @ " << line_number << std::endl;
                    exit(1);
                }
            }
            // Since the line didn't match anything it must be invalid
            else {
                std::cout << "Invalid Line: ";
                std::cout << line_trimmed;
                std::cout << " @ " << line_number << std::endl;
                exit(1);
            }
        }
        ++line_number;
    }
    return os;
}

std::ostringstream gen_code_from_line(std::string& line) {
    std::ostringstream os;
    if(line != "") {
        std::string line_trimmed = trim(line);
        // If its an instruction build the machine code
        if(is_instruction(line_trimmed) && !is_label(line_trimmed)) {
            vector<std::string> instruction_vector = parse_instruction(line);
            auto arg_type_vector = parse_arg_types(instruction_vector);
            if(arg_type_vector[0].first == "lnk") {
                os << gen_machine_code(arg_type_vector[2].first).str();
            } else {
                os << builder(arg_type_vector);
            }
        }
        // If its a comment there is nothing to do
        else if(is_comment(line)) {
            // Do Nothing :D
        }
        // If its a label find/create the label number and insert it
        else if(is_label(line)) {
            std::string label = trim_label(line);
            os << "# Label: " << label << '\n';
            os << "0x" << std::hex << 0xDF00 << '\n';
            if(LABEL_MAP.find(label) == LABEL_MAP.end()) {
                LABEL_MAP.insert({trim_label(line), CURRENT_LABEL_VALUE});
                os << "0x" << std::hex << CURRENT_LABEL_VALUE++ << '\n';
            } else {
                os << "0x" << std::hex << LABEL_MAP.find(label)->second << '\n';
            }
        }
        // If its a tag find/create the tag number and insert it
        else if(is_tag(line)) {
            vector<std::string> args = parse_instruction(line_trimmed);
            if(tag_to_int.find(args[0]) != tag_to_int.end()) {
                if(args[0] == ".main") {
                    os << "<<<===Main===>>> " << '\n';
                } else {
                    os << "# Function: " << args[1] << '\n';
                }
                os << "0x" << std::hex << tag_to_int[args[0]] << '\n';
                if(args[1] != "") {
                    os << "0x" << std::hex << 0xDF00 << '\n';
                    if(LABEL_MAP.find(args[1]) == LABEL_MAP.end()) {
                        LABEL_MAP.insert({args[1], CURRENT_LABEL_VALUE});
                        os << "0x" << std::hex << CURRENT_LABEL_VALUE++ << '\n';
                    } else {
                        os << "0x" << std::hex << LABEL_MAP.find(args[1])->second << '\n';
                    }
                }
            } else {
                std::cout << "Invalid Tag: ";
                std::cout << line_trimmed;
                exit(1);
            }
        }
        // Since the line didn't match anything it must be invalid
        else {
            std::cout << "Invalid Line: ";
            std::cout << line_trimmed;
            exit(1);
        }
    }
    return os;
}
