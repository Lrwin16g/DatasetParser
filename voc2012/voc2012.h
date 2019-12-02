#ifndef _VOC2012_H_
#define _VOC2012_H_

#include <fstream>
#include <vector>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

namespace voc2012
{
	const std::vector<std::string> voc_classes = { "aeroplane", "bicycle", "bird", "boat",
												   "bottle", "bus", "car", "cat", "chair",
												   "cow", "diningtable", "dog", "horse",
												   "motorbike", "person", "pottedplant",
												   "sheep", "sofa", "train", "tvmonitor" };

	// XML形式のアノテーションデータを変換
	template<typename Type>
	std::vector<std::vector<Type> > xml2list(std::string xml_path, bool normalize = true)
	{
		boost::property_tree::ptree pt;
		boost::property_tree::read_xml(xml_path, pt);

		// 画像サイズ
		Type width = pt.get_optional<Type>("annotation.size.width").get();
		Type height = pt.get_optional<Type>("annotation.size.height").get();

		std::vector<std::vector<Type> > anno_list;

		BOOST_FOREACH(const boost::property_tree::ptree::value_type & child, pt.get_child("annotation"))
		{
			if (child.first == "object")
			{
				// difficultに設定されているものは除外
				int difficult = child.second.get_optional<int>("difficult").get();
				if (difficult == 1) {
					continue;
				}

				// 物体名
				std::string name = child.second.get_optional<std::string>("name").get();

				// バウンディングボックスの情報　※VOCは原点が(1,1)なので1を引いて(0,0)に
				Type xmin = child.second.get_optional<Type>("bndbox.xmin").get() - 1;
				Type ymin = child.second.get_optional<Type>("bndbox.ymin").get() - 1;
				Type xmax = child.second.get_optional<Type>("bndbox.xmax").get() - 1;
				Type ymax = child.second.get_optional<Type>("bndbox.ymax").get() - 1;

				// 画像のサイズで正規化
				if (normalize) {
					xmin /= width;
					ymin /= height;
					xmax /= width;
					ymax /= height;
				}

				// アノテーションのクラス名をindexに変換
				Type index = -1;
				for (int i = 0; i < voc_classes.size(); ++i) {
					if (voc_classes[i] == name) {
						index = i;
						break;
					}
				}

				std::vector<Type> elem = { xmin, ymin, xmax, ymax, index };
				anno_list.push_back(elem);
			}
		}

		return anno_list;
	}

	std::vector<std::string> loadIDList(std::string filename, std::string root_path, std::string ext)
	{
		std::vector<std::string> id_list;
		std::ifstream ifs(filename);
		std::string line;
		while (std::getline(ifs, line)) {
			std::string id = root_path + line + ext;
			id_list.push_back(id);
		}

		return id_list;
	}

	void load_voc2012(std::string root_path, std::vector<std::string> &train_img_list,
					  std::vector<std::string> &train_anno_list, std::vector<std::string> &val_img_list,
					  std::vector<std::string> &val_anno_list)
	{
		std::string img_path = root_path + "\\JPEGImages\\";
		std::string anno_path = root_path + "\\Annotations\\";
		std::string train_path = root_path + "\\ImageSets\\Main\\train.txt";
		std::string val_path = root_path + "\\ImageSets\\Main\\val.txt";

		// 訓練と検証のファイルIDを取得
		train_img_list = loadIDList(train_path, img_path, ".jpg");
		train_anno_list = loadIDList(train_path, anno_path, ".xml");
		val_img_list = loadIDList(val_path, img_path, ".jpg");
		val_anno_list = loadIDList(val_path, anno_path, ".xml");
	}
};

#endif