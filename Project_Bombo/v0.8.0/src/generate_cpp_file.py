
print(
    f"************************"
    f"C++ File Generator"
    f"************************")

file_name = input("What should the file name be?: ")

file_name += ".cpp"

open(file_name, "x")
    
    
with open(file_name, "w") as file:
    x = "#include <iostream>\n\nint main()\n{\n\n    return 0;\n}\n"
    file.write(x)
    