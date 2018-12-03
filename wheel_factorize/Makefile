all: build

build: setup.py wheel_factorize
	python setup.py sdist bdist_wheel

push: dist
	twine upload dist/*

push-test: dist
	twine upload --repository-url https://test.pypi.org/legacy/ dist/*

clean:
	rm -rf build dist *.egg-info wheel_factorize/*.pyc

.phony: build push clean
