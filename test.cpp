#include <iostream>

// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */

int main()
{
    // Example usage
    std::cout << RED << "This text is red." << RESET << std::endl;
    std::cout << GREEN << "This text is green." << RESET << std::endl;
    std::cout << YELLOW << "This text is yellow." << RESET << std::endl;
    std::cout << BLUE << "This text is blue." << RESET << std::endl;
    std::cout << MAGENTA << "This text is magenta." << RESET << std::endl;
    std::cout << CYAN << "This text is cyan." << RESET << std::endl;
    std::cout << WHITE << "This text is white." << RESET << std::endl;

    return 0;
}