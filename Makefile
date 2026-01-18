# Development server - disables integrity and sets localhost base URL
serve:
	@sed -i 's/^integrity = true/integrity = false/' config.toml 2>/dev/null || true
	@echo ">>> Access at: http://127.0.0.1:1111"
	docker run -it --rm -p 1111:1111 -p 1024:1024 -w /app -v $(PWD):/app zola serve -i 0.0.0.0 --base-url http://127.0.0.1
	@sed -i 's/^integrity = false/integrity = true/' config.toml 2>/dev/null || true

# Production build - ensures integrity is enabled
build:
	@sed -i 's/^integrity = false/integrity = true/' config.toml 2>/dev/null || true
	docker run -it --rm -p 1111:1111 -w /app -v $(PWD):/app zola build

deploy: build
	rsync -av public romst.com:

.PHONY: serve build deploy
