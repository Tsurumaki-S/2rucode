.SUFFIXES: .cpp .o

-include MAKE/objfiles_mpi

.cpp.o:
	mpicxx -c $< -o $@

-include MAKE/headerdepend_mpi
