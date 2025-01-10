print(
    f"************************"
    f"Header File Generator"
    f"************************")


file_name = input("What should the file name be?: ")

syntax_file_name = file_name.upper() + "_H"

cpp_file_name = file_name + ".cpp"

file_name += ".h"

open(file_name, "x")


with open(file_name, "w") as file:
    x = f"#ifndef {syntax_file_name}\n#define {syntax_file_name}\n\n// structs\n\n// classes\n\n// variables\n\n// functions\n\n#endif"
    file.write(x)


with open(cpp_file_name, "w") as file:
    y = f"#include \"{file_name}\"\n\n// variables\n\n// functions\n\n"
    file.write(y)
