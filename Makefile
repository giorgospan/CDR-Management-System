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

# create everything
all:clean $(BUILDDIR)/$(TARGET)

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
