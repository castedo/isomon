all: dist dist/index.html dist/resources/boostbook.css dist/isomon_currency.hpp dist/isomon_money.hpp

dist:
	rm -rf dist
	mkdir -p dist

dist/index.html : dist xsl/chunked.xsl build/isomon.xml
	xsltproc --xinclude -o dist/ xsl/chunked.xsl build/isomon.xml

dist/resources/%.css : dist resources/%.css
	rm -rf dist/resources
	cp -r resources dist

build/isomon.xml : isomon.txt build/reference.xml
	asciidoc -b docbook -o build/isomon.xml isomon.txt

build/reference.xml : build/boostbook.xml
	xsltproc xsl/boostbook/docbook.xsl build/boostbook.xml > build/reference.xml

build/boostbook.xml : build/xml/all.xml
	xsltproc xsl/doxygen2boostbook.xsl build/xml/all.xml > build/boostbook.xml

build/xml/all.xml : build/xml
	xsltproc build/xml/combine.xslt build/xml/index.xml > build/xml/all.xml

build/xml : reference.dox ../currency.hpp ../currency_data.h
	rm -rf build/xml
	doxygen reference.dox


dist/isomon_currency.hpp : include_definitions.h ../iso_table_data.h ../currency_data.h ../currency.hpp
	python gen_unified_header.py $^ > $@

dist/isomon_money.hpp : include_definitions.h ../iso_table_data.h ../currency_data.h ../currency.hpp ../money.hpp ../money_calc.hpp
	python gen_unified_header.py $^ > $@

.PHONY : clean all

clean :
	rm -rf build
	rm -rf dist
