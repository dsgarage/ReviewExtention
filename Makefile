.PHONY: preprocess pdf pdf-nopre clean help
PRE_OUT := .out
PRE_CLI := node packages/review-macro-shims/bin/review-preprocess.js
BOOKCFG := config.yml
TEST_DIR := test-project

help:
	@echo "ReviewExtention Build Targets:"
	@echo "  make preprocess  - Run JS preprocessor only"
	@echo "  make pdf        - Full pipeline: JS → Ruby → PDF"
	@echo "  make pdf-nopre  - Ruby only (for debugging)"
	@echo "  make test-pdf   - Build test project PDF"
	@echo "  make clean      - Remove generated files"

preprocess:
	$(PRE_CLI) "articles/**/*.re" -o $(PRE_OUT) --stats

pdf: preprocess
	BUNDLE_GEMFILE=Gemfile bundle exec review-pdfmaker -c $(BOOKCFG)

pdf-nopre:
	BUNDLE_GEMFILE=Gemfile bundle exec review-pdfmaker -c $(BOOKCFG)

test-pdf:
	cd $(TEST_DIR) && review-pdfmaker config.yml

clean:
	rm -rf $(PRE_OUT) book-pdf book.pdf
	cd $(TEST_DIR) && rm -rf book-pdf book.pdf *.tex *.log *.aux