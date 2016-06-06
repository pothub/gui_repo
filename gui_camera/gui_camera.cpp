// gui_camera.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"

using namespace gui_camera;

[STAThreadAttribute]	//use folder dialog
int main() {
	ImageProcessingForm ^fm = gcnew ImageProcessingForm();
	fm->ShowDialog();
	return 0;
}
