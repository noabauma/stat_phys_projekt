OS:=$(shell uname)

ifeq ($(OS), Darwin)
	VIEW:=open
else
	VIEW:=xdg-open
endif

presentation.pdf: presentation.tex 1.tex 2.tex 3.tex start.tex
	pdflatex presentation.tex
	pdflatex presentation.tex
	$(VIEW) presentation.pdf

clean:
	rm -rf *.aux *.log *.pdf *.out *.toc *.lof *.nav *.snm
