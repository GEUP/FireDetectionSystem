#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <windows.h>

using namespace cv;
using namespace std;

//�� �̹���(img)�� ������׷��� ����Ͽ� hist�� ����
void calc_histo(const Mat& img, Mat& hist, Vec3i bins, Vec3f range, int _dims)
{
	int dims = (_dims <= 0) ? img.channels() : _dims;// ������׷� ������
	int		channels[] = { 0, 1, 2 };
	int		histSize[] = { bins[0], bins[1], bins[2] };

	float  range1[] = { 0, range[0] };
	float  range2[] = { 0, range[1] };
	float  range3[] = { 0, range[2] };
	const float* ranges[] = { range1, range2, range3 };

	calcHist(&img, 1, channels, Mat(), hist, dims, histSize, ranges);
	normalize(hist, hist, 0, 1, NORM_MINMAX);			// ����ȭ
}

//imgNameList�� �ִ� ��� �̹����� ������׷� ����Ͽ� DB_hists�� �����ϰ� ��ȯ
vector<Mat> load_histo(vector<char*>& imgNameList, Vec3i bins, Vec3f ranges, int nImages, int flag)
{
	vector<Mat> DB_hists;
	for (int i = 0; i < nImages; i++)
	{
		String  fname = format("./data/crawled_images/%d/%s", flag, imgNameList[i]);
		Mat hsv, hist, img = imread(fname, IMREAD_COLOR);
		if (img.empty()) continue;

		cvtColor(img, hsv, COLOR_BGR2HSV);			// HSV �÷� ��ȯ
		calc_histo(hsv, hist, bins, ranges, 2);			// 2���� ������׷� ���
		DB_hists.push_back(hist);
	}
	cout << format("%d ���� ������ �ε� �� ������׷� ��� �Ϸ�", DB_hists.size()) << endl;
	return DB_hists;
}

