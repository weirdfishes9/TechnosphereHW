#include <iostream>

#include "process.h"

int main(int argc, char const *argv[]) {
    process::Process test_proc("cat", {"-E"});

    const char message1[] = "Message1\n";
    test_proc.writeExact(message1, sizeof(message1));
    char str1[sizeof(message1) + 1];
    test_proc.readExact(str1, sizeof(message1) + 1);
    std::cout << str1 << std::endl;

    const char message2[] = "Message2\n";
    test_proc.write(message2, sizeof(message2));
    char str2[sizeof(message2) + 1];
    test_proc.read(str2, sizeof(message2) + 1);
    std::cout << str2 << std::endl;

    test_proc.close();

    return 0;
}