#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector <std::vector<std::string>> readFile(std::string sFilename)
	{
        std::vector < std::vector<std::string> > s;

        std::ifstream f(sFilename);
        if (!f.is_open()){
            std::vector < std::string> lins;
            lins.push_back("");
            s.push_back(lins);
            return s;
        }

		while (!f.eof())
		{
            std::string line;
            std::getline(f, line);
            
            if (!line.empty()){
                if (line.at(0) != '#'){
                    std::istringstream iss(line);
                    std::string token;
                    
                    std::vector < std::string> lins;
                    while(std::getline(iss, token, ';'))  
                        lins.push_back(token);

                    s.push_back(lins);
	    }}}
        return s;
    }



int main(int argc, char *argv[])
{
    if (argc > 1){
        std::string inputFile = argv[1];
        std::vector <std::vector<std::string>> Input;
        Input = readFile( inputFile );
        
        if (Input.size() > 0)
            for ( int ln = 0; ln < Input.size(); ln++){
                for ( int wn = 0; wn < Input[ln].size(); wn++)
                    std::cout << Input[ln][wn];
                std::cout << std::endl;
                }
    } else {
        std::cout << "Please provide filename as parameter...";
        std::cout << std::endl;
    }
	return 0;
}
