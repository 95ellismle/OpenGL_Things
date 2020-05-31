if [ -f "hello" ]
then
  rm hello
fi


g++ -g -Wall -I./include ./src/glad.c main.cpp `pkg-config --static --libs glfw3` -o hello
