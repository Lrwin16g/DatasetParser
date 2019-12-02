#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <exception>

#include <opencv2/opencv.hpp>

#include "mnist.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <input_dir> <output_dir>" << std::endl;
		return -1;
	}

	std::string src_dir(argv[1]);
	std::string dst_dir(argv[2]);

	std::vector<std::vector<std::vector<unsigned char> > > train_images;
	std::vector<std::vector<std::vector<unsigned char> > > test_images;
	std::vector<std::vector<int> > train_labels;
	std::vector<std::vector<int> > test_labels;

	try
	{
		mnist::load_mnist<unsigned char, int>(train_images, train_labels, test_images, test_labels,
											  src_dir);

		int digit = static_cast<int>(log10(train_images.size()) + 1);
		int height = train_images[0].size();
		int width = train_images[0][0].size();
		int num_category = train_labels[0].size();

		std::string label_file(dst_dir + "/label.csv");
		std::ofstream ofs(label_file);

		// train image
		for (int i = 0; i < train_images.size(); ++i) {
			cv::Mat image(height, width, CV_8UC1);
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					image.at<unsigned char>(y, x) = train_images[i][y][x];;
				}
			}

			std::ostringstream oss;
			oss << dst_dir << "/train/" << std::setw(digit) << std::setfill('0') << i << ".png";
			cv::imwrite(oss.str(), image);

#ifdef _DEBUG
			std::cout << "Save: " << oss.str() << std::endl;
#endif

			ofs << oss.str() << ",";
			for (int j = 0; j < num_category; ++j) {
				std::ostringstream oss_label;
				oss_label << train_labels[i][j];
				ofs << oss_label.str() << ",";
			}
			ofs << std::endl;
		}

		// test image
		for (int i = 0; i < test_images.size(); ++i) {
			cv::Mat image(height, width, CV_8UC1);
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					image.at<unsigned char>(y, x) = test_images[i][y][x];;
				}
			}

			std::ostringstream oss;
			oss << dst_dir << "/test/" << std::setw(digit) << std::setfill('0') << i << ".png";
			cv::imwrite(oss.str(), image);

#ifdef _DEBUG
			std::cout << "Save: " << oss.str() << std::endl;
#endif

			ofs << oss.str() << ",";
			for (int j = 0; j < num_category; ++j) {
				std::ostringstream oss_label;
				oss_label << test_labels[i][j];
				ofs << oss_label.str() << ",";
			}
			ofs << std::endl;
		}

		ofs.close();
	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
