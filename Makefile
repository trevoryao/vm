CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = vm
OBJECTS = actions/action.o actions/bad-entry.o actions/e-movement.o \
	actions/e-search.o actions/file-op.o actions/global.o \
	actions/incomplete.o actions/insert.o actions/keyboard.o \
	actions/movement.o actions/replace.o actions/scroll.o \
	actions/search.o actions/text-edit.o controllers/controller-base.o \
	controllers/input.o controllers/key-input.o models/file.o models/model-base.o \
	models/text-model.o models/text.o ui/window.o views/status-view.o \
	views/text-view.o views/view-base.o \
	vm.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lncurses

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
