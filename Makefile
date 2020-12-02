CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = vm
SRCDIR = .
OBJDIR = objs
BINDIR = bin
SOURCES := $(wildcard $(SRCDIR)/*.cc)
OBJECTS := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SOURCES))
DEPENDS = ${OBJECTS:.o=.d}

${BINDIR}/${EXEC}: ${OBJECTS} | ${BINDIR}
	${CXX} ${OBJECTS} -o ${BINDIR}/${EXEC}

$(OBJDIR)/%.o: $(SRCDIR)/%.cc | ${OBJDIR}
	$(CXX) $(CXXFLAGS) -c -o $@ $<

${BINDIR}:
	mkdir $@

${OBJDIR}:
	mkdir $@

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -r ${OBJDIR} ${BINDIR} 
