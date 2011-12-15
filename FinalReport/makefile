all: GDP18.pdf

CLEAN :=

GDP18.pdf: GDP18.tex approaches_chosen.tex approaches_chosen_camera.tex approaches_chosen_jpeg_manipulation.tex approaches_chosen_physical_implementation.tex approaches_considered.tex approaches_considered_camera.tex approaches_considered_jpeg_manipulation.tex approaches_considered_payload.tex bibliography.tex conclusions.tex docs.tex evaluation.tex future.tex glossary.tex implementation.tex implementation_camera.tex implementation_payload_controller.tex implementation_progressive_jpeg.tex introduction.tex management.tex management_risk.tex overview.tex research.tex research_custom_compression.tex research_jpeg_manipulation.tex specification.tex testing.tex testing_camera.tex
	pdflatex GDP18.tex
	pdflatex GDP18.tex

clean:
	rm -rf *.aux *.pdf *.dvi *.log *.lof *.lol *.lot *.out *.toc
	rm -rf $(CLEAN)
