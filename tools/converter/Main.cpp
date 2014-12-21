#include <fstream>
#include "tclap/CmdLine.h"
#include "NSG.h"
#include "SceneConverter.h"


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
        return "Has to be a valid file name";
    }

    std::string shortID() const
    {
        return "Output file";
    }

    bool check(const std::string& filename) const
    {
        std::ofstream file(filename);
        return file.is_open();
    }
};

int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    static const char* VERSION = "1.0";

    TCLAP::CmdLine cmd("NSG Converter", ' ', VERSION);

    IFileConstraint iConstraintFile;
    TCLAP::ValueArg<std::string> iArg("i", "input", "Input file to be converted", false, "", &iConstraintFile);

    OFileConstraint oConstraintFile;
    TCLAP::ValueArg<std::string> oArg("o", "output", "Output file", false, "", &oConstraintFile);

    cmd.add(iArg);
    cmd.add(oArg);

    cmd.parse(argc, argv);

    std::string inputFile = iArg.getValue();
    std::string outputFile = oArg.getValue();

    using namespace NSG;

	App app;
	auto window = app.GetOrCreateWindow("window", 0, 0, 1, 1);

    SceneConverter scene(inputFile);
    if (scene.Load())
        scene.Save(outputFile);
}