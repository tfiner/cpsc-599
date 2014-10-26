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
#include "function.h"
#include "noise_factory.h"

#include <iostream>
#include <fstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "optionparser.h"


using namespace std;


namespace {


    const double NANOS_PER_SECOND = 1000000000.0;
    const char* VERSION_STR = "Glow 0.6.0";


    option::ArgStatus Required(const option::Option& option, bool) {
        return option.arg == 0 ? option::ARG_ILLEGAL : option::ARG_OK;
    }

    enum optionIndex { 
        UNKNOWN, 
        HELP, 
        INPUT, 
        OUTPUT, 
        DEPTH, 
        VERBOSE, 
        VERSION, 
        PARSE, 
        PREVIEW,
        IMG_AREA
    };

    const option::Descriptor usage[] = {
        {UNKNOWN,   0,    "" ,    ""    ,         option::Arg::None,     "USAGE: example [options]\n\nOptions:" },
        {HELP,      0,    "h" ,   "help",         option::Arg::None,     "  --help,      -h  \tPrint usage and exit." },
        {INPUT,     0,    "i",    "input",        Required,              "  --input,     -i  \tThe file name of the input file scene (required)." },
        {OUTPUT,    0,    "o",    "output",       Required,              "  --output,    -o  \tThe file name of the rendering (required)." },
        {DEPTH,     0,    "d",    "depth",        Required,              "  --depth,     -d  \tThe file name of a depth image (optional)." },
        {VERBOSE,   0,    "v",    "verbose",      option::Arg::None,     "  --verbose,   -v  \tIncrease verbosity count (default: 0)." },
        {VERSION,   0,    "",     "version",      option::Arg::None,     "  --version,       \tDisplays the version information." },
        {PARSE,     0,    "p",    "parseOnly",    option::Arg::None,     "  --parseOnly, -p  \tJust parse the scene file, don't render anything." },
        {PREVIEW,   0,    "",     "preview",      Required,              "  --preview,       \tThe directory to write previews of the noise library tree." },    
        {IMG_AREA,  0,    "",     "imageArea",    Required,              "  --imageArea,     \tX0,X1,Y0,Y1 separated by commas (e.g. '--imgArea 256,512,0,256')." },

        {UNKNOWN, 0,    "" ,    "",             option::Arg::None,     "\nExamples:\n  glow --input scene.txt --output scene.png" },
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

        if ( options[PREVIEW] && !options[PREVIEW].arg ) {
            cerr << "Required argument for preview not found.\n";
            valid = false;
        }

        if ( !options[PARSE] && !options[PREVIEW] && 
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

    int CommandLineRun(int argc, char** argv) {
        argc-=(argc>0); 
        argv+=(argc>0); // skip program name argv[0] if present

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

        using namespace boost::posix_time;

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

        if ( options[PREVIEW] ) {
            auto const & objs   = scene->GetObjects();
            for(auto const & o : objs){
                if (auto const func = dynamic_cast<Function*>(o.get())) {
                    WritePreviewBmps(func->GetModules(), options[PREVIEW].arg);
                    break;
                }
            }
            return 0;
        }

        if ( options[IMG_AREA] ) {
            std::stringstream ss;
            ss << options[IMG_AREA].arg;

            auto x0 = 0;
            auto x1 = 0;
            auto y0 = 0;
            auto y1 = 0;
            auto dummy = ' ';

            if (!(ss >> x0 >> dummy >> x1 >> dummy >> y0 >> dummy >> y1)) {
                LOG_MSG(0, "Couldn't parse image area comma delimited string: " 
                    << "'" << options[IMG_AREA].arg << "'");
                return -1;
            }

            auto & vp = scene->GetViewPlane();

            LOG_MSG(1, "Setting image area to: "
                << "x0: " << x0 << " "
                << "x1: " << x1 << " "
                << "y0: " << y0 << " "
                << "y1: " << y1
            );

            vp->SetImgAreaPixels(x0, x1, y0, y1);
        }


        auto const & vp = scene->GetViewPlane();
        auto const px = vp->GetWidthPixels();
        auto const py = vp->GetHeightPixels();
        if (options[VERBOSE]) {
            cout << "Writing (" << px << ", " << py << ") color image to " << options[OUTPUT].arg << "\n";

            if (options[DEPTH] && options[DEPTH].arg)
                cout << "Writing (" << px << ", " << py << ") depth image to " << options[DEPTH].arg << "\n";    
        }

        {
            LOG_MSG(1, "Setting recorder PNG");
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
        
        LOG_MSG(1, "Begin render...");
        auto renderBegin = microsec_clock::local_time().time_of_day();
        scene->GetCamera()->Render( *scene );
        auto renderEnd = microsec_clock::local_time().time_of_day();

        auto renderDuration = (renderEnd - renderBegin).total_nanoseconds() / NANOS_PER_SECOND;
        LOG_MSG(1, "Time to render scene: " << renderDuration << "s");

        return 0;
    }


    int Run(const std::vector<std::string>& args) {
        // While we are in this function, the pointers will be valid.
        std::vector<char*> argv;
        std::transform(args.cbegin(), args.cend(), std::back_inserter(argv), 
            [](const std::string& s){ return const_cast<char*>(s.c_str()); }
        );
        auto argc = argv.size();
        return CommandLineRun(argc, argv.data());
    }    


} // glow
