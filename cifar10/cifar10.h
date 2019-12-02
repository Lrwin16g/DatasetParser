#ifndef _CIFAR10_H_
#define _CIFAR10_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace cifar10
{
	const int number_of_images = 10000;	// 1ファイル辺りの画像数
	const int channel = 3;	// チャンネル数
	const int rows = 32;	// 画像縦サイズ
	const int cols = 32;	// 画像横サイズ
	const int number_of_categories = 10;	// クラス数

	// 画像データを読み出し
	template<typename Type1, typename Type2>
    void loadImages(std::string filename, std::vector<std::vector<std::vector<std::vector<Type1> > > > &images,
					std::vector<std::vector<Type2> > &labels)
	{
		std::ifstream ifs(filename, std::ios::in | std::ios::binary);

		if (!ifs.is_open()) {
			throw std::runtime_error("failed to open " + filename);
		}

		images.resize(number_of_images);
		labels.resize(number_of_images);

		for (int i = 0; i < number_of_images; ++i)
		{
			images[i].resize(channel);
			labels[i].resize(number_of_categories);
			for (int j = 0; j < number_of_categories; ++j) {
				labels[i][j] = 0;
			}

			unsigned char label = 0xff;
			ifs.read((char*)&label, sizeof(label));
			labels[i][label] = 1;

			for (int j = 0; j < channel; ++j)
			{
				images[i][j].resize(rows);

				for (int y = 0; y < rows; ++y)
				{
					images[i][j][y].resize(cols);

					for (int x = 0; x < cols; ++x)
					{
						unsigned char temp = 0;
						ifs.read((char*)&temp, sizeof(temp));
						images[i][j][y][x] = static_cast<Type1>(temp);
					}
				}
			}
		}
	}

	// ラベル名称を読み出し
    void loadLabelNames(std::string filename, std::vector<std::string> &label_names)
	{
		std::ifstream ifs(filename, std::ios::in | std::ios::binary);

		if (!ifs.is_open()) {
			throw std::runtime_error("failed to open " + filename);
		}

		label_names.resize(number_of_categories);

		for (int i = 0; i < number_of_categories; ++i)
		{
			getline(ifs, label_names[i]);
		}

#ifdef _DEBUG
		for (int i = 0; i < number_of_categories; ++i) {
			std::cout << label_names[i] << " ";
		}
		std::cout << std::endl;
#endif
	}

	// cifar-10データをまとめて読み出し
	template<typename Type1, typename Type2>
    void load_cifar10(std::vector<std::vector<std::vector<std::vector<Type1> > > > &train_images, std::vector<std::vector<Type2> > &train_labels,
                      std::vector<std::vector<std::vector<std::vector<Type1> > > > &test_images, std::vector<std::vector<Type2> > &test_labels,
                      std::string dir_path)
	{
		std::vector<std::vector<std::vector<std::vector<Type1> > > > images_1, images_2, images_3, images_4, images_5;
		std::vector<std::vector<Type2> > labels_1, labels_2, labels_3, labels_4, labels_5;

		cifar10::loadImages<Type1, Type2>(dir_path + "/data_batch_1.bin", images_1, labels_1);
		cifar10::loadImages<Type1, Type2>(dir_path + "/data_batch_2.bin", images_2, labels_2);
		cifar10::loadImages<Type1, Type2>(dir_path + "/data_batch_3.bin", images_3, labels_3);
		cifar10::loadImages<Type1, Type2>(dir_path + "/data_batch_4.bin", images_4, labels_4);
		cifar10::loadImages<Type1, Type2>(dir_path + "/data_batch_5.bin", images_5, labels_5);

		cifar10::loadImages<Type1, Type2>(dir_path + "/test_batch.bin", test_images, test_labels);

		train_images.clear();
		std::copy(images_1.begin(), images_1.end(), std::back_inserter(train_images));
		std::copy(images_2.begin(), images_2.end(), std::back_inserter(train_images));
		std::copy(images_3.begin(), images_3.end(), std::back_inserter(train_images));
		std::copy(images_4.begin(), images_4.end(), std::back_inserter(train_images));
		std::copy(images_5.begin(), images_5.end(), std::back_inserter(train_images));

		train_labels.clear();
		std::copy(labels_1.begin(), labels_1.end(), std::back_inserter(train_labels));
		std::copy(labels_2.begin(), labels_2.end(), std::back_inserter(train_labels));
		std::copy(labels_3.begin(), labels_3.end(), std::back_inserter(train_labels));
		std::copy(labels_4.begin(), labels_4.end(), std::back_inserter(train_labels));
		std::copy(labels_5.begin(), labels_5.end(), std::back_inserter(train_labels));
	}
};

#endif