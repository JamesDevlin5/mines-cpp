FONT_URL=https://github.com/ryanoasis/nerd-fonts/releases/download/v2.2.2/Hack.zip

CXX=g++ -std=c++17 -g
FLAGS=-Wall -Wextra -Werror -pedantic -g
SRC_DIR=src
INC_DIR=include
EXE_FILE=prog
CXX_LIBS=-lsfml-graphics -lsfml-window -lsfml-system

$(EXE_FILE): font.ttf
	$(CXX) $(SRC_DIR)/*.cpp -o $@ -I$(INC_DIR) $(CXX_LIBS)

font.ttf:
	@echo "Downloading font ($(FONT_URL))..."
	@curl -fsSLo "font.zip" $(FONT_URL)
	@unzip -q "font.zip" "Hack Regular Nerd Font Complete.ttf"
	@mv "Hack Regular Nerd Font Complete.ttf" "$@"
	@$(RM) "font.zip"

clean:
	@$(RM) -v $(EXE_FILE)

help:
	@echo "Usage: make {$(EXE_FILE)|clean|help}" 1>&2 && false

.PHONY: prog clean help
