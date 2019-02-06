# directories
OBJDIR   := ./obj
INCDIR   := ./include
SRCDIR   := ./src
BUILDDIR := ./build

# name of executable
TARGET   := werhauz

# specify source,object,header files
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
INCLUDES := $(wildcard $(INCDIR)/*.h)

CC       := gcc
CFLAGS   := -g -I$(INCDIR)


all:clean $(BUILDDIR)/$(TARGET)

valgrind:clean $(BUILDDIR)/$(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(BUILDDIR)/$(TARGET) -h1 10 -h2 10 -s 1024 -o ./input_files/input11.txt -c ./input_files/config_file.txt

run:clean $(BUILDDIR)/$(TARGET)
	$(BUILDDIR)/$(TARGET) -h1 10 -h2 10 -s 1024 -o ./input_files/input11.txt -c ./input_files/config_file.txt

# create executable [by linking object files]
# $(BUILDDIR) is an order-only-prerequisite
$(BUILDDIR)/$(TARGET): $(OBJECTS)  | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(BUILDDIR):
	mkdir $@

# create object files
# $(OBJDIR) is an order-only-prerequisite
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $@

# cleanup binary files
.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(BUILDDIR)/$(TARGET)
	@echo "Cleanup complete!"

# count
.PHONY: count
count:
	wc $(SOURCES) $(INCLUDES)
