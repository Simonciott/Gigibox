LIBS=-lsfml-graphics -lsfml-window -lsfml-system
LIB_PATH=../SFML/lib/
STATIC_LIBS=$(LIB_PATH)libsfml-graphics-s.a $(LIB_PATH)libsfml-window-s.a $(LIB_PATH)libsfml-system-s.a
INCLUDE=-I../SFML/include
LIB=-L$(LIB_PATH)
CXX := g++

all: main

%.o: %.cpp
	$(CXX) -c $< $(INCLUDE) $(LIB) -fpermissive -o $@

main: main.o Assembly/Assembly.o Assembly/Instructions.o Game/Buffers.o Game/Input.o Game/Sprite.o Game/Texture.o System/Arguments.o System/CommonFunctions.o System/LanguageMessages.o
	@echo "** Building the game"
	$(CXX) -o Gigibox main.o Assembly/Assembly.o Assembly/Instructions.o Game/Buffers.o Game/Input.o Game/Sprite.o Game/Texture.o System/Arguments.o System/CommonFunctions.o System/LanguageMessages.o $(LIB) $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	fish -c "rm -f Gigibox **.o"
