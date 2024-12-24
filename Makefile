serve:
	docker run -it --rm -p 1111:1111 -w /app -v $(PWD):/app zola serve -i 0.0.0.0

build:
	docker run -it --rm -p 1111:1111 -w /app -v $(PWD):/app zola build

deploy:
	ssh romst.com rm -rfv public
	rsync -av public/* romst.com:nginx/webroot/html/

.PHONY: serve build deploy
