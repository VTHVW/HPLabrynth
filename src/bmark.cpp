#include <benchmark/benchmark.h>
#include "HPLabrynth.h"
#include <math.h>

static void BM_HPGen(benchmark::State& state){

    unsigned size=state.range(0);
    HPLabrynth::LabParams params;

    for (auto _ : state) {
        params.dims = {size,size};
        params.start = {1,1};
        params.end = {size,size};
        HPLabrynth::HPLab hpl(params.dims, params.start,params.end,true);
        state.SetComplexityN(state.range(0));
    }
}

static void BM_HPCheck(benchmark::State& state){

    unsigned size=state.range(0);
    HPLabrynth::LabParams params;

    for (auto _ : state) {
        params.dims = {size,size};
        params.start = {1,1};
        params.end = {size,size};
        state.PauseTiming();
        HPLabrynth::HPLab hpl(params.dims, params.start,params.end,false);
        state.ResumeTiming();
        hpl.hasPath();
        state.SetComplexityN(state.range(0));
    }
}

BENCHMARK(BM_HPCheck)->RangeMultiplier(2)->Range(4,1028)->Complexity();
BENCHMARK(BM_HPGen)->RangeMultiplier(2)->Range(2,64)->Complexity(benchmark::oN)->Complexity([](benchmark::IterationCount n)->double{return std::pow(n,4);});

BENCHMARK_MAIN();