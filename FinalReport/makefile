all: GDP18.pdf

CLEAN :=

GDP18.pdf: GDP18.tex approaches_chosen.tex approaches_considered.tex bibliography.tex conclusions.tex docs.tex evaluation.tex future.tex glossary.tex implementation.tex introduction.tex management.tex overview.tex research.tex specification.tex testing.tex implementation_payload_controller.tex approaches_considered_payload.tex
	pdflatex GDP18.tex
	pdflatex GDP18.tex

clean:
	rm -rf *.aux *.pdf *.dvi *.log *.lof *.lol *.lot *.out *.toc
	-rm -rf $(CLEAN)
