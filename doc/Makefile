
LATEX = latex
DVI2PDF = dvipdfm

all: SKM2_Lab_InitDoc.pdf

SKM2_Lab_InitDoc.pdf: SKM2_Lab_InitDoc.dvi
	$(DVI2PDF) $?

SKM2_Lab_InitDoc.dvi: SKM2_Lab_InitDoc.tex
	$(LATEX) $?

clean:
	rm -f SKM2_Lab_InitDoc.{dvi,aux,log}

