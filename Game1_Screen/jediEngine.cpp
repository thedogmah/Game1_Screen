#include "jediEngine.h"

jediEngine::jediEngine()
{
}

void jediEngine::loadAssetFile()
{

	std::fstream assetFstream;
	std::stringstream ssAsset;
	std::string assetName;

	assetFstream.open("assetlist.txt");
	if (assetFstream.is_open())
	{
		int item = 1;
		int loop = 0;
		std::cout << "Tree file open\n";
		while (std::getline(assetFstream, assetName))
		{

			std::stringstream oss(assetName);
			while (oss >> assetName)

			{
				//oss >> asset;

				loop++;
				std::cout << "Loaded Asset: " << assetName << ", " << '\n';
				//sTreeAsset.x = std::stof(asset);//	std::cout << line << std::endl;}
				vAssetLabel.push_back(assetName);

				}

			}
		}
	}

void jediEngine::addAsset(char* label)
{
	vAssetLabel.push_back(label);

}
void jediEngine::saveAssets()
{

	assetFstream.open("assetlist.txt");
	if (assetFstream.is_open())
	{

		for (auto asset : vAssetLabel)
			assetFstream << asset << '\n';
	}
	assetFstream.close();
	std::cout << "\nAssets Saved to assetlist.txt";
}
