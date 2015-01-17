/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "tclap/CmdLine.h"
#include "NSG.h"
#include "BlenderDNA.h"
#include <fstream>

class IFileConstraint : public TCLAP::Constraint<std::string>
{
public:
    IFileConstraint () {}

    std::string description() const
    {
        return "Has to be a valid file (full path)";
    }

    std::string shortID() const
    {
        return "Input file";
    }

    bool check(const std::string& filename) const
    {
        std::ifstream file(filename, std::ios::binary);
        return file.is_open();
    }
};

class OFileConstraint : public TCLAP::Constraint<std::string>
{
public:
    OFileConstraint () {}

    std::string description() const
    {
        return "Has to be a valid directory";
    }

    std::string shortID() const
    {
        return "Output directory";
    }

    bool check(const std::string& filename) const
    {
        Path path(filename);
        bool hasExtension = path.HasExtension();
        std::ofstream file(path.GetFullAbsoluteFilePath());
        return !hasExtension && !file.is_open();
    }
};


int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    try
    {
        static const char* VERSION = "1.0";

        TCLAP::CmdLine cmd("Blender generator", ' ', VERSION);

        IFileConstraint iConstraintFile;
        TCLAP::ValueArg<std::string> iArg("i", "input", "Input file", true, "", &iConstraintFile);

        OFileConstraint oConstraintFile;
		TCLAP::ValueArg<std::string> oArg("o", "output", "Output directory", true, "", &oConstraintFile);

        cmd.add(iArg);
        cmd.add(oArg);

        cmd.parse(argc, argv);

        using namespace NSG;

        Path inputFile(iArg.getValue());
        Path outputDir;
        outputDir.SetPath(oArg.getValue());

        return Blender::GenerateDNA(inputFile, outputDir);
    }
    catch (TCLAP::ArgException& e)
    {
        std::cerr << endl << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    catch (std::exception& e)
    {
        const char* pWhat = "*** UNKNOWN EXCEPTION (1) ***";

        if (!string(e.what()).empty())
            pWhat = e.what();

        std::cerr << endl << "error: " << pWhat << std::endl;
    }
    catch (...)
    {
        std::cerr << endl << "*** UNKNOWN EXCEPTION (2) *** " << endl;
    }

    return -1;

}
