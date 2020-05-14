EXE="square"
INCLUDES="-I./include"
SRC_FILES="./src/*.c"
LIBS="`pkg-config --static --libs glfw3`"

MAIN_CPP="main.cpp"

# Remove the exe before compiling
if [ -f "$EXE" ]
then
  rm $EXE
fi


g++ -g -Wall $INCLUDES $SRC_FILES $MAIN_CPP $LIBS -o $EXE
