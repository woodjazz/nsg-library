/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "NSG.h"
#include "TrueTypeConverter.h"
#include "tclap/CmdLine.h"
#include <cstdint>
#include <fstream>
using namespace NSG;
class IFileConstraint : public TCLAP::Constraint<std::string> {
public:
    IFileConstraint() {}

    std::string description() const {
        return "Has to be a valid file (full path)";
    }

    std::string shortID() const { return "Input file"; }

    bool check(const std::string& filename) const {
        std::ifstream file(filename, std::ios::binary);
        return file.is_open();
    }
};

class OFileConstraint : public TCLAP::Constraint<std::string> {
public:
    OFileConstraint() {}

    std::string description() const { return "Has to be a valid directory"; }

    std::string shortID() const { return "Output directory"; }

    bool check(const std::string& filename) const {
        Path path(filename);
        bool hasExtension = path.HasExtension();
        std::ofstream file(path.GetFullAbsoluteFilePath());
        return !hasExtension && !file.is_open();
    }
};

class BitmapPixelsConstraint : public TCLAP::Constraint<int> {
public:
    BitmapPixelsConstraint(){};

    std::string description() const {
        stringstream ss;

        ss << "Must be equal or greater than 32";

        return ss.str();
    }

    std::string shortID() const { return "Bitmap pixels (width or height)"; }

    bool check(const int& pixels) const { return pixels >= 32; }
};

class FontPixelsConstraint : public TCLAP::Constraint<int> {
public:
    FontPixelsConstraint(){};

    std::string description() const {
        stringstream ss;

        ss << "Must be equal or greater than 8";

        return ss.str();
    }

    std::string shortID() const { return "Font pixels height"; }

    bool check(const int& pixels) const { return pixels >= 8; }
};

class CharacterConstraint : public TCLAP::Constraint<int> {
public:
    CharacterConstraint(){};

    std::string description() const {
        stringstream ss;

        ss << "Must be greater than 31";

        return ss.str();
    }

    std::string shortID() const { return "Character"; }

    bool check(const int& ch) const { return ch > 31; }
};

int NSG_MAIN(int argc, char* argv[]) {
    using namespace NSG;

    try {
        static const char* VERSION = "1.0";

        TCLAP::CmdLine cmd("NSG Converter", ' ', VERSION);
        cmd.setExceptionHandling(false);

        IFileConstraint iConstraintFile;
        TCLAP::ValueArg<std::string> iArg("i", "input",
                                          "Input file to be converted", false,
                                          "", &iConstraintFile);

        OFileConstraint oConstraintFile;
        TCLAP::ValueArg<std::string> oArg("o", "output", "Output directory",
                                          false, "", &oConstraintFile);

        BitmapPixelsConstraint pixelsConstraint;
        TCLAP::ValueArg<int> wArg("x", "width",
                                  "Bitmap width. By default is 512.", false,
                                  512, &pixelsConstraint);

        TCLAP::ValueArg<int> hArg("y", "height",
                                  "Bitmap height. By default is 512.", false,
                                  512, &pixelsConstraint);

        FontPixelsConstraint fontPixelsConstraint;
        TCLAP::ValueArg<int> fArg("f", "fheight",
                                  "Font height in pixels. By default is 32.",
                                  false, 32, &fontPixelsConstraint);

        CharacterConstraint charConstraint;
        TCLAP::ValueArg<int> sArg(
            "s", "sChar", "Starting character to bake. By default is 32.",
            false, 32, &charConstraint);
        TCLAP::ValueArg<int> eArg("e", "eChar",
                                  "Final character to bake. By default is 127.",
                                  false, 127, &charConstraint);

        TCLAP::SwitchArg zArg("z", "compress", "Compress the file.", false);

        cmd.add(iArg);
        cmd.add(oArg);
        cmd.add(wArg);
        cmd.add(hArg);
        cmd.add(fArg);
        cmd.add(sArg);
        cmd.add(eArg);
        cmd.add(zArg);

        cmd.parse(argc, argv);

        using namespace NSG;

        Path inputFile(iArg.getValue());
        Path outputDir;
        outputDir.SetPath(oArg.getValue());

        if (outputDir.HasPath() && inputFile.HasExtension()) {
            if (Path::GetLowercaseFileExtension(inputFile.GetFilename()) ==
                "ttf") {
                int fontPixelsHeight = fArg.getValue();
                int bitmapWidth = wArg.getValue();
                int bitmapHeight = hArg.getValue();
                int sChar = sArg.getValue();
                int eChar = eArg.getValue();
                TrueTypeConverter obj(inputFile, sChar, eChar, fontPixelsHeight,
                                      bitmapWidth, bitmapHeight);
                obj.Load();
                CHECK_CONDITION(obj.Save(outputDir, zArg.getValue()));
            } else {
                LOGE("Cannot convert file. Unknown file extension");
                return -1;
            }
        }
        return 0;
    } catch (TCLAP::ArgException& e) {
        std::cerr << endl
                  << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
    } catch (std::exception& e) {
        const char* pWhat = "*** UNKNOWN EXCEPTION (1) ***";

        if (!string(e.what()).empty())
            pWhat = e.what();

        std::cerr << endl << "error: " << pWhat << std::endl;
    } catch (TCLAP::ExitException& e) {
        return e.getExitStatus();
    } catch (...) {
        std::cerr << endl << "*** UNKNOWN EXCEPTION (2) *** " << endl;
    }

    return -1;
}
