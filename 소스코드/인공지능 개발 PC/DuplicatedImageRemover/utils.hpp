#define FOLDER "./data/crawled_images/"

//�ϳ��� ������׷�(query_hist)�� ��� ������׷���(DB_hists)�� ���Ͽ� �񱳰��� DB_similaritys�� ����
Mat calc_similarity(Mat queryHist, vector<Mat>& DB_hists)
{
	Mat DB_similaritys;

	for (int i = 0; i < (int)DB_hists.size(); i++)
	{
		double compare = compareHist(queryHist, DB_hists[i], HISTCMP_CORREL);
		DB_similaritys.push_back(compare);
	}

	return DB_similaritys;
}

//DB_similaritys�� Ž���Ͽ� ������(sinc) �̻����� ���絵�� ���� �̹����� ����ϰ� ���� ���θ� ������
void select_view(vector<char*>& imgNameList, double sinc, Mat DB_similaritys, Vec3i bins, Vec3f ranges, int flag, int n)
{
	Mat m_idx, sorted_sim;
	sortIdx(DB_similaritys, m_idx, SORT_EVERY_COLUMN + SORT_DESCENDING);
	cv::sort(DB_similaritys, sorted_sim, SORT_EVERY_COLUMN + SORT_DESCENDING);

	for (int i = n+1; i < (int)sorted_sim.total(); i++)
	{
		double  sim = sorted_sim.at<double>(i);
		if (sim > sinc)
		{
			int idx = m_idx.at<int>(i);
			char fname[256];
			sprintf_s(fname,"%s%d/%s", FOLDER, flag, imgNameList[idx]);
			Mat img = imread(fname, IMREAD_COLOR);
			if (img.empty()) {
				continue;
			}
			String  title = format("%s", imgNameList[idx]);
			cout << title << "���絵 = "<< sim << endl;
			imshow(title, img);
			moveWindow(title, 400, img.rows);
			int key = waitKey();
			if (key == 0) {
				remove(fname);
				printf("%s is removed\n", fname);
				Sleep(2);
			}
			if (key == 27) {
				destroyWindow(title);
				return;
			}
			destroyWindow(title);
		}
	}
}

//imgPath ������ �����ϴ� ��� �̹���(.jpg) ���� �̸��� imgNameList�� ����
void getFileList(char* imgPath, vector<char*>& imgNameList) {
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	if ((hFind = FindFirstFile(imgPath, &FindFileData)) != INVALID_HANDLE_VALUE) {
		do {
			char* finename = (char*)malloc(256);
			strncpy_s(finename, 256, FindFileData.cFileName, 256);
			imgNameList.push_back(finename);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	else {
		printf("INVALID_HANDLE_VALUE");
		exit(1);
	}
}