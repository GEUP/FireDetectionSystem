#include "histo.hpp"
#include "utils.hpp"

//�ߺ� �̹��� ������ ������
void removeDuplicatedImageFile(vector<char*>& imgNameList, Vec3i & bins, Vec3f & ranges, int flag, double  sinc) {
	
	//��� �̹����� ������׷� ���
	vector<Mat> DB_hists = load_histo(imgNameList, bins, ranges, imgNameList.size(), flag);

	//�˸��� ����
	Mat info = Mat(210, 400, CV_8UC3, Scalar(255, 255, 255));
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printf("���۹�\n\n Delete  (���� ����� �̹��� ����) \nSpace  (���� �̹��� ����)\nESC: ����\n");
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");

	putText(info, "is Duplicate?", Point(0, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255));
	putText(info, "YES: Delete", Point(0, 110), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	putText(info, "NO: Space", Point(0, 140), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	putText(info, "QUIT: ESC", Point(0, 170), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

	imshow("Infomation", info);
	moveWindow("Infomation", 0, 0);
	int i = 0;
	for (const auto& a : imgNameList) {

		//�̹��� ����
		char img[256] = FOLDER;
		char buf[256];
		sprintf_s(buf, "%d/", flag);
		strncat_s(img, buf, 256);
		strncat_s(img, a, 256);
		printf("\n----%s----\n", a);
		Mat query = imread(img, IMREAD_COLOR);
		if (query.empty()) {
			i++;
			continue;
		}
		imshow("����", query);
		moveWindow("����", 400, 0);
		
		//���� �̹��� ������׷� ���
		Mat hsv, query_hist;
		cvtColor(query, hsv, COLOR_BGR2HSV);					// HSV �÷� ��ȯ
		calc_histo(hsv, query_hist, bins, ranges, 2);			//������׷� ���
		
		//��� �̹����� ������׷��� ���絵 ��
		Mat DB_similaritys = calc_similarity(query_hist, DB_hists);
		
		//������ �̻����� ���絵�� ���� �̹����� ����ϰ� ���� ���θ� ������
		select_view(imgNameList, sinc, DB_similaritys, bins, ranges,flag,i);

		i++;
	}
}

int main()
{
	Vec3i bins(30, 42, 0);
	Vec3f ranges(180, 256, 0);
	vector<char*> nonFireFileList;
	vector<char*> fireFileList;
	vector<char*> smokeFileList;
	char nonFireDir[256] = "./data/crawled_images/0/*.jpg";
	char fireDir[256] = "./data/crawled_images/1/*.jpg";
	char smokeDir[256] = "./data/crawled_images/2/*.jpg";

	double  sinc;
	cout << "   ���� ���絵(0~1) �Է�: ";
	cin >> sinc;
	
	printf("���絵 %f", sinc);
	removeN = 0;
	getFileList(nonFireDir, nonFireFileList);
	removeDuplicatedImageFile(nonFireFileList, bins, ranges , 0, sinc);
	printf("������ �Ϲ� �̹��� : %d\n", removeN);

	removeN = 0;
	getFileList(fireDir, fireFileList);
	removeDuplicatedImageFile(fireFileList, bins, ranges, 1, sinc);
	printf("������ ȭ�� �̹��� : %d\n", removeN);

	removeN = 0;
	getFileList(smokeDir, smokeFileList);
	removeDuplicatedImageFile(smokeFileList, bins, ranges, 2, sinc);
	printf("������ ���� �̹��� : %d\n", removeN);

	return 0;
}