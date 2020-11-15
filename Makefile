CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = vm
OBJECTS = vm.o models/model-base.o models/text-display.o ui/file.o ui/window.o views/text-view.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}