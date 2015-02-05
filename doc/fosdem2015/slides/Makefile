all : fwatch_fosdem2015.pdf

.PHONY : all clean

fwatch_fosdem2015.pdf : fwatch_fosdem2015.tex
#	pdflatex $^
#	pdflatex $^

#	latex $^
# 	bibtex
	latex $^
	latex $^
	dvips -j0 fwatch_fosdem2015
	ps2pdf  -dPDFX -dEmbedAllFonts=true -dSubsetFonts=true -dEPSCrop=true fwatch_fosdem2015.ps


clean :
	rm -f *.ps *.pdf *.dat *.log *.out *.aux *.dvi *~ *.nav *.snm *.vrb *.toc

show : fwatch_fosdem2015.pdf
	evince $^ &