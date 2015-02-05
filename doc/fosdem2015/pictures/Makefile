SVG=$(wildcard *.svg)
PNG=$(wildcard *.png)
JPG=$(wildcard *.jpg)
PDF=$(SVG:.svg=.pdf)
EPS=$(SVG:.svg=.eps) $(PNG:.png=.eps) $(JPG:.jpg=.eps)


.PHONY: all
all: $(PDF) $(EPS)

%.pdf : %.svg
	inkscape --without-gui $< -A $@

%.eps : %.svg
	inkscape --without-gui $< -E $@

%.eps : %.png
	inkscape --without-gui $< -E $@

%.eps : %.jpg
	inkscape --without-gui $< -E $@

clean:
	rm -f $(PDF)
	rm -f $(EPS)

show:
	$(info all svg: $(SVG))
	$(info all pdf: $(PDF))
	$(info all eps: $(EPS))
	$(info all jpg: $(JPG))
	$(info all png: $(PNG))



