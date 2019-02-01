# directories
OBJDIR := ./obj
INCDIR := ./include
SRCDIR := ./src
BINDIR := ./bin

# name of executable
TARGET := werhauz

# specify source,object,header files
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c =$(OBJDIR)/%.o)
INCLUDES := $(wildcard $(INCDIR)/*.h)

CC       := gcc
CFLAGS   := -O3 -g -I $(INCDIR) # -O3 enables further optimization

# all
all: $(BINDIR)/$(TARGET)


# create executable [by linking object files ]
$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)


# create object files
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# cleanup object files from previous compilations
.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(BINDIR)/$(TARGET)
	echo "Cleanup complete!"

# count
.PHONY: count
count:
	wc $(SOURCES) $(INCLUDES)
