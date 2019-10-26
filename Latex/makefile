.RECIPEPREFIX +=
filename := sprawozdanie
trash_extensions := toc stderr sh out log aux
trash_files := $(foreach extension, $(filename), $(addprefix $(extension)., $(trash_extensions))) inputs.tex

all: pdf clean

pdf:
    pdflatex -shell-escape ${filename}.tex
    pdflatex -shell-escape ${filename}.tex

clean :
    rm -f $(trash_files)

cleaner :
    rm -f $(filename).pdf

