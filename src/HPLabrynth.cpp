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

#include <fstream>
#include <sstream>
#include <string>
#include "HPLabrynth.h"
#include <random>
#include <iostream>
#include <getopt.h>

namespace HPOpts {

    Modes prettyfy(Modes m) {
        /**
         * Just Add _PRETTY if it is a prettyfyable mode
         *
         * @m the mode to prettify
         * @return the mode prettifyed or itself
         */
        switch (m) {
            case CHECK: return CHECK_PRETTY;
            case RANDOM_CMD: return RANDOM_CMD_PRETTY;
            case RANDOM_INPUT: return RANDOM_INPUT_PRETTY;
            default: return m;
        }
    }

    Modes filefy(Modes m) {
        /**
          * Just Add _FILE if it is a "to file"able mode
          *
          * @m the mode to filefy
          * @return the mode filefyed or itself
          */
        switch (m) {
            case RANDOM_CMD: return RANDOM_CMD_FILE;
            case RANDOM_INPUT: return RANDOM_INPUT_FILE;
            default: return m;
        }
    }

    ModeOpts getMode(int argc, char *argv[]) {
        /**
          * Big Logic Handler for CMD options
           */
        int option_index = 0;
        const ModeOpts help_choice = {HELP,"",""};
        int asked_help=0,asked_pretty=0,asked_fout=0,asked_skip=0;

        const struct option long_options[] = {
            {"help",no_argument,nullptr,'h'},
            {"pretty",no_argument,nullptr,'p'},
            {"skip",no_argument,nullptr,'s'},
            {"out",required_argument,nullptr,'o'},
            {"check",required_argument,nullptr,'c'},
            {"rand",optional_argument,nullptr,'r'},
            {nullptr,0,nullptr,0}
        };

        Modes curr_mode= HELP;
        std::string params,ofn,cfn;

        if(argc <2) return help_choice;

        while(true) {

            int c = getopt_long(argc,argv,"hpso:c:r:",long_options,&option_index);
            if(c == -1) break;

            switch (c) {
                case 'p': asked_pretty = 1; break;
                case 's': asked_skip = 1; break;
                case 'o':
                    asked_fout = 1;
                    ofn = optarg;
                break;
                case 'c':
                    if (curr_mode == RANDOM_CMD || curr_mode == RANDOM_INPUT) {
                        asked_help = 1;
                    }else {
                        curr_mode=CHECK;
                        cfn = optarg;
                    }
                break;
                case 'r':
                    if (curr_mode == CHECK) {
                        asked_help = 1;
                    }else {
                        if (optarg) {
                            curr_mode=RANDOM_CMD;
                            params = optarg;
                        }else {
                            curr_mode=RANDOM_INPUT;
                        }
                    }
                break;
                case '?':
                default:
                case 'h': asked_help = 1;
            }
        }

        if (asked_help || curr_mode==HELP) return help_choice;
        if (curr_mode == CHECK) {
            if (asked_skip || asked_fout) return help_choice;
            if (cfn.empty()) return help_choice;
            if (asked_pretty) return {prettyfy(curr_mode),"",cfn};
            return {curr_mode,"",cfn};
        }
        if (curr_mode == RANDOM_CMD) {
            if (asked_skip) {
                if (asked_fout || asked_pretty || params.empty()) return help_choice;
                return {RANDOM_SKIP,params,""};
            }

            if (asked_fout && asked_pretty) return help_choice;
            if (asked_fout) return {filefy(curr_mode),params,ofn};
            if (asked_pretty) return {prettyfy(curr_mode),params,""};
            return {curr_mode,params,""};
        }
        if (curr_mode == RANDOM_INPUT) {
            if (asked_skip) return help_choice;
            if (asked_fout && asked_pretty) return help_choice;

            if (asked_fout) return {filefy(curr_mode),"",ofn};
            if (asked_pretty) return {prettyfy(curr_mode),"",""};
            return {curr_mode,params,""};
        }
        return help_choice;
    }

    void printHelp() {
        std::cout << HELP_TXT << std::endl;
    }

    void checkFile(std::string filename,bool pretty) {
        ::HPLabrynth::LabParams par = ::HPLabrynth::getLabParamsFromFile(filename);
        ::HPLabrynth::HPLab hpl(par);
        std::cout << hpl.toParams(pretty) << std::endl;
        std::cout << (hpl.hasPath() ? "Has a path!!" : "No path found") << std::endl;
    }


    void randomLabCMD(std::string param, bool pretty, bool skipCheck) {
        std::istringstream iss(param);
        ::HPLabrynth::LabParams par;
        iss >> par.dims.height >> par.dims.width
            >> par.start.i >> par.start.j
            >> par.end.i >> par.end.j;
        ::HPLabrynth::HPLab hpl(par.dims,par.start,par.end,!skipCheck);
        std::cout << hpl.toParams(pretty) << std::endl;
    }

    void randomLabCMD(std::string param, std::string filename) {

        std::ofstream os(filename);
        if (!os.is_open()) throw std::runtime_error("Can't open file");

        std::istringstream iss(param);
        ::HPLabrynth::LabParams par;
        iss >> par.dims.height >> par.dims.width
            >> par.start.i >> par.start.j
            >> par.end.i >> par.end.j;

        ::HPLabrynth::HPLab hpl(par.dims,par.start,par.end);
        os << hpl.toParams() << std::endl;
    }

    void randomLabInp(bool pretty) {
        ::HPLabrynth::LabParams par;
        std::cout << "dims [h w] : ";
        std::cin    >> par.dims.height
                    >> par.dims.width;
        std::cout << "\nstart [y x]: ";
        std::cin    >> par.start.i
                    >> par.start.j;
        std::cout << "\nend [y x]: ";
        std::cin    >> par.end.i
                    >> par.end.j;

        ::HPLabrynth::HPLab hpl(par.dims,par.start,par.end);
        std::cout << hpl.toParams(pretty) << std::endl;
    }
    void randomLabInp(std::string filename) {

        std::ofstream os(filename);
        if (!os.is_open()) throw std::runtime_error("Can't open file");

        ::HPLabrynth::LabParams par;
        std::cout << "dims [h w] : ";
        std::cin    >> par.dims.height
                    >> par.dims.width;
        std::cout << "\nstart [y x]: ";
        std::cin    >> par.start.i
                    >> par.start.j;
        std::cout << "\nend [y x]: ";
        std::cin    >> par.end.i
                    >> par.end.j;

        ::HPLabrynth::HPLab hpl(par.dims,par.start,par.end);
        os << hpl.toParams() << std::endl;
    }

}

namespace HPLabrynth {

    std::ostream &operator<<(std::ostream &os, const LabCoords &elem) {
        return os << elem.i << " " << elem.j;
    }

    bool operator==(const LabCoords &el1, const LabCoords &el2) {
        return el1.i == el2.i && el1.j == el2.j;
    }

    std::ifstream &operator>>(std::ifstream &is, LabCoords &elem) {
        is >> elem.i >> elem.j;
        return is;
    }

    std::ifstream &operator>>(std::ifstream & is, LabDims & dims) {
        is >> dims.height >> dims.width;
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const LabParams &par){
        os << par.dims.height << " " << par.dims.width << std::endl;
        os << par.start << std::endl;
        os << par.end << std::endl;
        os << par.lab;
        return os;
    }

    HPLab::HPLab(LabParams &params) {
        N = params.dims.width;
        M = params.dims.height;
        start = params.start;
        start.i--;
        start.j--;
        end = params.end;
        end.i--;
        end.j--;
        lab = new Set[N * M];

        std::istringstream iss(params.lab);

        for (unsigned i = 0; i < M ; i++) {
            std::string line;
            std::getline(iss, line, '\n');
            for (unsigned j = 0; j < N; j++) {
                makeSet({i,j},line.at(j) != 'x');
            }
        }
    }

    HPLab::HPLab(LabDims dims, LabCoords start, LabCoords end,bool make_valid) {
        N = dims.width;
        M = dims.height;
        this->start = start;
        this->start.i--;
        this->start.j--;
        this->end = end;
        this->end.i--;
        this->end.j--;
        lab = new Set[N * M];

        if (start == end ||
            N<this->start.j || N<this->end.j ||
            M<this->start.i || M<this->end.i) {
            throw std::invalid_argument("HPLab::HPLab: invalid coordonates");
        }

        std::random_device rd;
        if (rd.entropy() > 0) {
            std::srand(rd());
        }else {
            std::srand(time(NULL));
        }

        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                LabCoords cur{i,j};
                makeSet(cur,(std::rand() < RAND_MAX/2) || cur == this->end || cur == this->start);
            }
        }

        if (make_valid) {
            while (!hasPath()) {
                unsigned ind;
                do {
                    ind = std::rand() % (N*M);
                }while (lab[ind].valid);
                lab[ind].valid = true;
                checkNeighbours({(ind - (ind%N))/N,ind%N});
            }
        }
    }

    bool HPLab::hasPath() {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                checkNeighbours({i,j});
            }
        }

        return find(start) == find(end);
    }

    void HPLab::checkNeighbours(LabCoords elem) {
        if (elem.j > 0)
            unite(elem,{elem.i,elem.j-1});
        if (elem.j < N-1)
            unite(elem,{elem.i,elem.j+1});
        if (elem.i > 0)
            unite(elem,{elem.i-1,elem.j});
        if (elem.i < M-1)
            unite(elem,{elem.i+1,elem.j});
    }

    LabParams HPLab::toParams(bool pretty) {
        LabCoords ext_start{start.i+1,start.j+1}, ext_end{end.i+1,end.j+1};
        std::string hWall = "";
        if (pretty) {
            for (unsigned i = 0; i < N; i++) {
                hWall.append("▒");
            }
            hWall = RedFG "▒" + hWall + "▒\n" ResetColor;
        }
        std::string lab_s = pretty? hWall : "";
        for (unsigned i = 0; i < M; i++) {
            if (pretty) lab_s.append(RedFG "▒" ResetColor);
            for (unsigned j = 0; j < N; j++) {
                auto path = " ";
                auto wall = "x";
                LabCoords cur{i,j};
                if (pretty){
                    wall = RedFG "▒" ResetColor;
                    if (start == cur)
                        path = BoldGreenFG "S" ResetColor;
                    else if (end == cur)
                        path = BoldGreenFG "E" ResetColor;
                }
                lab_s.append(lab[i*N + j].valid? path : wall);
                lab_s.shrink_to_fit();
            }
            lab_s.append(pretty? RedFG "▒\n" ResetColor: "\n");
        }
        if (pretty) lab_s.append(hWall);
        LabParams par{{N,M},ext_start,ext_end,lab_s};
        return par;
    }

    HPLab::~HPLab() {
        delete[] lab;
    }

    void HPLab::makeSet(LabCoords elem, const bool is_valid) {
        Set &S = lab[elem.i*N + elem.j];
        S.valid = is_valid;
        S.ref = elem;
    }

    void HPLab::link(LabCoords el1, LabCoords el2) {
        Set &S1 = this->lab[el1.i*N + el1.j];
        Set &S2 = this->lab[el2.i*N + el2.j];

        if (S1.valid && S2.valid) {
            if (S1.rank > S2.rank) {
                S2.ref = el1;
            } else {
                S1.ref = el2;
                if (S2.rank == S1.rank) {
                    S2.rank += 1;
                }
            }
        }
    }

    LabCoords HPLab::find(LabCoords el) {
        Set &S = this->lab[el.i*N + el.j];

        if (el != S.ref) {
            S.ref = find(S.ref);
        }

        return S.ref;
    }

    void HPLab::unite(const LabCoords &el1, const LabCoords &el2) {
        const LabCoords ref1 = find(el1);
        const LabCoords ref2 = find(el2);
        if (ref1 != ref2)
            link(ref1, ref2);
    }

    LabParams getLabParamsFromFile(std::string &filename) {
        std::ifstream file(filename);
        LabParams labParams;

        if (!file.is_open()) {
            throw std::runtime_error("File could not be opened");
        }

        std::string line;
        file >> labParams.dims
             >> labParams.start
             >> labParams.end;

        std::getline(file, line);

        for (unsigned i = 0; i < labParams.dims.height; i++) {
            std::getline(file, line);
            labParams.lab += line + '\n';
        }

        file.close();

        return labParams;
    }

}
