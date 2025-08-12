
#include <Enxel.h>
#include <string>
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    // Setting working dir into Asset folder
    // Need refactoring later
	if (argc >= 3 && std::string(argv[1]) == "-C") 
	{
        std::string assetDir = argv[2];
        
        try 
		{
            std::filesystem::current_path(assetDir);
            std::cout << "Working directory set to: " << std::filesystem::current_path() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Could not change to directory '" << assetDir << "': " << e.what() << std::endl;
            return 1;
        }
    } else 
	{
        std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
    }

	Enxel::Enxel enxel;
	enxel.StartEngine();

}
