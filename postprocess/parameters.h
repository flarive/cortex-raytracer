#pragma once

#include "effects.h"
#include "effects/effect.h"


#include <sstream>
#include <filesystem>

using namespace std;

class parameters
{
public:

	bool quietMode = false;

	string inputpath;
	string outputpath;

	short fx_index;
	pmap fx_args;

	static parameters getArgs(int argc, char* argv[])
	{
		parameters params;

		int loop = 2;
		for (int count = 0; count < argc; count++)
		{
			string arg = argv[count];

			if (arg.starts_with("-"))
			{
				string param = arg.substr(1);
				string value = argv[count + 1];

				if (param == "quiet")
				{
					params.quietMode = true;
				}
				else if (param == "input")
				{
					std::filesystem::path dir(std::filesystem::current_path());
					std::filesystem::path file(value.c_str());
					std::filesystem::path fullexternalProgramPath = dir / file;

					auto fullAbsPath = std::filesystem::absolute(fullexternalProgramPath);
					params.inputpath = fullAbsPath.string();
				}
				else if (param == "output")
				{
					std::filesystem::path dir(std::filesystem::current_path());
					std::filesystem::path file(value.c_str());
					std::filesystem::path fullexternalProgramPath = dir / file;

					auto fullAbsPath = std::filesystem::absolute(fullexternalProgramPath);
					params.outputpath = fullAbsPath.string();
				}
				else if (param == "effect")
				{
					params.fx_index = stoi(value, 0, 10);

					if (params.fx_index == 1)
					{
						for (int scount = loop; scount < argc; scount++)
						{
							string arg = argv[scount];

							if (arg.starts_with("-"))
							{
								string param = arg.substr(1);
								string value = argv[scount + 1];

								if (param == "threshold")
									params.fx_args.emplace(param, stof(value));
								else if (param == "radius")
									params.fx_args.emplace(param, stof(value));
							}
						}
					}
					else if (params.fx_index == 2)
						params.fx_index = pp_effect::glow;
					else if (params.fx_index == 3)
						params.fx_index = pp_effect::csb;
					else if (params.fx_index == 4)
						params.fx_index = pp_effect::steinberg;
					else
						params.fx_index = pp_effect::none;
				}
			}

			loop++;
		}

		return params;
	}

	//static pmap getEffectArgs()
	//{
	//	// TO DO
	//}
};


