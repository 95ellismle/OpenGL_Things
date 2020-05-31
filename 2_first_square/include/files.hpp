#ifndef FILES_HEADER_GUARD
#define FILES_HEADER_GUARD

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


namespace IO {
    /*
      A class to simply read a file and store the txt.
    */
    class File {
        private:

        protected:

            /*
              A callback function for the parse_lines func.

              Will just parse one line, this can be overriden if the file can be
              parsed line by line to make things a bit easier.

              This will set the file_txt attribute and any other data attributes.

              Inputs:
                     * line <std::string &> => The reference to the line

            */
            virtual void parse_1_line(std::string &line, unsigned int iline=0) {
                file_txt = file_txt + line + "\n";
            }

            /*
             A callback function to read the files txt line by line and pop 
             it in the std::string file_txt.

             This is the method to be overriden in any derived classes.

             This should have 1 argument -the filestream reference.

             This sets the file_txt attribute and any other data attributes
            */
            virtual void parse_lines(std::ifstream &fin) {
                std::string line;
                while (getline(fin, line)) {
                    parse_1_line(line);
                }
            }

        public:
            std::string file_txt;
            std::string file_path;
            
            /* 
             Constructor, doesn't do anything.
            */
            File() { }

            /* 
             Write the file_txt attribute to a file.

             Inputs:
               * fp <std::string> => The filepath to be written to.
            */
            virtual void write (std::string fp) {
                std::ofstream out_file(fp);

                if (out_file.is_open()) {
                    out_file << file_txt;
                } else {
                    std::cerr << "Unable to open file!";
                }
            }

            /*
             Will try to open a file, check it is openable, call the parsing function
             and then close the file.

             This shouldn't be overriden.
            */
            void read(std::string fp) {
                file_path = fp;
                std::ifstream fin(file_path);

                // Check the file is openable
                if (fin.fail()) {
                    std::cerr << "Couldn't open file '" << file_path;
                    std::cerr << "'. Please check it exists" << std::endl;
                    throw "IOError";
                }

                // Read the text and store it in tmp_txt
                parse_lines(fin);

                fin.close();
            }
    };


    /*
     Will read a list of ints

     The format of a vertices file is:
        x1 y1 z1
        x2 y2 z2
        ...
        xN yN zN
    */
    class IntArrayFile : public File {
        private:
            unsigned int elem_count = 0;
            int tmp = 0;
            void set_num_vertices() {
                std::ifstream fin(file_path);
                
                std::string line;
                num_vertices=0;
                while (getline(fin, line)) {
                    num_vertices++;
                }

                fin.close();
            }

        protected:
            /*
             Will parse 1 line in the data file.

             See the File class for more details.
            */
            void parse_1_line(std::string &line, unsigned int iline=0) override {
                // Declare some variables
                std::stringstream ss(line);
                unsigned int count=0;

                // Loop over lines and add data
                while (ss >> tmp) {
                    data[elem_count] = tmp;
                    elem_count++;
                    count ++;
                }

                if (count != 3) {
                    std::cerr << "Incorrect number of vertices in line " << iline + 1;
                    std::cerr << "\n\t* Line = '" << line << "'";
                    std::cerr << "\n\t* Num Verts Found = " << count;
                    std::cerr << "\n\t* Line No = " << iline + 1 << std::endl;
                    throw "FloatArrayFileError";
                }
            }

            /*
             Will parse all the lines in the data file.

             See File class for more details.
            */
            void parse_lines(std::ifstream &fin) override {
                std::string line;

                // Get how many lines there are in the file
                set_num_vertices();

                // Allocate the data structure
                size = sizeof(int) * 3  * num_vertices;
                data = (int*) malloc(size);

                // Parse each line
                unsigned int iline=0;
                while (getline(fin, line)) {
                    parse_1_line(line, iline);
                    iline++;
                }
            }

        public:
            unsigned int num_vertices;
            size_t size;
            int *data;
    };

    /*
     Will read a list of floats

     The format of a vertices file is:
        x1 y1 z1
        x2 y2 z2
        ...
        xN yN zN
    */
    class FloatArrayFile : public File {
        private:
            unsigned int elem_count = 0;
            void set_num_vertices() {
                std::ifstream fin(file_path);
                
                std::string line;
                num_vertices=0;
                while (getline(fin, line)) {
                    num_vertices++;
                }

                fin.close();
            }

        protected:
            /*
             Will parse 1 line in the data file.

             See the File class for more details.
            */
            void parse_1_line(std::string &line, unsigned int iline=0) override {
                // Declare some variables
                std::stringstream ss(line);
                unsigned int count=0;
                float tmp = 0;

                // Loop over lines and add data
                while (ss >> tmp) {
                    data[elem_count] = tmp;
                    elem_count++;
                    count ++;
                }

                if (count != 3) {
                    std::cerr << "Incorrect number of vertices in line " << iline + 1;
                    std::cerr << "\n\t* Line = '" << line << "'";
                    std::cerr << "\n\t* Num Verts Found = " << count;
                    std::cerr << "\n\t* Line No = " << iline + 1 << std::endl;
                    throw "FloatArrayFileError";
                }
            }

            /*
             Will parse all the lines in the data file.

             See File class for more details.
            */
            void parse_lines(std::ifstream &fin) override {
                std::string line;

                // Get how many lines there are in the file
                set_num_vertices();

                // Allocate the data structure
                size = sizeof(float) * 3  * num_vertices;
                data = (float*) malloc(size);

                // Parse each line
                unsigned int iline=0;
                while (getline(fin, line)) {
                    parse_1_line(line, iline);
                    iline++;
                }
            }

        public:
            unsigned int num_vertices;
            size_t size;
            float *data;
    };
}

#endif

