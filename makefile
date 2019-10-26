#Compiler and Linker
CC          := mpic++

#The Target Binary Program
TARGET      := knn
#src and inc subfolders
MODULES		:= 
			   
#The Directories, Source, Includes, Objects, Binary
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
INC         := $(addprefix -I $(INCDIR)/,$(MODULES))		\
			   -I $(INCDIR) 								\
			   -I /usr/local/include 
LIB         := -flto
CXXFLAGS    := -std=c++17 -Wall -Wextra -pedantic $(INC) -Wno-unused-parameter \
			   -fdiagnostics-color -Ofast  -flto 
			   			   
SOURCES_TARGET := $(shell find $(SRCDIR)  -type f -name *.$(SRCEXT))
SOURCES_COMMON := $(filter-out src/main.cpp,$(SOURCES_TARGET))

OBJECTS_TARGET := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES_TARGET:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: directories ${TARGET}
#Make the Directories
directories: 
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)
	
run: all
	./${TARGETDIR}/${TARGET}

clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

$(TARGET): $(OBJECTS_TARGET)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)
	
#Compile target
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)x
	$(CC) $(CXXFLAGS) -c -o $@ $<

#Non-File Targets
.PHONY: all clean cleaner run
