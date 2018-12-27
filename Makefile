CFLAGS := -g -Wall -Wextra -Wpedantic -Ibuild/include $(shell pkg-config --cflags glfw3 glew)
LDFLAGS := ${LDFLAGS} $(shell pkg-config --libs glew glfw3)

src = $(shell find src/ -name *.c)
headers = $(shell find src/ -name *.h)
obj = $(shell echo $(src) | sed 's/src/build/g; s/\.c/\.o/g')
build_dirs = $(shell find src/ -type d | sed 's/src/build/g')
resources = $(shell ./build_scripts/get_resource_headers.sh)


neighbors: build build/neighbors

build:
	mkdir $(build_dirs)

$(obj): build/%.o: src/%.c $(headers) $(resources)
	$(CC) -c -o $@ $< $(CFLAGS)

build/neighbors: $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(resources): build/include/%.h: %
	./build_scripts/compile_resources.sh $<

.PHONY: clean

clean:
	rm -r build
