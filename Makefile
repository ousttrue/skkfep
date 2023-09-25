#
# Makefile for skkfep
#
all: genMakefile
	make -f genMakefile all

skkfep: genMakefile
	make -f genMakefile skkfep

simpledic: genMakefile
	make -f genMakefile simpledic

clean: genMakefile
	make -f genMakefile clean
	rm -f genMakefile

genMakefile: protoMakefile config.h
	awk -f extrpar `grep 'configs/.*\.h' config.h | sed 's/.*\(configs\/.*\.h\).*/\1/'` > genMakefile
	awk -f extrpar config.h | cat - protoMakefile >> genMakefile
