/*
* MIT License
 *
 * Copyright (c) 2025 Turco Vale
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef HPLABRYNTH_H
#define HPLABRYNTH_H

/* colors for pretty output */
#define RedFG "\033[0;31m"
#define BoldGreenFG "\033[1;32m"
#define ResetColor "\033[0m"

#include <iostream>


namespace HPOpts {

    const auto HELP_TXT =   "Usage: HPLabrynth OPTIONS\n"
                            "       Check if a labyrinth in the correct file format is solvable or generate one\n"
                            "OPTIONS:\n"
                            "-p, --pretty                   make output colorful\n"
                            "                               not compatible with -o\n"
                            "-oFILENAME , --out=FILENAME   output directly in FILENAME\n"
                            "                               not compatible with -p\n"
                            "-s, --skip                     skip solvable check\n"
                            "                               only supported with -r in CMD mode\n"
                            "-cFILENAME , --check=FILENAME  check if FILENAME is solvable\n"
                            "                               not compatible with -o, -s or -r"
                            "-rPARAMS , --rand[=PARAMS]     generate random labyrinth\n"
                            "                               if PARAMS is provided enter CMD mode\n"
                            "                                   in this case PARAMS must be 6 whitespace-separated numbers\n"
                            "                                   indicating respectively height width starty startx endy endx\n"
                            "                               if PARAMS is not provided enter INPUT mode\n"
                            "                                   prompt the user for the same 6 numbers\n"
                            "-h , --help                    display this help and exit";

    enum Modes {
        HELP,
        CHECK,
        CHECK_PRETTY,
        RANDOM_CMD,
        RANDOM_SKIP,
        RANDOM_CMD_FILE,
        RANDOM_CMD_PRETTY,
        RANDOM_INPUT,
        RANDOM_INPUT_PRETTY,
        RANDOM_INPUT_FILE,
    };

    struct ModeOpts {
        Modes mode;
        std::string params;
        std::string filename;
    };

    Modes prettyfy(Modes m);
    Modes filefy(Modes m);

    void printHelp();
    void checkFile(std::string filename,bool pretty);


    void randomLabCMD(std::string param, bool pretty, bool skipCheck);
    void randomLabCMD(std::string param, std::string filename);

    void randomLabInp(bool pretty);
    void randomLabInp(std::string filename);


    ModeOpts getMode(int argc, char *argv[]);
}

namespace HPLabrynth {

    struct LabCoords {
        unsigned i;
        unsigned j;
    };
    std::ostream &operator<<(std::ostream &os, LabCoords &elem);
    bool operator==(const LabCoords &el1,const LabCoords &el2);
    std::ifstream &operator>>(std::ifstream &is, LabCoords &elem);

    struct LabDims {
        unsigned width;
        unsigned height;
    };
    std::ifstream &operator>>(std::ifstream & is, LabDims & dims);

    struct Set {
        LabCoords ref{};
        unsigned rank = 0;
        bool valid = true;
    };

    struct LabParams {
        LabDims dims;
        LabCoords start;
        LabCoords end;
        std::string lab;
    };
    std::ostream& operator<<(std::ostream& os, const LabParams &par);

    class HPLab{
        unsigned N;
        unsigned M;
        Set *lab;
        LabCoords start;
        LabCoords end;

    public:
        explicit HPLab(LabParams &params);
        HPLab(LabDims dims, LabCoords start, LabCoords end,bool make_valid=true);
        bool hasPath();
        void checkNeighbours(LabCoords elem);
        LabParams toParams(bool pretty = false);
        ~HPLab();

    private:
        void makeSet(LabCoords elem, bool is_valid);

        void link(LabCoords el1, LabCoords el2);

        LabCoords find(LabCoords el);

        void unite(const LabCoords &el1, const LabCoords &el2);
    };

    LabParams getLabParamsFromFile(std::string &filename);

}

#endif //HPLABRYNTH_H
