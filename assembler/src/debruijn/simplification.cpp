//****************************************************************************
//* Copyright (c) 2011-2013 Saint-Petersburg Academic University
//* All Rights Reserved
//* See file LICENSE for details.
//****************************************************************************

#include "standard.hpp"
#include "graph_simplification.hpp"
#include "omni_labelers.hpp"
#include "io/single_read.hpp"

#include "simplification.hpp"

namespace debruijn_graph {

void Simplification::run(conj_graph_pack &gp) {
    using namespace omnigraph;

    total_labeler_graph_struct graph_struct(gp.g, &gp.int_ids, &gp.edge_pos);
    total_labeler labeler/*tot_lab*/(&graph_struct);

    detail_info_printer printer(gp, labeler, cfg::get().output_dir);
    printer(ipp_before_first_gap_closer);

    SimplifyGraph(gp, 0/*removal_handler_f*/, labeler, printer, 10
                  /*, etalon_paired_index*/);

    AvgCovereageCounter<Graph> cov_counter(gp.g);
    cfg::get_writable().ds.set_avg_coverage(cov_counter.Count());
}

#if 0
void corrected_and_save_reads(const conj_graph_pack& gp) {
    //saving corrected reads
    //todo read input files, correct, save and use on the next iteration

    auto_ptr<io::IReader<io::PairedReadSeq>> paired_stream =
            paired_binary_multireader(false, /*insert_size*/0);
    io::ModifyingWrapper<io::PairedReadSeq> refined_paired_stream(
        *paired_stream,
        GraphReadCorrectorInstance(gp.g, *MapperInstance(gp)));

    auto_ptr<io::IReader<io::SingleReadSeq>> single_stream =
            single_binary_multireader(false, /*include_paired_reads*/false);
    io::ModifyingWrapper<io::SingleReadSeq> refined_single_stream(
        *single_stream,
        GraphReadCorrectorInstance(gp.g, *MapperInstance(gp)));

    if (cfg::get().graph_read_corr.binary) {
        INFO("Correcting paired reads");

        io::BinaryWriter paired_converter(
            cfg::get().paired_read_prefix + "_cor", cfg::get().max_threads,
            cfg::get().buffer_size);
        paired_converter.ToBinary(refined_paired_stream);

        INFO("Correcting single reads");
        io::BinaryWriter single_converter(
            cfg::get().single_read_prefix + "_cor", cfg::get().max_threads,
            cfg::get().buffer_size);
        single_converter.ToBinary(refined_single_stream);
    } else {
        //save in fasta
        VERIFY(false);
    }

    INFO("Error correction done");
}

void correct_mismatches(conj_graph_pack &gp) {
    INFO("Correcting mismatches");
    auto_ptr<io::IReader<io::SingleReadSeq>> stream = single_binary_multireader(true, true);
    size_t corrected = MismatchShallNotPass<conj_graph_pack, io::SingleReadSeq>(gp, 2).StopAllMismatches(*stream, 1);
    INFO("Corrected " << corrected << " nucleotides");
}

void parallel_correct_mismatches(conj_graph_pack &gp) {
    INFO("Correcting mismatches");
    auto streams = single_binary_readers(true,  true);
    size_t corrected = MismatchShallNotPass<conj_graph_pack, io::SingleReadSeq>(gp, 2).ParallelStopAllMismatches(*streams, 1);
    INFO("Corrected " << corrected << " nucleotides");
}

void exec_simplification(conj_graph_pack& gp) {
    simplify_graph(gp);
    
    if (cfg::get().correct_mismatches)
    {
        parallel_correct_mismatches(gp);
        }
    save_simplification(gp);
    if (cfg::get().graph_read_corr.enable) {
        //			corrected_and_save_reads(gp);
    }
    
    } else {
        INFO("Loading Simplification");

        path::files_t used_files;
        load_simplification(gp, &used_files);
        link_files_by_prefix(used_files, cfg::get().output_saves);
        //		if (cfg::get().correct_mismatches) {
        //			parallel_correct_mismatches(gp);
        //		}
    }
}
#endif

} //debruijn_graph