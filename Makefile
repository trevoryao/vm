CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = vm
OBJECTS = vm.o controllers/input.o models/model-base.o models/text-model.o models/file.o \
	ui/window.o views/text-view.o actions/action.o actions/command.o actions/movement.o \
	views/status-view.o views/view-base.o models/text.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lncurses

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}