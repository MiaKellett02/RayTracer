
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

#include "ColourRGB.h"
#include "Camera.h"
#include <libMath.h>

#include "Material.h"
#include "Ellipsoid.h"
#include "DirectionalLight.h"
#include "Scene.h"
typedef enum input_args
{
	OUTPUT_FILE = 1,
	OUTPUT_WIDTH,
	OUTPUT_HEIGHT,
	OUTPUT_RPP,
}input_args;


void displayUsage(char* a_path)
{
	std::string fullpath = a_path; //get the full path as a string
		//strip off the path part of the string to only keep the executable name
	std::string exeName = fullpath.substr(fullpath.find_last_of('\\') + 1, fullpath.length());
	//display a message to the user indicating usage of the executable
	std::cout << "usage: " << exeName << " [output image name] [image width] [image height] [Rays Per Pixel]" << std::endl;
	std::cout << "\t-h or --help\t\tShow this message" << std::endl;
}

int main(int argv, char* argc[])
{
	//Set up the dimensions of the image
	int imageWidth = 256;
	int imageHeight = 256;
	int raysPerPixel = 100;
	int bounces = 10;
	int channelColours = 255;
	//output file name
	std::string outputFilename;
#pragma region Process Variadic Args
	if (argv < 2) //less than 2 as the path and executable name are always present
	{
		displayUsage(argc[0]);
		return EXIT_SUCCESS;
	}
	else //Some variadic arguments have been passed
	{
		for (int i = 1; i < argv; ++i)
		{
			std::string arg = argc[i];
			if (arg == "-h" || arg == "--help")
			{
				displayUsage(argc[0]);
				return EXIT_SUCCESS;
			}
			switch (i)
			{
				case OUTPUT_FILE:
				{
					outputFilename = argc[OUTPUT_FILE];
					//check to see if the extension was included
					if (outputFilename.find_last_of(".") == std::string::npos)
					{
						//no extension better add one
						outputFilename.append(".ppm");
					}
					break;
				}
				case OUTPUT_WIDTH:
				{
					imageWidth = atoi(argc[OUTPUT_WIDTH]);
					break;
				}
				case OUTPUT_HEIGHT:
				{
					imageHeight = atoi(argc[OUTPUT_HEIGHT]);
					break;
				}
				case OUTPUT_RPP:
				{
					raysPerPixel = atoi(argc[OUTPUT_RPP]);
				}
				default:
				{
					continue;
				}
			}
		}

	}
#pragma endregion
	//Need to change std::cout to outputFileName
	std::streambuf* backup = std::cout.rdbuf();	//backup std::cout's stream buffer
	std::ofstream outbuff(outputFilename.c_str());	//create an out file stream and set it to our output file name
	std::cout.rdbuf(outbuff.rdbuf());	//set the stream buffer for cout to the file out stream buffer

	Scene mainScene;

	Camera mainCamera;
	mainCamera.SetPerspective(60.f, (float)imageWidth / (float)imageHeight, 0.1f, 1000.f);
	mainCamera.SetPosition(Vector3(-10.f, 2.0f, -10.f));
	mainCamera.LookAt(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));

	Random::SetSeed(time(nullptr));

	//output the Image Header data
	std::cout << "P3" << std::endl;
	std::cout << imageWidth << ' ' << imageHeight << std::endl;
	std::cout << channelColours << std::endl;

	//Put a light in the scene
	DirectionalLight dl = DirectionalLight(Matrix4::IDENTITY, Vector3(0.75f, 0.75f, 0.75f), Vector3(-0.5f, -0.707f, -0.5));

	//Define some materials for use with any objects in the scene.
	Material lightBlueSmooth = Material(Vector3(0.2f, 0.6f, 1.0f), 0.1f, 0.3f, 0.9f, 0.0f, 1.0f, 0.9f, 1.52f);
	Material lightBlueRough = Material(Vector3(0.2f, 0.6f, 1.0f), 0.2f, 0.9f, 0.3f, 0.02f, 0.6f, 0.0f, 1.52f);
	Material greenSmooth = Material(Vector3(0.0f, 0.6f, 0.0f), 0.2f, 0.3f, 0.9f, 0.0f, 0.0f, 1.0f, 1.52f);
	Material greenRough = Material(Vector3(0.0f, 0.6f, 0.0f), 0.2f, 0.9f, 0.5f, 1.0f, 0.0f, 0.0f, 2.61f);
	Material yellowRough = Material(Vector3(0.5f, 0.5f, 0.0f), 0.2f, 0.6f, 0.7f, 1.0f, 0.0f, 0.0f, 2.61f);
	Material water = Material(Vector3(0.2f, 0.6f, 1.0f), 0.2f, 0.9f, 0.3f, 0.02f, 0.6f, 0.9f, 1.333f);
	Material brown = Material(Vector3(0.412f, 0.353f, 0.275f), 0.2f, 0.9f, 0.5f, 1.0f, 0.0f, 0.0f, 2.61f);
	Material grey = Material(Vector3(0.502f, 0.502f, 0.502f), 0.2f, 0.6f, 0.7f, 1.0f, 0.0f, 0.0f, 2.61f);
	Material greyMetal = Material(Vector3(0.502f, 0.502f, 0.502f), 0.2f, 0.6f, 0.7f, 1.0f, 0.9f, 0.0f, 2.61f);

	//Define the Ground spheres.
	Ellipsoid lakeBottom(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	lakeBottom.SetScale(Vector3(500.0f, 0.001f, 500.0f));
	lakeBottom.SetMaterial(&greenRough);

	//Define the water sphere.
	Ellipsoid lakeWater(Vector3(-2.0f, 0.001f, -17.0f), 1.0f);
	lakeWater.SetScale(Vector3(25.0f, 0.003f, 25.0f));
	lakeWater.SetMaterial(&water);

	//Define trees.
	Ellipsoid treeTrunk(Vector3(-10.0f, 0.0f, 20.0f), 1.0f);
	treeTrunk.SetScale(Vector3(1.0f, 10.0f, 1.0f));
	treeTrunk.SetMaterial(&brown);
	Ellipsoid treeLeaves(Vector3(-10.0f, 10.0f, 20.0f), 3.0f);
	treeLeaves.SetScale(Vector3(1.0f, 1.5f, 1.0f));
	treeLeaves.SetMaterial(&greenRough);

	//Define rocks.
	Ellipsoid rock1(Vector3(20.0f, 0.0f, 15.0f), 2.0f);
	rock1.SetScale(Vector3(5.0f, 2.0f, 1.0f));
	rock1.SetMaterial(&grey);

	//Define a UFO
	Ellipsoid saucer(Vector3(100.0f, 10.0f, 15.0f), 2.0f);
	saucer.SetScale(Vector3(10.0f, 1.0f, 10.0f));
	saucer.SetMaterial(&greyMetal);
	Ellipsoid engines(Vector3(100.0f, 10.0f, 15.0f), 1.0f);
	engines.SetScale(Vector3(3.0f, 6.0f, 3.0f));
	engines.SetMaterial(&yellowRough);

	//Define a glass table.
	Ellipsoid tableTop(Vector3(-5.0f, 2.0f, 20.0f), 3.0f);
	tableTop.SetScale(Vector3(1.0f, 0.1f, 1.0f));
	tableTop.SetMaterial(&lightBlueSmooth);
	Ellipsoid tableLeg1(Vector3(-3.5f, 1.0f, 18.5f), 1.0f);
	tableLeg1.SetScale(Vector3(0.1f, 1.0f, 0.1f));
	tableLeg1.SetMaterial(&grey);
	Ellipsoid tableLeg2(Vector3(-6.5f, 1.0f, 18.5f), 1.0f);
	tableLeg2.SetScale(Vector3(0.1f, 1.0f, 0.1f));
	tableLeg2.SetMaterial(&grey);
	Ellipsoid tableLeg3(Vector3(-5.0f, 1.0f, 21.5f), 1.0f);
	tableLeg3.SetScale(Vector3(0.1f, 1.0f, 0.1f));
	tableLeg3.SetMaterial(&grey);

	mainScene.AddObject(&lakeBottom);
	mainScene.AddObject(&lakeWater);
	mainScene.AddObject(&treeTrunk);
	mainScene.AddObject(&treeLeaves);
	mainScene.AddObject(&rock1);
	mainScene.AddObject(&saucer);
	mainScene.AddObject(&engines);
	mainScene.AddObject(&tableTop);
	mainScene.AddObject(&tableLeg1);
	mainScene.AddObject(&tableLeg2);
	mainScene.AddObject(&tableLeg3);
	mainScene.AddLight(&dl);
	mainScene.SetCamera(&mainCamera);

	//get reciprocal of image dimensions
	float invWidth = 1.f / (float)imageWidth;
	float invHeight = 1.f / (float)imageHeight;
	//for each vertical interval of the near plane
	for (int i = 0; i < imageHeight; ++i)
	{
		std::clog << "\rCurrently rendering scanline " << i + 1 << " of " << imageHeight << std::flush;
		//for each interval of the near plane horizontally
		for (int j = 0; j < imageWidth; j++)
		{
			ColourRGB rayColour(0.0f, 0.0f, 0.0f);
			for (int p = 0; p < raysPerPixel; p++)
			{
				//Calculate Screen space y location.
				float screenSpaceY = 1.0f - 2.0f * ((float)i + Random::RandFloat()) * invHeight;
				//Get the current pixel in screen space coordinates (in range -1 to 1).
				float screenSpaceX = 2.0f * ((float)j + Random::RandFloat()) * invWidth - 1.0f;
				Vector2 screenSpacePos = Vector2(screenSpaceX, screenSpaceY);
				Ray screenRay = mainScene.GetScreenRay(screenSpacePos);
				rayColour += mainScene.CastRay(screenRay, bounces);
			}
			rayColour = rayColour * (1.0f / (float)raysPerPixel);
			//Write to output stream.
			WriteColourRGB(std::cout, rayColour);
		}
		std::cout << std::endl;
	}

	//set the output stream buffer back to what it was previously
	std::cout.rdbuf(backup);
	return EXIT_SUCCESS;
}