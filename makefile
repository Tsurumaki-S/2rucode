.SUFFIXES: .cpp .o

-include MAKE/objfiles

.cpp.o:
	g++ -c $< -o $@

-include MAKE/headerdepend
