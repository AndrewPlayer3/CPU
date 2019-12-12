/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
DOESN'T WORK WITH 32-BIT MINGW/32-BIT GCC
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
map<string, REGISTERS> str_to_reg = {
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

/* map from opcode to vector of the digits that can be assembled            */
/* the last three digits are the possible second digits (aka A*CD)          */
/* when the last operand is a register, or integer, or string, respectively */
map<string, vector<int>> op_to_int = {
    {"mov", {0xE, 0x1, 0x0, 0x3}},
    {"jmp", {0xD, 0x0, 0x0, 0x0}},
    {"je" , {0xD, 0x2, 0x2, 0x2}},
    {"jl" , {0xD, 0x3, 0x3, 0x3}},
    {"jg" , {0xD, 0x4, 0x4, 0x4}},
    {"jle", {0xD, 0x5, 0x5, 0x5}},
    {"jge", {0xD, 0x6, 0x6, 0x6}},
    {"cmp", {0xD, 0x1, 0x1, 0x1}},
    {"add", {0xA, 0x0, 0x5, 0x0}},
    {"sub", {0xA, 0x1, 0x6, 0x0}},
    {"mul", {0xA, 0x2, 0x7, 0x0}},
    {"mod", {0xA, 0x3, 0x8, 0x0}},
    {"div", {0xA, 0x4, 0x9, 0x0}},
    {"and", {0xB, 0x0, 0x6, 0x0}},
    {"or" , {0xB, 0x1, 0x7, 0x0}},
    {"xor", {0xB, 0x4, 0xB, 0x0}},
    {"rsh", {0xB, 0x3, 0x9, 0x0}},
    {"lsh", {0xB, 0x2, 0x8, 0x0}},
    {"not", {0xB, 0x5, 0xA, 0x0}},
    {"ini", {0xE, 0x2, 0x2, 0x2}},
    {"ins", {0xE, 0x8, 0x8, 0x8}},
    {"out", {0xF, 0x0, 0x0, 0x0}},
    {"put", {0xF, 0x3, 0x3, 0x3}},
    {"pln", {0xF, 0x4, 0x4, 0x4}},
    {"mdp", {0xF, 0x1, 0x1, 0x1}},
    {"rdp", {0xF, 0x2, 0x2, 0x2}},
    {"nop", {0x0, 0x0, 0x0, 0x0}}
};

/* These are all the opcode mnemonics for now    */
vector<std::string> opcode_vector = {
    "mov",
    "jmp",
    "je" ,
    "jl" ,
    "jg" ,
    "jle",
    "jge",
    "cmp",
    "add",
    "sub",
    "mul",
    "mod",
    "div",
    "and",
    "or" ,
    "xor",
    "rsh",
    "lsh",
    "not",
    "ini",
    "ins",
    "out",
    "put",
    "pln",
    "mdp",
    "rdp",
    "nop"
};
/* btw                                           */
/* std::map::count() was giving us trouble thats */
/* why this exists along with the op_to_int map  */

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

bool is_label(const std::string& line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    while(++pos != line_trimmed.size() && line_trimmed[pos] != ':');
    if(line_trimmed[pos] != ':') {
        return false;
    }
    return true;
}

/* gets just the label name before the ':' */
std::string trim_label(const std::string& label) {
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
bool is_opcode(const std::string& str) {
    for(std::string s : opcode_vector) {
        if(str == s) return true;
    }
    return false;
}

/* returns true if line is a comment */
bool is_comment(const std::string& line) {
    if(line.size() > 0 && trim(line).size() > 0 && trim(line)[0] != '#') {
        return false;
    } 
    return true;
} 
/* returns true if line is a register */
bool is_register(const std::string& str) {
    if(str_to_reg.count(str) == 0) {
        return false;
    }
    return true;
}

/* returns true if line is an instruction */
bool is_instruction(const std::string& line) {
    bool prop = is_opcode(trim(line.substr(0, 3)));
    if(prop) {
        return true;
    }
    return false;
}

/* returns true if line is an integer */
bool is_int(const std::string& str) {
    int num;
    std::istringstream is(str);
    if(is >> num) {
        return true;
    }
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
    int pos = 0;
    std::string line_trimmed = trim(line);
    std::string op = "";
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) {
        op += line_trimmed[pos++];
    }
    if(!is_opcode(op)) {
        std::cout << "Invalid Instruction: " << op << std::endl;
        exit(1);
    }
    while(pos != line_trimmed.size() && iswspace(line_trimmed[pos])) {
        pos++;
    }
    std::string arg0 = "";
    std::string arg1 = "";
    while(pos != line_trimmed.size() 
    && (!iswspace(line_trimmed[pos])) && line_trimmed[pos] != ',') {
        arg0 += line_trimmed[pos++];
    }
    while(pos != line_trimmed.size() 
    && (iswspace(line_trimmed[pos]) || line_trimmed[pos] == ',')) {
        pos++;
    }
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) {
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
    if(!is_int(arg1) && !is_register(arg1) && pos != line_trimmed.size()) {
        while(pos != line_trimmed.size() 
        && (line_trimmed[pos-1] != '0' && line_trimmed[pos-2] != '\\')) {
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
    return instruction_plus_args;
}

/* makes a vector with the opcode, and operands, paired with their types */
/* NOTE: This has no error handling, only pass it validated instructions */
vector<pair<std::string, ARG_TYPE>> parse_arg_types(const vector<std::string>& arg_vector) {
    vector<pair<std::string, ARG_TYPE>> arg_type_vec;
    for(std::string arg : arg_vector) {
        if(is_opcode(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, OPCODE));
        } else if(is_int(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, INTEGER));
        } else if(is_register(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, REGISTER));
        } else {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, STRING));
        }
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
    int B, C, D = 0x0;
    bool is_int_arg = false;
    int int_arg = 0x0;
    bool is_str_arg = false;
    std::string str_arg = "";
    if(instr.size() == 3) {
        switch(instr[2].second) {
            case INTEGER:
                B = int_bits;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            case REGISTER:
                B = reg_bits;
                C = str_to_reg[instr[1].first];
                D = str_to_reg[instr[2].first];
                break;
            case STRING:
                B = str_bits;
                C = 0x0;
                if(op_bits != 0xD) {
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
            default:
                std::cout << "Something wrong in general" << std::endl;
                exit(1);
        }
    }
    int inst = (op_bits << 12) | (B << 8) | (C << 4) | (D << 0);
    std::ostringstream os;
    os << "0x" << std::hex << inst << '\n';
    if(is_int_arg) {
        os << "0x" << int_arg << '\n';
    }
    if(is_str_arg && str_arg != "") {
        os << str_arg << '\n';
    }
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
            if(is_instruction(line_trimmed)) {
                vector<std::string> instruction_vector = parse_instruction(line);
                auto arg_type_vector = parse_arg_types(instruction_vector);
                os << builder(arg_type_vector);
            } else if(is_comment(line)) {
                os << trim(line) << '\n';
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
