# pkg/json.mk

JSON_BUILD_DIR := .build/pkg/json
JSON_HEADER := $(JSON_BUILD_DIR)/json.hpp
JSON_URL := https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

JSON_INCLUDE := -I.build/pkg/json

 : json

json: $(JSON_HEADER)

$(JSON_HEADER):
	@mkdir -p $(JSON_BUILD_DIR)
	@if [ ! -f $(JSON_HEADER) ]; then \
		echo "Downloading nlohmann/json.hpp into build/pkg/json..."; \
		curl -L $(JSON_URL) -o $(JSON_HEADER); \
	else \
		echo "json.hpp already exists in build/pkg/json."; \
	fi