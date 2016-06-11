#pragma once
#ifndef _CAMERA_H_INCLUDED_
#define _CAMERA_H_INCLUDED_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <direct.h>
#include "msclr\marshal_cppstd.h"

using namespace System;
using namespace System::IO;

using namespace std;

class Cam {
public:
	static int file_num;
	static int label_mass_pixel;
	static string image_root;
	static string auto_file;
	static string auto_root;
	static string combo_text1;
	static string combo_text2;
	static string combo_text3;
	static string write_path;
	const char *write_path_c;
	FILE* file_generate;
	string GetFileName(const string &path);
	Cam() {}
	Cam(String^ Path);
	void FixAngle();
	void Average();
	void OutlineDifference();
	void InsideDifference();
	void MakeSigmaImage();
	void OutofSigma();

	void SetCombo(String^ combo_string, int mode);

	void StartTimer();
	void StopTimer();
	void SetAuto(String^ auto_);
};

#endif