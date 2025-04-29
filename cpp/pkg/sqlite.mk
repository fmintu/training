SQLITE_DIR := .build/pkg/sqlite
SQLITE_URL := https://www.sqlite.org/2024/sqlite-amalgamation-3450200.zip
SQLITE_ZIP := $(SQLITE_DIR)/sqlite.zip
SQLITE_SRC := $(SQLITE_DIR)/sqlite3.c
SQLITE_HDR := $(SQLITE_DIR)/sqlite3.h
SQLITE_OBJ := $(SQLITE_DIR)/sqlite3.o
SQLITE_LIB := $(SQLITE_DIR)/libsqlite.a

CFLAGS_SQLITE := -O2 -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION=1

.PHONY: sqlite clean_sqlite

pkg_sqlite: $(SQLITE_LIB)

# Download and extract
$(SQLITE_SRC) $(SQLITE_HDR): $(SQLITE_ZIP)
	unzip -o $(SQLITE_ZIP) -d $(SQLITE_DIR)
	mv -f $(SQLITE_DIR)/sqlite-amalgamation-*/sqlite3.* $(SQLITE_DIR)
	rm -rf $(SQLITE_DIR)/sqlite-amalgamation-*

$(SQLITE_ZIP):
	mkdir -p $(SQLITE_DIR)
	curl -L $(SQLITE_URL) -o $(SQLITE_ZIP)

# Compile .c using gcc not g++
$(SQLITE_OBJ): $(SQLITE_SRC)
	@mkdir -p $(dir $@)
	gcc -c $(CFLAGS_SQLITE) $< -o $@

$(SQLITE_LIB): $(SQLITE_OBJ)
	ar rcs $@ $^

clean_sqlite:
	rm -rf $(SQLITE_DIR)