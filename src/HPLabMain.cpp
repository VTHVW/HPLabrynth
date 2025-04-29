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

#include "HPLabrynth.h"

int main(int argc, char *argv[]) {
    /*gets mode and optional parameter for that mode*/
    const auto mopts = HPOpts::getMode(argc, argv);

    std::string filename(mopts.filename);

    /*execute right operation*/
    switch(mopts.mode) {
        case HPOpts::CHECK: HPOpts::checkFile(filename,false); return 0;
        case HPOpts::CHECK_PRETTY: HPOpts::checkFile(filename,true); return 0;
        case HPOpts::RANDOM_CMD: HPOpts::randomLabCMD(mopts.params,false,false); return 0;
        case HPOpts::RANDOM_SKIP: HPOpts::randomLabCMD(mopts.params,false,true); return 0;
        case HPOpts::RANDOM_CMD_PRETTY: HPOpts::randomLabCMD(mopts.params,true,false); return 0;
        case HPOpts::RANDOM_CMD_FILE: HPOpts::randomLabCMD(mopts.params,filename); return 0;
        case HPOpts::RANDOM_INPUT: HPOpts::randomLabInp(false); return 0;
        case HPOpts::RANDOM_INPUT_PRETTY: HPOpts::randomLabInp(true); return 0;
        case HPOpts::RANDOM_INPUT_FILE: HPOpts::randomLabInp(filename); return 0;
        case HPOpts::HELP:
        default:
            HPOpts::printHelp();
            return 1;
    }
}
