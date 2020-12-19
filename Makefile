include makefile.vars

$(BIN)/cache_sim: $(OBJECTS)/CacheLine.o $(OBJECTS)/DirectMap.o $(OBJECTS)/Dispatcher.o $(OBJECTS)/main.o
	$(GCC) $(OBJECTS)/CacheLine.o $(OBJECTS)/DirectMap.o $(OBJECTS)/Dispatcher.o $(OBJECTS)/main.o -o $(BIN)/cache_sim $(LINKER_LIBS)

$(OBJECTS)/main.o: $(SOURCES)/main.cpp
	$(GCC) $(GCC_FLAGS)  $(INCLUDES) $(SOURCES)/main.cpp -o $(OBJECTS)/main.o

$(OBJECTS)/CacheLine.o: $(SOURCES)/CacheLine.cpp
	$(GCC) $(GCC_FLAGS)  $(INCLUDES) $(SOURCES)/CacheLine.cpp -o $(OBJECTS)/CacheLine.o

$(OBJECTS)/DirectMap.o: $(SOURCES)/DirectMap.cpp
	$(GCC) $(GCC_FLAGS)  $(INCLUDES) $(SOURCES)/DirectMap.cpp -o $(OBJECTS)/DirectMap.o

$(OBJECTS)/Dispatcher.o: $(SOURCES)/Dispatcher.cpp
	$(GCC) $(GCC_FLAGS)  $(INCLUDES) $(SOURCES)/Dispatcher.cpp -o $(OBJECTS)/Dispatcher.o

clean:
	rm -rf ./bin/* ./objects/*
