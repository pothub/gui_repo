// gui_camera.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"

using namespace gui_camera;

[STAThreadAttribute]	//use folder dialog
int main() {
	ImageProcessingForm ^fm = gcnew ImageProcessingForm();
	fm->ShowDialog();
	return 0;
}
