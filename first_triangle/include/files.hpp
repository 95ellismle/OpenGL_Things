#ifndef FILES_HEADER_GUARD
#define FILES_HEADER_GUARD

#include <iostream>
#include <fstream>


namespace IO {
    /*
      A class to simply read a file and store the txt.
    */
    class File {
        public:
            std::string file_txt;
            std::string file_path;
            
            /* Constructor */
            File(std::string fp) {
                file_path = fp;

                std::ifstream fin(file_path);

                // Check the file is openable
                if (fin.fail()) {
                    std::cerr << "Couldn't open file '" << file_path << "'. Please check it exists" << std::endl;
                    throw "IOError";
                }
                
                // Read the text and store it in tmp_txt
                std::string line;
                while (getline (fin, line)) {
                    file_txt = file_txt + line + "\n";
                }

                fin.close();
            }

            /* Write a file */
            void write (std::string fp) {
                std::ofstream out_file(fp);

                if (out_file.is_open()) {
                    out_file << file_txt;
                } else {
                    std::cerr << "Unable to open file!";
                }
            }
    };
}

#endif
