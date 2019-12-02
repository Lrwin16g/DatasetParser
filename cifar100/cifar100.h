#ifndef _CIFAR10_H_
#define _CIFAR10_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace cifar100
{
	const int num_train_images = 50000;	// 訓練画像の数
	const int num_test_images = 10000;	// テスト画像の数
	const int channel = 3;	// チャンネル数
	const int rows = 32;	// 画像縦サイズ
	const int cols = 32;	// 画像横サイズ
	const int num_super_category = 20;	// スーパークラス数
	const int num_sub_category = 100;	// サブクラス数

	// 画像データを読み出し
	template<typename Type1, typename Type2>
    void loadImages(std::string filename, std::vector<std::vector<std::vector<std::vector<Type1> > > > &images,
					std::vector<std::vector<Type2> > &super_labels, std::vector<std::vector<Type2> > &sub_labels)
	{
		std::ifstream ifs;
		ifs.exceptions(std::fstream::badbit);
		ifs.open(filename.c_str(), std::ios::in | std::ios::binary);

		if (!ifs.is_open()) {
			throw std::runtime_error("failed to open " + filename);
		}

		images.clear();
		super_labels.clear();
		sub_labels.clear();

		while (true)
		{
			std::vector<Type2> super_label(num_super_category);
			unsigned char label_1 = 0xff;
			ifs.read(reinterpret_cast<char*>(&label_1), sizeof(label_1));
			super_label[label_1] = 1;
			super_labels.push_back(super_label);

			std::vector<Type2> sub_label(num_sub_category);
			unsigned char label_2 = 0xff;
			ifs.read(reinterpret_cast<char*>(&label_2), sizeof(label_2));
			sub_label[label_2] = 1;
			sub_labels.push_back(sub_label);

			std::vector<std::vector<std::vector<Type1> > > image(channel);
			for (int i = 0; i < channel; ++i)
			{
				image[i].resize(rows);

				for (int y = 0; y < rows; ++y)
				{
					image[i][y].resize(cols);

					for (int x = 0; x < cols; ++x)
					{
						unsigned char temp = 0;
						ifs.read(reinterpret_cast<char*>(&temp), sizeof(temp));
						image[i][y][x] = static_cast<Type1>(temp);
					}
				}
			}

			if (ifs.eof()) {
				break;
			}

			images.push_back(image);
		}

#ifdef _DEBUG
		std::cout << "number_of_images: " << images.size() << std::endl;
#endif
	}

	// ラベル名称を読み出し
    void loadLabelNames(std::string filename, std::vector<std::string> &label_names)
	{
		std::ifstream ifs;
		ifs.exceptions(std::fstream::badbit);
		ifs.open(filename.c_str(), std::ios::in);

		if (!ifs.is_open()) {
			throw std::runtime_error("failed to open " + filename);
		}

		label_names.clear();

		while (true)
		{
			std::string str;
			ifs >> str;

			if (ifs.eof()) {
				break;
			}

			label_names.push_back(str);
		}

#ifdef _DEBUG
		std::cout << "number of categories: " << label_names.size() << std::endl;
		for (int i = 0; i < label_names.size(); ++i) {
			std::cout << label_names[i] << " ";
		}
		std::cout << std::endl;
#endif
	}

	// cifar-100データをまとめて読み出し
	template<typename Type1, typename Type2>
    void load_cifar100(std::vector<std::vector<std::vector<std::vector<Type1> > > > &train_images, std::vector<std::vector<Type2> > &train_super_labels,
					   std::vector<std::vector<Type2> > &train_sub_labels, std::vector<std::vector<std::vector<std::vector<Type1> > > > &test_images,
					   std::vector<std::vector<Type2> > &test_super_labels, std::vector<std::vector<Type2> > &test_sub_labels, std::string dir_path)
	{
		cifar100::loadImages(dir_path + "/train.bin", train_images, train_super_labels, train_sub_labels);
		cifar100::loadImages(dir_path + "/test.bin", test_images, test_super_labels, test_sub_labels);
	}
};

#endif