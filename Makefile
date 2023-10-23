SOURCES = main.cpp
SOURCES += ga/ga.cpp
SOURCES += ga/parameter.cpp
SOURCES += ga/group.cpp
SOURCES += ga/individual.cpp
SOURCES += ga/evaluate/adj_aspl.cpp
SOURCES += ga/select/copy_selector.cpp
SOURCES += ga/select/survive_selector.cpp
SOURCES += ga/select/elitist_recombination.cpp
SOURCES += ga/select/mgg.cpp
SOURCES += ga/mutate/two_opt.cpp
SOURCES += ga/mutate/mutater_base.cpp
SOURCES += ga/crossover/i_crosser.cpp
SOURCES += ga/crossover/twx.cpp
SOURCES += ga/crossover/resx.cpp
SOURCES += ga/crossover/gx.cpp
SOURCES += ga/crossover/dmsx.cpp
SOURCES += ga/grid_graph_base.cpp
SOURCES += ga/other/randomizer.cpp
SOURCES += ga/other/local_search.cpp
SOURCES += ga/other/meta_observer.cpp
SOURCES += ga/other/init_graph_generator.cpp
SOURCES += io/edges_file_writer.cpp
SOURCES += io/edges_file_reader.cpp
SOURCES += other/int_queue.cpp
SOURCES += other/command_line_argument.cpp
SOURCES += other/directory.cpp
SOURCES += other/random.cpp
SOURCES += other/console.cpp
SOURCES += test/unit_test.cpp


COMPILE_OPTION = -g -march=native
INCLUDE_ROOT = -I ./


OBJECTS_DIR = ./object_file/
OBJECTS_FILE = $(notdir $(SOURCES:.cpp=.o))
OBJECTS_PATH = $(addprefix $(OBJECTS_DIR), $(OBJECTS_FILE))

debug: $(OBJECTS_PATH) $(LIBRARY)
	g++ -O0 -march=native -o grid $^

release: $(SOURCES) $(LIBRARY)
	g++ -O3 $(INCLUDE_ROOT) -march=native -o grid $^

$(word 1, $(OBJECTS_PATH)): $(word 1, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 2, $(OBJECTS_PATH)): $(word 2, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<
	
$(word 3, $(OBJECTS_PATH)): $(word 3, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 4, $(OBJECTS_PATH)): $(word 4, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 5, $(OBJECTS_PATH)): $(word 5, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 6, $(OBJECTS_PATH)): $(word 6, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 7, $(OBJECTS_PATH)): $(word 7, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 8, $(OBJECTS_PATH)): $(word 8, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 9, $(OBJECTS_PATH)): $(word 9, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 10, $(OBJECTS_PATH)): $(word 10, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 11, $(OBJECTS_PATH)): $(word 11, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 12, $(OBJECTS_PATH)): $(word 12, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 13, $(OBJECTS_PATH)): $(word 13, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 14, $(OBJECTS_PATH)): $(word 14, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 15, $(OBJECTS_PATH)): $(word 15, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 16, $(OBJECTS_PATH)): $(word 16, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 17, $(OBJECTS_PATH)): $(word 17, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 18, $(OBJECTS_PATH)): $(word 18, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<
	
$(word 19, $(OBJECTS_PATH)): $(word 19, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 20, $(OBJECTS_PATH)): $(word 20, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 21, $(OBJECTS_PATH)): $(word 21, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 22, $(OBJECTS_PATH)): $(word 22, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 23, $(OBJECTS_PATH)): $(word 23, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 24, $(OBJECTS_PATH)): $(word 24, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 25, $(OBJECTS_PATH)): $(word 25, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 26, $(OBJECTS_PATH)): $(word 26, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 27, $(OBJECTS_PATH)): $(word 27, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 28, $(OBJECTS_PATH)): $(word 28, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 29, $(OBJECTS_PATH)): $(word 29, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<

$(word 30, $(OBJECTS_PATH)): $(word 30, $(SOURCES))
	g++ $(COMPILE_OPTION) $(INCLUDE_ROOT) -o $@ -c $<


.PHONY: clean
clean: $(OBJECTS_PATH)
	$(RM) $(OBJECTS_DIR)/*.o