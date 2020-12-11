CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = vm
OBJECTS = actions/action.o actions/add.o actions/backspace.o actions/buffer.o \
	actions/e-movement.o actions/e-search.o actions/file-op.o \
	actions/global.o actions/incomplete.o actions/insert.o \
	actions/keyboard.o actions/movement.o actions/replace.o actions/scroll.o \
	actions/search.o actions/text-edit.o controllers/controller-base.o \
	controllers/input.o controllers/key-input.o exceptions/bad-entry.o \
	exceptions/display-message.o exceptions/display-warning.o \
	models/clipboard.o models/file.o models/model-base.o \
	models/move.o models/register.o models/row.o models/text-model.o \
	models/text-search.o \
	models/text.o models/undo.o ui/window.o ui/graphics.o views/status-view.o \
	views/text-view.o views/view-base.o vm.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lncurses 

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}
