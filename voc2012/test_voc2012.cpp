#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include <opencv2/opencv.hpp>

#include "voc2012.h"

int main(int argc, char* argv[])
{
	std::srand(time(NULL));

	std::vector<std::string> train_img_list, train_anno_list,
							 val_img_list, val_anno_list;

	voc2012::load_voc2012("data", train_img_list, train_anno_list,
						  val_img_list, val_anno_list);

	std::cout << "train_img_list: " << train_img_list.size() << std::endl;
	std::cout << "train_anno_list: " << train_anno_list.size() << std::endl;
	std::cout << "val_img_list: " << val_img_list.size() << std::endl;
	std::cout << "val_anno_list: " << val_anno_list.size() << std::endl;

	while (true) {
		int idx = rand() % train_img_list.size();
		std::cout << "index: " << train_img_list[idx] << std::endl;

		cv::Mat img = cv::imread(train_img_list[idx]);
		std::vector<std::vector<int> > anno = voc2012::xml2list<int>(train_anno_list[idx], false);

		for (int i = 0; i < anno.size(); ++i) {
			cv::rectangle(img, cv::Point(anno[i][0], anno[i][1]), cv::Point(anno[i][2], anno[i][3]),
				cv::Vec3d(0, 255, 0), 3, cv::LINE_AA);
		}

		cv::imshow("voc2012", img);
		if (cv::waitKey(0) == 'q') {
			break;
		}
	}
	cv::destroyAllWindows();

	return 0;
}
