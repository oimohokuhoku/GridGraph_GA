SOURCES = main.cpp
SOURCES += ga/group.cpp
SOURCES += ga/individual.cpp
SOURCES += ga/evaluate/adj_aspl.cpp
SOURCES += ga/select/mgg.cpp
SOURCES += ga/mutate/rotate.cpp
SOURCES += ga/mutate/two_opt.cpp
SOURCES += ga/mutate/mutater_base.cpp
SOURCES += ga/crossover/i_crosser.cpp
SOURCES += ga/crossover/twx.cpp
SOURCES += ga/crossover/resx.cpp
SOURCES += ga/crossover/dex.cpp
SOURCES += ga/crossover/uniform_cross.cpp
SOURCES += ga/grid_graph_base.cpp
SOURCES += ga/other/randomizer.cpp
SOURCES += ga/other/local_search.cpp
SOURCES += ga/other/meta_observer.cpp
SOURCES += io/edges_file_writer.cpp
SOURCES += io/edges_file_reader.cpp
SOURCES += other/int_queue.cpp
SOURCES += other/common.cpp
SOURCES += other/command_line_argument.cpp
SOURCES += test/unit_test.cpp

COMPILE_OPTION = -g -march=native

OBJECTS_DIR = ./object_file/
OBJECTS_FILE = $(notdir $(SOURCES:.cpp=.o))
OBJECTS_PATH = $(addprefix $(OBJECTS_DIR), $(OBJECTS_FILE))

debug: $(OBJECTS_PATH)
	g++ -O0 -march=native -o grid $^

release: $(SOURCES)
	g++ -O3 -march=native -o grid $^

$(word 1, $(OBJECTS_PATH)): $(word 1, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 2, $(OBJECTS_PATH)): $(word 2, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<
	
$(word 3, $(OBJECTS_PATH)): $(word 3, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 4, $(OBJECTS_PATH)): $(word 4, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 5, $(OBJECTS_PATH)): $(word 5, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 6, $(OBJECTS_PATH)): $(word 6, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 7, $(OBJECTS_PATH)): $(word 7, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 8, $(OBJECTS_PATH)): $(word 8, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 9, $(OBJECTS_PATH)): $(word 9, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 10, $(OBJECTS_PATH)): $(word 10, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 11, $(OBJECTS_PATH)): $(word 11, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 12, $(OBJECTS_PATH)): $(word 12, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 13, $(OBJECTS_PATH)): $(word 13, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 14, $(OBJECTS_PATH)): $(word 14, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 15, $(OBJECTS_PATH)): $(word 15, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 16, $(OBJECTS_PATH)): $(word 16, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 17, $(OBJECTS_PATH)): $(word 17, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 18, $(OBJECTS_PATH)): $(word 18, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<
	
$(word 19, $(OBJECTS_PATH)): $(word 19, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 20, $(OBJECTS_PATH)): $(word 20, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 21, $(OBJECTS_PATH)): $(word 21, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 22, $(OBJECTS_PATH)): $(word 22, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

$(word 23, $(OBJECTS_PATH)): $(word 23, $(SOURCES))
	g++ $(COMPILE_OPTION) -o $@ -c $<

.PHONY: clean
clean: $(OBJECTS_PATH)
	$(RM) $(OBJECTS_DIR)/*.o