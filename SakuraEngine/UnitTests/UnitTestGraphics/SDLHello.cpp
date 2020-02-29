/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:43:56
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 13:32:14
 */
#include <iostream>
#include "VkTestApplication.h"

int main(int argc, char** argv)
{
	auto app = std::make_unique<VkTestApplication>();
	app->run();
	return 0;
}