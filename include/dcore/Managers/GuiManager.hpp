#pragma once

class GuiManager {
public:
	void BeginWindow(const char *title = nullptr);
	void BeginListChooser();
	bool Selectable(const char *name, bool isAlreadySelected);
	bool Button(const char *name);
	void EndListChooser();
	void BeginBottomBar();
	void EndBottomBar();
	void TextTitle(const char *text);
	void LongText(const char *text);
	void EndWindow();
};
