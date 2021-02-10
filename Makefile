include Makefile.compilation

ccsrc = $(wildcard src/ast/*.cc) \
        $(wildcard src/visitor/*.cc) \
        $(wildcard src/visitor/printer/*.cc) \
        $(wildcard src/visitor/semantic/*.cc) \
        $(wildcard src/visitor/codegen/*.cc) \
        $(wildcard src/parser-scanner/*.cc) \
        $(wildcard src/*.cc)
obj = $(ccsrc:.cc=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

vsopc: $(obj)
	@(cd src/parser-scanner && $(MAKE))
	$(CC) -g -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.cc
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: install-tools
install-tools:;

.PHONY: fullclean
fullclean:
	$(MAKE) clean
	$(MAKE) cleandep
	rm -rf vsopcompiler vsopcompiler.tar.xz

.PHONY: clean
clean:
	rm -f $(obj) $(EXEC)

.PHONY: cleandep
cleandep:
	rm -f $(dep)

archive:
	$(MAKE) fullclean
	mkdir -p $(DIR)
	cp -r src $(DIR)
	cp Makefile Makefile.compilation $(DIR)
	mkdir -p vsopcompiler/tests
	cp tests/*.vsop vsopcompiler/tests/
	cp build_exec.sh $(DIR)
	cp report.pdf $(DIR)
	tar -cJvf vsopcompiler.tar.xz vsopcompiler

docker:
	make archive
	docker cp vsopcompiler 979449cbaf19:home/vagrant/
