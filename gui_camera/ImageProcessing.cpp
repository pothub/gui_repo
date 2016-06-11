#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib")	//for timer interrupt
#include <mmsystem.h>				//for timer interrupt
#include "Labeling.h"				//for labeling

using namespace std;

#define MAX_IMAGE_NUM 200
#define LABEL_ARRAY_NUM	1024

#define width	640
#define hight	480

string file_path[MAX_IMAGE_NUM];
string filename[MAX_IMAGE_NUM];
string Cam::image_root;
string Cam::auto_file;
string Cam::auto_root;
string Cam::combo_text1;
string Cam::combo_text2;
string Cam::combo_text3;
string Cam::write_path;
int Cam::file_num = 0;
int Cam::label_mass_pixel = 2;

cv::Mat moved_img[MAX_IMAGE_NUM];
cv::Mat ave_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
__int16 R_min = ave_img.rows, R_max = 0, C_min = ave_img.cols, C_max = 0;

string Cam::GetFileName(const string &path) {
	size_t pos1 = path.rfind('\\');
	if (pos1 != string::npos) return path.substr(pos1 + 1, path.size() - pos1 - 1);

	pos1 = path.rfind('/');
	if (pos1 != string::npos) return path.substr(pos1 + 1, path.size() - pos1 - 1);

	return path;
}


Cam::Cam(String^ Path) {
	cli::array<String^>^ file = Directory::GetFiles(Path);
	file_num = file->Length;
	for (int i = 0; i < file_num; i++) {
		String^ file_ = file[i];
		file_path[i] = msclr::interop::marshal_as<std::string>(file_);
		filename[i] = GetFileName(file_path[i]);
		//cout << file_path[i] << "\n";
	}
	image_root = msclr::interop::marshal_as<std::string>(Path);
	image_root.erase(image_root.rfind("\\"));
	cout << image_root << "\n";
}

void Cam::SetCombo(String^ combo_string, int mode) {
	if (mode == 1)
		combo_text1 = msclr::interop::marshal_as<std::string>(combo_string);
	else if (mode == 2)
		combo_text2 = msclr::interop::marshal_as<std::string>(combo_string);
	else if (mode == 3)
		combo_text3 = msclr::interop::marshal_as<std::string>(combo_string);
}

void Cam::FixAngle() {
	write_path = image_root + "\\fix_angle_countGray.txt";
	write_path_c = write_path.c_str();
	file_generate = freopen(write_path_c, "w", stdout);
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		//cout << file_path[i] << "\n";
		//read as color
		cv::Mat src_img = cv::imread(file_path[terget_num], 1);
		cv::Mat gray_img, median_img, bin_img;

		//Get rid of the noise
		cv::cvtColor(src_img, gray_img, CV_RGB2GRAY);
		int median_level = stoi(combo_text1);	//string to int
		cv::medianBlur(gray_img, median_img, median_level);
		if (combo_text2 == "Auto")
			cv::threshold(median_img, bin_img, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		else {
			int threshold_level = stoi(combo_text2);
			cv::threshold(median_img, bin_img, threshold_level, 255, cv::THRESH_BINARY);
		}
		bitwise_and(gray_img, bin_img, gray_img);

		//To calculate the center of gravity
		cv::Moments moment = cv::moments(bin_img, true);
		int gravityX = (int)(moment.m10 / moment.m00);
		int gravityY = (int)(moment.m01 / moment.m00);

		//To calculate the workpiece angle
		double ang = 0.5*atan2(2.0*moment.mu11, moment.mu20 - moment.mu02) * 180 / M_PI;

		//cout << write_path << "\n";
		//circle(bin_img, cv::Point(gravityX, gravityY), 20, cv::Scalar(100, 100, 0), 2, 2);
		//circle(bin_img, cv::Point(maxX, maxY), 20, cv::Scalar(100, 100, 0), 2, 2);
		//write_path = image_root + "\\check_angle";
		//_mkdir(write_path.c_str());
		//write_path += "\\" + filename[terget_num];
		//cv::imwrite(write_path, bin_img);

		//Parallel movement of the image
		cv::Point2f center(src_img.cols*0.5, src_img.rows*0.5);
		cv::Mat affine_matrix = cv::getRotationMatrix2D(center, 0, 1);
		affine_matrix.at<double>(0, 2) += 320 - gravityX;
		affine_matrix.at<double>(1, 2) += 240 - gravityY;
		cv::warpAffine(gray_img, moved_img[terget_num], affine_matrix, median_img.size());

		//Rotational movement of the image
		affine_matrix = cv::getRotationMatrix2D(center, ang, 1);
		cv::warpAffine(moved_img[terget_num], moved_img[terget_num], affine_matrix, median_img.size());

		write_path = image_root + "\\fix_angle";
		_mkdir(write_path.c_str());
		write_path += "\\" + filename[terget_num];
		cv::imwrite(write_path, moved_img[terget_num]);

		int countGray = 0;
		for (int R = 0; R < src_img.rows; ++R) {
			for (int C = 0; C < src_img.cols; ++C) {
				countGray += moved_img[terget_num].data[R*src_img.cols + C*src_img.elemSize()];
			}
		}
		cout << terget_num << "\t" << countGray << "\n";
	}
	fclose(file_generate);
	cerr << "end of FixAngle\n";
}

void Cam::Average() {
	__int16 ave_tmp[640][480] = { 0 };
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		for (int y = 0; y < moved_img[terget_num].rows; ++y) {
			for (int x = 0; x < moved_img[terget_num].cols; ++x) {
				ave_tmp[x][y] += (int)moved_img[terget_num].data[y * moved_img[terget_num].step + x];
			}
		}
	}

	//set of computational domain
	for (int y = 0; y < ave_img.rows; ++y) {
		for (int x = 0; x < ave_img.cols; ++x) {
			ave_tmp[x][y] /= file_num;
			ave_img.data[y * ave_img.step + x] = ave_tmp[x][y];
			if (ave_tmp[x][y] > 0) {
				if ((x < ave_img.cols / 2) && (x < C_min)) C_min = x;
				if ((x >= ave_img.cols / 2) && (x > C_max)) C_max = x;
				if ((y < ave_img.rows / 2) && (y < R_min)) R_min = y;
				if ((y >= ave_img.rows / 2) && (y > R_max)) R_max = y;
			}
		}
	}

	write_path = image_root + "\\average.bmp";
	cv::imwrite(write_path, ave_img);
	//cerr << ":" << C_min << ":" << C_max << ":" << R_min << ":" << R_max;
	cerr << "end of Average\n";
}

void Cam::OutlineDifference() {
	cv::Mat ave_th, moved_th, outline_diff_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
	cv::threshold(ave_img, ave_th, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	write_path = image_root + "\\outline_diff";
	_mkdir(write_path.c_str());
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		cv::threshold(moved_img[terget_num], moved_th, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		outline_diff_img = moved_th - ave_th;
		outline_diff_img += ave_th - moved_th;

		write_path = image_root + "\\outline_diff\\" + filename[terget_num];
		cv::imwrite(write_path, outline_diff_img);
	}
	cerr << "end of OutlineDifference\n";
}

void Cam::InsideDifference() {
	cv::Mat ave_th, moved_th;
	cv::Mat ave_and_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
	cv::Mat moved_and_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
	cv::Mat inside_diff_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
	cv::threshold(ave_img, ave_th, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	write_path = image_root + "\\inside_diff";
	_mkdir(write_path.c_str());
	write_path = image_root + "\\inside_diff_countGray.txt";
	write_path_c = write_path.c_str();
	file_generate = freopen(write_path_c, "w", stdout);
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		cv::threshold(moved_img[terget_num], moved_th, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		bitwise_and(moved_img[terget_num], ave_th, moved_and_img);
		bitwise_and(moved_th, ave_img, ave_and_img);
		inside_diff_img = moved_and_img - ave_and_img;
		inside_diff_img += ave_and_img - moved_and_img;
		write_path = image_root + "\\inside_diff\\" + filename[terget_num];
		cv::imwrite(write_path, inside_diff_img);
		int countGray = 0;
		for (int R = 0; R < inside_diff_img.rows; ++R) {
			for (int C = 0; C < inside_diff_img.cols; ++C) {
				countGray += inside_diff_img.data[R*inside_diff_img.cols + C*inside_diff_img.elemSize()];
			}
		}
		cout << terget_num << "\t" << countGray << "\n";
	}
	fclose(file_generate);
	cerr << "end of InsedeDifference\n";	//output of console
}

void Cam::MakeSigmaImage() {
	write_path = image_root + "\\StandardDeviation_pixel.csv";
	write_path_c = write_path.c_str();
	file_generate = freopen(write_path_c, "w", stdout);
	unsigned __int16 *pix_ave = new unsigned __int16[ave_img.rows*ave_img.cols];
	unsigned __int32 *pix_dis = new unsigned __int32[ave_img.rows*ave_img.cols];

	//initialize pix_array
	for (int R = R_min; R < R_max; ++R) {
		for (int C = C_min; C < C_max; ++C) {
			pix_ave[R * 640 + C] = 0;
			pix_dis[R * 640 + C] = 0;
		}
	}
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		for (int R = R_min; R < R_max; ++R) {
			for (int C = C_min; C < C_max; ++C) {
				__int16 pixel_moved = static_cast<__int16>(moved_img[terget_num].data
					[R*moved_img[terget_num].cols + C*moved_img[terget_num].elemSize()]);
				pix_ave[R * 640 + C] += pixel_moved;
			}
		}
		cerr << "CalcStandardDeviation....." << terget_num << "\n";
	}
	for (int R = R_min; R < R_max; ++R) {
		for (int C = C_min; C < C_max; ++C) {
			pix_ave[R * 640 + C] /= file_num;
			cout << pix_ave[R * 640 + C] << ",";
		}
	}
	for (int terget_num = 0; terget_num < file_num - 1; terget_num++) {
		for (int R = R_min; R < R_max; ++R) {
			for (int C = C_min; C < C_max; ++C) {
				__int16 pixel_moved = static_cast<__int16>(moved_img[terget_num].data
					[R*moved_img[terget_num].cols + C*moved_img[terget_num].elemSize()]);
				__int16 tmp = pix_ave[R * 640 + C] - pixel_moved;
				//cerr << tmp << ",";
				pix_dis[R * 640 + C] += tmp*tmp;
			}
		}

		//cerr << "\n";
	}
	for (int R = R_min; R < R_max; ++R) {
		for (int C = C_min; C < C_max; ++C) {
			pix_dis[R * 640 + C] /= file_num;
			pix_dis[R * 640 + C] = sqrt(pix_dis[R * 640 + C]);
		}
	}
	fclose(file_generate);

	// make sigma_image
	write_path = image_root + "\\probability_distribution";
	_mkdir(write_path.c_str());

	for (int sigma = 0; sigma < 7; sigma++) {
		cv::Mat sigma_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
		for (int R = R_min; R < R_max; ++R) {
			for (int C = C_min; C < C_max; ++C) {
				__int16 pix = pix_ave[R * 640 + C] + (sigma - 3) * pix_dis[R * 640 + C];
				if (pix > 255) pix = 255;
				else if (pix < 0) pix = 0;
				sigma_img.data[R * 640 + C] = pix;
			}
		}
		char oss[32];
		sprintf(oss, "%dsigma.bmp", sigma - 3);
		write_path = image_root + "\\probability_distribution\\" + oss;
		cv::imwrite(write_path, sigma_img);
	}
	cerr << "end of CalcStandardDeviation\n";

}

cv::Mat Nsigma_img[7];
void Cam::OutofSigma() {
	//read sigma_image as gray
	for (int sigma = 0; sigma < 7; sigma++) {
		char oss[32];
		sprintf(oss, "%dsigma.bmp", sigma - 3);
		write_path = image_root + "\\probability_distribution\\" + oss;
		Nsigma_img[sigma] = cv::imread(write_path, 0);
	}

	//write_path = image_root + "\\out_sigma";
	//_mkdir(write_path.c_str());
	//for (int terget_num = 0; terget_num < file_num - 1; terget_num++){
	//	cv::Mat outlier_img = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
	//	for (int R = R_min; R < R_max; ++R){
	//		for (int C = C_min; C < C_max; ++C){
	//			if ((Nsigma_img[4].data[R * 640 + C]>moved_img[terget_num].data[R * 640 + C]) &&
	//				(Nsigma_img[2].data[R * 640 + C]<moved_img[terget_num].data[R * 640 + C])){
	//				outlier_img.data[R * 640 + C] = 0;
	//			}
	//			else if ((Nsigma_img[5].data[R * 640 + C]>moved_img[terget_num].data[R * 640 + C]) &&
	//				(Nsigma_img[1].data[R * 640 + C] < moved_img[terget_num].data[R * 640 + C])){
	//				outlier_img.data[R * 640 + C] = 0;
	//			}
	//			else if ((Nsigma_img[6].data[R * 640 + C]>moved_img[terget_num].data[R * 640 + C]) &&
	//				(Nsigma_img[0].data[R * 640 + C] < moved_img[terget_num].data[R * 640 + C])){
	//				outlier_img.data[R * 640 + C] = 255;
	//			}

	//		}
	//	}
	//	write_path = image_root + "\\out_sigma\\" + filename[terget_num];
	//	cv::imwrite(write_path, outlier_img);
	//}

	cerr << "end of OutofSigma\n";
}

MMRESULT timerID;
void CALLBACK timerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dummy1, DWORD dummy2) {

	Cam::write_path = Cam::auto_root + "\\auto_tmp";
	_mkdir(Cam::write_path.c_str());
	static int filenum_static = 0;

	cli::array<String^>^ file = Directory::GetFiles(msclr::interop::marshal_as<System::String^>(Cam::auto_file));
	Cam::file_num = file->Length;
	for (int i = 0; i < Cam::file_num; i++) {
		String^ file_ = file[i];
		file_path[i] = msclr::interop::marshal_as<std::string>(file_);
	}
	cv::Mat	gray_img, median_img, bin_img, moved_img, moved_img_c, labeled_c;
	cv::Mat	red_img(cv::Size(width, hight), CV_8UC3, cv::Scalar(0, 0, 0));
	for (int terget_num = 0; terget_num < Cam::file_num; terget_num++) {
		cv::Mat src_img = cv::imread(file_path[terget_num], 1);
		cv::cvtColor(src_img, gray_img, CV_RGB2GRAY);
		cv::medianBlur(gray_img, median_img, 5);
		cv::threshold(median_img, bin_img, 100, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		bitwise_and(gray_img, bin_img, gray_img);
		cv::Moments moment = cv::moments(bin_img, true);
		int gravityX = (int)(moment.m10 / moment.m00);
		int gravityY = (int)(moment.m01 / moment.m00);
		double ang = 0.5*atan2(2.0*moment.mu11, moment.mu20 - moment.mu02) * 180 / M_PI;
		cv::Point2f center(src_img.cols*0.5, src_img.rows*0.5);
		cv::Mat affine_matrix = cv::getRotationMatrix2D(center, 0, 1);
		affine_matrix.at<double>(0, 2) += 320 - gravityX;
		affine_matrix.at<double>(1, 2) += 240 - gravityY;
		cv::warpAffine(gray_img, moved_img, affine_matrix, median_img.size());
		affine_matrix = cv::getRotationMatrix2D(center, ang, 1);
		cv::warpAffine(moved_img, moved_img, affine_matrix, median_img.size());
		cv::cvtColor(moved_img, moved_img_c, CV_GRAY2BGR);
		cv::cvtColor(moved_img, labeled_c, CV_GRAY2BGR);

		cv::Mat label_src = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
		cv::Mat sigma_diff = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
		short	*label_result = new short[width * hight];

		for (int R = 0; R < src_img.rows; ++R) {
			for (int C = 0; C < src_img.cols; ++C) {
				//if ((Nsigma_img[5].data[R * 640 + C] > moved_img.data[R * 640 + C]) &&
				//	(Nsigma_img[1].data[R * 640 + C] < moved_img.data[R * 640 + C])) {
				//}
				if ((Nsigma_img[6].data[R * 640 + C] > moved_img.data[R * 640 + C]) &&
					(Nsigma_img[5].data[R * 640 + C] < moved_img.data[R * 640 + C])) {
					sigma_diff.data[R * 640 + C] =
						Nsigma_img[6].data[R * 640 + C] - moved_img.data[R * 640 + C];
					label_src.data[R * 640 + C] = 255;
					moved_img_c.data[(R * 640 + C) * 3 + 0] = 0;
					moved_img_c.data[(R * 640 + C) * 3 + 1] = 0;
					moved_img_c.data[(R * 640 + C) * 3 + 2] = 255;
				}
				else if ((Nsigma_img[1].data[R * 640 + C] > moved_img.data[R * 640 + C]) &&
					(Nsigma_img[0].data[R * 640 + C] < moved_img.data[R * 640 + C])) {
					sigma_diff.data[R * 640 + C] =
						moved_img.data[R * 640 + C] - Nsigma_img[0].data[R * 640 + C];
					label_src.data[R * 640 + C] = 255;
					moved_img_c.data[(R * 640 + C) * 3 + 0] = 0;
					moved_img_c.data[(R * 640 + C) * 3 + 1] = 0;
					moved_img_c.data[(R * 640 + C) * 3 + 2] = 255;
				}
				//else if ((Nsigma_img[6].data[R * 640 + C] > moved_img.data[R * 640 + C]) &&
				//	(Nsigma_img[0].data[R * 640 + C] < moved_img.data[R * 640 + C])) {
				//	moved_img_c.data[(R * 640 + C) * 3 + 0] = 0;
				//	moved_img_c.data[(R * 640 + C) * 3 + 1] = 0;
				//	moved_img_c.data[(R * 640 + C) * 3 + 2] = 255;
				//	label_src.data[R * 640 + C] = 255;
				//}

			}
		}
		//img open
		//cv::Mat element(1, 2, CV_8U, cv::Scalar::all(255));
		//cv::morphologyEx(open_img, open_img, cv::MORPH_OPEN, element);
		LabelingBS labeling;
		labeling.Exec(label_src.data, label_result, width, hight, true, Cam::label_mass_pixel);
		//cv::Mat labeled_c = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);	//color

		//for (int i = 0; i < labeling.GetNumOfResultRegions(); i++) {
		//	RegionInfoBS *ri = labeling.GetResultRegionInfo(i);
		//	cerr << i << "\t" << ri->GetNumOfPixels() << "\n";
		//}
		int label_array[LABEL_ARRAY_NUM] = { 0 };
		for (int R = 0; R < hight; ++R) {
			for (int C = 0; C < width; ++C) {
				label_array[label_result[R * 640 + C]] += sigma_diff.data[R * 640 + C];
				//if (label_result[R * 640 + C] > 1) {
				//	labeled_c.data[(R * 640 + C) * 3 + 0] = label_result[R * 640 + C] * 5;
				//	labeled_c.data[(R * 640 + C) * 3 + 1] = 255 - label_result[R * 640 + C] * 5;
				//	if (label_result[R * 640 + C] % 2 == 0) {
				//		labeled_c.data[(R * 640 + C) * 3 + 2] = 255;
				//	}
				//	else {
				//		labeled_c.data[(R * 640 + C) * 3 + 2] = 0;
				//	}

				//}

			}
		}
		label_array[0] = 0;	//background

		int label_array_tmp[LABEL_ARRAY_NUM] = { 0 };
		memcpy(label_array_tmp, label_array, LABEL_ARRAY_NUM);

		int sorted_label[10] = { 0 };
		for (int serch = 0; serch < 10; serch++) {
			int i_max = 0, i_tmp = 0;
			for (int i = 0; i < LABEL_ARRAY_NUM; i++) {
				if (i_max < label_array_tmp[i]) {
					i_max = label_array_tmp[i];
					i_tmp = i;
				}
			}
			label_array_tmp[i_tmp] = 0;
			sorted_label[serch] = i_tmp;
		}
		//for (int serch = 0; serch < 10; serch++) {
		//	cerr << sorted_label[serch] << "\t";
		//}
		//cerr << "\n";
		for (int R = 0; R < hight; ++R) {
			for (int C = 0; C < width; ++C) {
				for (int serch = 0; serch < 10; serch++) {
					if (label_result[R * 640 + C] == sorted_label[serch]) {
						labeled_c.data[(R * 640 + C) * 3 + 0] = 0;
						labeled_c.data[(R * 640 + C) * 3 + 1] = 0;
						labeled_c.data[(R * 640 + C) * 3 + 2] = (10 - serch)*10 + 150;
					}
				}
			}
		}

		//for (int i = 1; label_array[i] > 0; i++) {
		//	RegionInfoBS *ri = labeling.GetResultRegionInfo(i - 1);
		//	int ave_l = label_array[i] / ri->GetNumOfPixels();
		//	cerr << i << "\t" << label_array[i] << "\t" << ri->GetNumOfPixels() << "\t"
		//		<< ave_l << "\n";
		//}

		//cv::imshow("sigma_diff", sigma_diff);
		cv::imshow("labeled_c", labeled_c);
		//char oss[32];
		//sprintf(oss, "%d_tmp.bmp", filenum_static++);
		//Cam::write_path = Cam::auto_root + "\\auto_tmp\\" + oss;
		//cv::imwrite(Cam::write_path, labeled_c);

		cv::waitKey(5);
	}
	return;
}

void Cam::StartTimer() {
	Cam::label_mass_pixel = stoi(Cam::combo_text3);	//string to int
	int count = 0;
	timerID = timeSetEvent(1000, 0, (LPTIMECALLBACK)timerProc, (DWORD)&count, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
	if (!timerID) cerr << "set timer failed\n";

}

void Cam::StopTimer() {
	timeKillEvent(timerID);
}
void Cam::SetAuto(String^ auto_) {
	auto_file = msclr::interop::marshal_as<std::string>(auto_);
	auto_root = auto_file;
	auto_root.erase(auto_root.rfind("\\"));
	//read sigma_image as gray
	for (int sigma = 0; sigma < 7; sigma++) {
		char oss[32];
		sprintf(oss, "%dsigma.bmp", sigma - 3);
		write_path = auto_root + "\\probability_distribution\\" + oss;
		Nsigma_img[sigma] = cv::imread(write_path, 0);
	}


}