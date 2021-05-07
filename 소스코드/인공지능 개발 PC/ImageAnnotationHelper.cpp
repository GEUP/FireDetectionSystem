#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <windows.h>


#define FOLDER "./data/crawled_images/"
using namespace cv;
using namespace std;

int mouse_mode = 1; //�巡�� ������ ���� �÷���
Point pt1, pt2;	//�簢�� �»�� ���ϴ� ��ǥ
Mat current_image;
char src_img_window_name[256];



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

//�巹�� ���콺 �̺�Ʈ ó���Ͽ� �簢���� �»�� ��ǥ�� ���ϴ� ��ǥ�� p1, p2�� ������ 
static void onMouse(int event, int x, int y, int, void*)
{
	if (x > current_image.cols)
		x = current_image.cols-1;
	else if (x <= 0)
		x = 1;

	if (y > current_image.rows)
		y = current_image.rows-1;
	else if (y<= 0)
		y = 1;
	Point pt(x, y);
	Mat image;
	if ((mouse_mode==3)&&(event == EVENT_LBUTTONUP)) 				// ���ʹ�ư ����
	{		
		pt2 = pt;										// ������ǥ ����
		mouse_mode = 1;								// ��ư ���� ���� ����
		if (pt1.x > pt2.x) {
			int q = pt1.x;
			pt1.x = pt2.x;
			pt2.x = q;
		}
		if (pt1.y > pt2.y) {
			int q = pt1.y;
			pt1.y = pt2.y;
			pt2.y = q;
		}
		printf("(%d, %d) (%d, %d)\n", pt1.x, pt1.y, pt2.x, pt2.y);
		current_image.copyTo(image);
		rectangle(image, pt1, pt2, Scalar(0, 255, 0), 2);
		imshow(src_img_window_name, image);

	}
	else if ((mouse_mode == 1) && (event == EVENT_LBUTTONDOWN)) 	// ���ʹ�ư ������
	{
		pt1 = pt;										// ������ǥ ����
		mouse_mode = 2;
		current_image.copyTo(image);
	}
	
	if (mouse_mode >= 2) {							// ���ʹ�ư ������ or �巡�� 
		mouse_mode = 3;		// ���콺 ���� ����
		pt2 = pt;
		current_image.copyTo(image);
		rectangle(image, pt1, pt2, Scalar(0, 0, 255),3);
		imshow(src_img_window_name, image);
	}
	
	
}

//�̹���(Mat)�� �� ���� ����Ʈ(p1, p2)�� ���� �̹�������(.xml)���ϰ� �̹���(.jpg) ���� ����
void writeXML(Mat & image) {
	char folder[256]= FOLDER;
	char img[256];
	char dst_img[256];
	char xml[256] = ".xml";
	char dst_xml[256];
	strncpy_s(img, 256, src_img_window_name, 256);
	sprintf_s(dst_img, "%sannotated/%s", folder, img);
	
	for (int i = 0; i < strlen(img); i++) {
		if (img[i] == '.') {
			img[i] = '\0';
			break;
		}
	}
	sprintf_s(dst_xml, "%sannotated/%s%s", folder,img, xml);
	printf("imwrite: %s\nfprintf: %s\n",dst_img,dst_xml);
	imwrite(dst_img, image);
	FILE* fp;
	fopen_s(&fp, dst_xml, "wt");
	fprintf(fp,"<annotation>\n"
				"\t<folder>annotated</folder>\n"
				"\t<filename>%s</filename>\n"
				"\t<path>%s</path>\n"
				"\t<source>\n"
				"\t\t<database>Unknown</database>\n"
				"\t</source>\n"
				"\t<size>\n"
				"\t\t<width>%d</width>\n"
				"\t\t<height>%d</height>\n"
				"\t\t<depth>%d</depth>\n"
				"\t</size>\n"
				"\t<segmented>1</segmented>\n"
				"\t<object>\n"
				"\t\t<name>firew</name>\n"
				"\t\t<pose>Unspecified</pose>\n"
				"\t\t<truncated>0</truncated>\n"
				"\t\t<difficult>0</difficult>\n"
				"\t\t<bndbox>\n"
				"\t\t\t<xmin>%d</xmin>\n"
				"\t\t\t<ymin>%d</ymin>\n"
				"\t\t\t<xmax>%d</xmax>\n"
				"\t\t\t<ymax>%d</ymax>\n"
				"\t\t</bndbox>\n"
				"\t</object>\n"
				"</annotation>", src_img_window_name,dst_img,image.cols,image.rows,image.channels(),pt1.x,pt1.y,pt2.x,pt2.y);
}

//�̹����� ����ϰ� �̹��� ��� �����쿡 �巡���Ͽ� �簢�� ��ǥ ������ xml ���Ϸ� ������ �� �ֵ��� ������
void imageAnnotation(vector<char*>& imgNameList, const char* question, int flag) {

	Mat info = Mat(190, 800, CV_8UC3, Scalar(255, 255, 255));

	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printf("���۹�\n\nSpace: Ȯ��  (���� �̹��� ����)\nESC: ����\n");
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");

	putText(info, question, Point(0, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255));
	putText(info, "YES: Space", Point(0, 110), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	putText(info, "QUIT: ESC", Point(0, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	
	imshow(question, info);
	moveWindow(question, 0, 0);
	for (const auto& a : imgNameList) {
		char src_img[256] = FOLDER;
		char buf[256];
		sprintf_s(buf, "%d/", flag);
		strncat_s(src_img, buf, 256);
		strncat_s(src_img, a, 256);
		Mat image = imread(src_img, IMREAD_COLOR);
		resize(image, image, Size(400, 400));
		imshow(a, image);
		image.copyTo(current_image);
		setMouseCallback(a, onMouse, 0);
		strncpy_s(src_img_window_name, a, 256);
		moveWindow(a, 400, 100);
		int key;
		do {
			key = waitKey();
		} while ((key != 27) && (key != 32));
		destroyWindow(a);
		if (key == 27)
			break;
		else if (key == 32) {
			writeXML(image);
		}
	}
	destroyWindow(question);
}

int main(void)
{
	
	vector<char*> fireFileList;
	char fireDir[256];
	char cwd[256];
	sprintf_s(fireDir, "%s/1/*.jpg", FOLDER);
	
	getFileList(fireDir, fireFileList);
	imageAnnotation(fireFileList,"Capture Fire" ,1);
	
	
	return 0;
}