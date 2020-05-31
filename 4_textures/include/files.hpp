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
             Will just count how many lines there are in the file.
            */
            unsigned int count_num_lines () {
                std::ifstream fin(file_path);

                std::string line;
                unsigned int num_lines=0;
                while (getline(fin, line))
                    num_lines++;

                fin.close();

                return num_lines;
            }

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
     A base class for a 2D array file, the various derivative of the array file can be subclassed from this e.g. IntArrayFile.

     An array file takes the following form:
        vec1a vec1b vec1c ... vec1N
        vec2a vec2b vec2c ... vec2N
        vec3a vec3b vec3c ... vec3N
                .
                .
                .
        vecMa vecMb vecMc ... vecMN

    */
    class ArrayFile : public File {
        private:
        protected:
            float tmp;
            unsigned int elem_count;

            void virtual allocate_arrays() {}
            unsigned int virtual add_to_data(std::string &line) { return 0; }

            /*
             Will print an error message and throw an error if the number of elements in a vector isn't correct
            */
            void check_num_elem(unsigned int n_elem, unsigned int iline, std::string &line) {
                if (n_elem != num_arr_elem) {
                    std::cerr << "Incorrect number of vertices in line " << iline + 1;
                    std::cerr << "\n\t* Line = '" << line << "'";
                    std::cerr << "\n\t* Num Verts Found = " << n_elem;
                    std::cerr << "\n\t* Line No = " << iline + 1 << std::endl;
                    //throw "ArrayFileError";
                }
            }

            /*
             Will count how many elements are in the first vector and assume the rest of the
             vectors declared have the same length.
            */
            unsigned int count_num_arr_elem(std::string line) {
                std::stringstream ss(line);

                unsigned int count=0;
                while (ss >> tmp) {
                    count++;
                }

                return count;
            }

            void virtual parse_1_line(std::string &line, unsigned int iline=0) { }

            /*
             Will parse all the lines in the data file.

             See File class for more details.
            */
            void parse_lines(std::ifstream &fin) override {
                std::string line;
                unsigned int iline=0;
                unsigned int count=0;
                elem_count = 0;

                // Get how many lines there are in the file
                num_vertices = count_num_lines();
                // Get how many elements in each vector
                getline(fin, line);
                num_arr_elem = count_num_arr_elem(line);

                // Allocate the data structure
                allocate_arrays();

                // Parse the first line (as we already have it)
                count=add_to_data(line);
                check_num_elem(count, iline, line);
                iline++;

                // Parse each following line
                while (getline(fin, line)) {
                    count=add_to_data(line);
                    check_num_elem(count, iline, line);
                    iline++;
                }

                // Remember to close the file
                fin.close();
            }

        public:
            void virtual print(int *data) {
                for (unsigned int i=0; i<num_vertices; i++) {
                    for (unsigned int j=0; j<num_arr_elem-1; j++) {
                        std::cout << data[(i*num_arr_elem) + j]<< ", ";
                    }
                    std::cout << data[((i+1) * num_arr_elem) -1] << std::endl;
                }
            }
            void virtual print(float *data) {
                for (unsigned int i=0; i<num_vertices; i++) {
                    for (unsigned int j=0; j<num_arr_elem-1; j++) {
                        std::cout << data[(i*num_arr_elem) + j]<< ", ";
                    }
                    std::cout << data[((i+1) * num_arr_elem) -1] << std::endl;
                }
            }

            unsigned int num_vertices;
            unsigned int num_arr_elem;
            unsigned int length;
            size_t size;
    };

    /*
     Will read a list of ints. See ArrayFile for more info.
    */
    class IntArrayFile : public ArrayFile {
        protected:
            int tmp = 0;

            /*  Will add the data from a line into the data variable.  */
            unsigned int add_to_data(std::string &line) override {
                unsigned int count=0;
                std::stringstream ss(line);
                
                while (ss >> tmp) {
                    data[elem_count] = tmp;
                    elem_count++;
                    count++;
                }

                return count;
            }

            /*
             Will allocate the int array that hold the data and sets the size variable.
            */
            void allocate_arrays() override {
                size = sizeof(int) * num_arr_elem * num_vertices;
                data = (int*) malloc(size);
            }

        public:
            void print() {
                ArrayFile::print(data);
            }
            int *data;
    };

    /*
     Will read a 2D array of floats. See ArrayFile for more info
    */
    class FloatArrayFile : public ArrayFile {
        protected:
            float tmp = 0;

            /*  Will add the data from a line into the data variable.  */
            unsigned int add_to_data(std::string &line) override {
                unsigned int count=0;
                std::stringstream ss(line);
                
                while (ss >> tmp) {
                    data[elem_count] = tmp;
                    elem_count++;
                    count++;
                }

                return count;
            }

            /*
             Will allocate the int array that hold the data and sets the size variable.
            */
            void allocate_arrays() override {
                length = num_arr_elem * num_vertices;
                size = sizeof(float) * length;
                data = (float*) malloc(size);
            }

        public:
            void print() {
                ArrayFile::print(data);
            }
            float *data;
    };

    // Overloading the maths operators for easy data manipulation
    FloatArrayFile operator+(FloatArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] + var2;
        }
        return var1;
    };
    FloatArrayFile operator-(FloatArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] - var2;
        }
        return var1;
    };
    FloatArrayFile operator*(FloatArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] * var2;
        }
        return var1;
    };
    FloatArrayFile operator/(FloatArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] / var2;
        }
        return var1;
    };
    // Overloading the maths operators for easy data manipulation
    IntArrayFile operator+(IntArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] + var2;
        }
        return var1;
    };
    IntArrayFile operator-(IntArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] - var2;
        }
        return var1;
    };
    IntArrayFile operator*(IntArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] * var2;
        }
        return var1;
    };
    IntArrayFile operator/(IntArrayFile &var1, const float &var2) {
        for (unsigned int i=0; i<var1.length; i++) {
            var1.data[i] = var1.data[i] / var2;
        }
        return var1;
    };
}

#endif

