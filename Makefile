.PHONY: clean_rebuild
clean_rebuild:
	rm -rf build/*
	cmake -B build
	cmake --build build
	cmake --install build --prefix=./install/

.PHONY: docker_rebuild
clean_rebuild:
	rm -rf build/*
	cmake -B build
	cmake --build build
	cmake --install build --prefix=./docker-install/
