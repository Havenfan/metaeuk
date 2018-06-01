#include "Util.h"
#include "CommandCaller.h"
#include "Debug.h"
#include "FileUtil.h"
#include "LocalParameters.h"
#include "reduceredundancy.sh.h"

int reduceredundancy(int argc, const char **argv, const Command& command) {
    LocalParameters& par = LocalParameters::getLocalInstance();
    par.parseParameters(argc, argv, command, 2);

    CommandCaller cmd;
    if (par.removeTmpFiles) {
        cmd.addVariable("REMOVE_TMP", "TRUE");
    }

    // check if temp dir exists and if not, try to create it:
    if (FileUtil::directoryExists(par.db3.c_str()) == false){
        Debug(Debug::INFO) << "Temporary folder " << par.db3 << " does not exist or is not a directory.\n";
        if (FileUtil::makeDir(par.db3.c_str()) == false){
            Debug(Debug::WARNING) << "Could not crate tmp folder " << par.db3 << ".\n";
            EXIT(EXIT_FAILURE);
        } else {
            Debug(Debug::INFO) << "Created directory " << par.db3 << "\n";
        }
    }

    FileUtil::writeFile(par.db3 + "/reduceredundancy.sh", reduceredundancy_sh, reduceredundancy_sh_len);
    std::string program(par.db3 + "/reduceredundancy.sh");
    cmd.execProgram(program.c_str(), par.filenames);


    return EXIT_SUCCESS;
}