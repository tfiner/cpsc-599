// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#include "factory.h"
#include "scene_loader.h"
#include "memento.h"
#include "camera.h"
#include "scene.h"
#include "recorder_png.h"
#include "view_plane.h"
#include "log_msg.h"
#include "direction_light.h"

#include <iostream>
#include <fstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "optionparser.h"


using namespace std;


namespace {


    const double NANOS_PER_SECOND = 1000000000.0;
    const char* VERSION_STR = "Glow 0.5.0";


    option::ArgStatus Required(const option::Option& option, bool) {
        return option.arg == 0 ? option::ARG_ILLEGAL : option::ARG_OK;
    }

    enum  optionIndex { UNKNOWN, HELP, INPUT, OUTPUT, DEPTH, VERBOSE, VERSION, PARSE };
    const option::Descriptor usage[] = {
        {UNKNOWN, 0,    "" ,    ""    ,         option::Arg::None,     "USAGE: example [options]\n\nOptions:" },
        {HELP,    0,    "h" ,   "help",         option::Arg::None,     "  --help,      -h  \tPrint usage and exit." },
        {INPUT,   0,    "i",    "input",        Required,              "  --input,     -i  \tThe file name of the input file scene (required)." },
        {OUTPUT,  0,    "o",    "output",       Required,              "  --output,    -o  \tThe file name of the rendering (required)." },
        {DEPTH,   0,    "d",    "depth",        Required,              "  --depth,     -d  \tThe file name of a depth image (optional)." },
        {VERBOSE, 0,    "v",    "verbose",      option::Arg::None,     "  --verbose,   -v  \tIncrease verbosity count (default: 0)." },
        {VERSION, 0,    "",     "version",      option::Arg::None,     "  --version,       \tDisplays the version information." },
        {PARSE,   0,    "p",    "parseOnly",    option::Arg::None,     "  --parseOnly, -p  \tJust parse the scene file, don't render anything." },
        {UNKNOWN, 0,    "" ,    ""   ,          option::Arg::None,     "\nExamples:\n"
                                                 "  glow --input scene.txt --output scene.png" },
        {0,0,0,0,0,0}
    };
    auto const NUM_USAGE = sizeof(usage) / sizeof(usage[0]);

    
} // namespace {


namespace glow {


    std::string ReadScene(const char* fileName) {
        ifstream ifs(fileName);
        string line, scene;
        while( getline(ifs,line) )
            scene += line + "\n";
        return scene;
    }


    bool ValidateOptions(option::Option* options, option::Parser& parse) {
        for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
            cerr << "Unknown option: " << opt->name << "\n";

        for (int i = 0; i < parse.nonOptionsCount(); ++i)
            cerr << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";     

        bool valid = true;

        if ( !options[INPUT] || !options[INPUT].arg ) {
            cerr << "Required argument for input not found.\n";
            valid = false;
        }

        if ( !options[PARSE] && 
            (!options[OUTPUT] || !options[OUTPUT].arg) ) {
            cerr << "Required argument for output not found.\n";
            valid = false;
        }

        return valid;
    }


    int LoadParseScene(SceneParams& sp, std::string& sceneText, const option::Option* options){
        if (options[VERBOSE])
            cout << "Loading scene: " << options[INPUT].arg << "\n";

        sceneText = ReadScene( options[INPUT].arg );
        if ( sceneText.empty() ) {
            cerr << "Problem reading " << options[INPUT].arg << "\n";
            return -1;
        }

        try{
            if (options[VERBOSE])
                cout << "Parsing scene.\n";

            sp = LoadScene(sceneText);

            if (options[VERBOSE])
                cout << sp;
            
            if (options[PARSE])
                return 0;

        } catch(std::exception& e) {
            cerr << "Caught an error: " << e.what() << "\n";
            return -2;
        }

        return 0;
    }


    int run(int argc, char** argv) {
        using namespace boost::posix_time;
        argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
        option::Stats  stats(usage, argc, argv);
        option::Option options[NUM_USAGE], buffer[NUM_USAGE];
        option::Parser parse(usage, argc, argv, options, buffer);

        logVerbosity = options[VERBOSE].count();

        if (options[HELP] || argc == 0) {
            option::printUsage(std::cout, usage);
            return 0;
        }

        if (options[VERSION]){
            cout << VERSION_STR << "\n";
            return 0;
        }

        if ( !ValidateOptions(options, parse) ) {
            option::printUsage(std::cout, usage);
            return -1;
        }

        if (parse.error()) {
            cerr << "Missing or invalid arguments.\n\n";
            option::printUsage(std::cout, usage);
            return -1;
        }

        SceneParams sp;
        std::string sceneText;
        auto parseBegin = microsec_clock::local_time().time_of_day();

        auto loadedParsed = LoadParseScene(sp, sceneText, options);
        if (0 != loadedParsed)
            return loadedParsed;
        
        auto parseEnd = microsec_clock::local_time().time_of_day();
        auto parseDuration = (parseEnd - parseBegin).total_nanoseconds() / NANOS_PER_SECOND;
        LOG_MSG(1, "Time to load and parse scene: " << parseDuration << "s");

        auto scene = MakeScene( sp );    
        if ( options[PARSE] )
            return 0;

        auto & vp = scene->GetViewPlane();
        auto const px = vp->GetWidthPixels();
        auto const py = vp->GetHeightPixels();
        if (options[VERBOSE]) {
            cout << "Writing (" << px << ", " << py << ") color image to " << options[OUTPUT].arg << "\n";

            if (options[DEPTH] && options[DEPTH].arg)
                cout << "Writing (" << px << ", " << py << ") depth image to " << options[DEPTH].arg << "\n";    
        }

        {
            auto colorRecorder = RecorderPtr( 
                new RecorderPng(options[OUTPUT].arg, px, py, 8, sceneText.c_str()) 
            );
            scene->SetColorRecorder( colorRecorder );
        }

        if (options[DEPTH] && options[DEPTH].arg) {
            auto depthRecorder = RecorderPtr( 
                new RecorderPng(options[DEPTH].arg, px, py, 8, sceneText.c_str()) 
            );
            scene->SetDepthRecorder( depthRecorder );
        }
        
        auto renderBegin = microsec_clock::local_time().time_of_day();
        scene->GetCamera()->Render( *scene );
        auto renderEnd = microsec_clock::local_time().time_of_day();

        auto renderDuration = (renderEnd - renderBegin).total_nanoseconds() / NANOS_PER_SECOND;
        LOG_MSG(1, "Time to render scene: " << renderDuration << "s");

        return 0;
    }

} // glow
