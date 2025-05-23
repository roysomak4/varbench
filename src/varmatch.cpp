#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include "gzstream.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include "fmt/format.h"
#include "strfunc.h"

std::unordered_map<std::string, std::string> load_variant_signatures(const std::string &vcf_file) {
    std::unordered_map<std::string, std::string> variant_signatures;

    // get file stream handler
    igzstream in_vcf(vcf_file.c_str());

    // buffer to hold content of one line of the VCF file
    std::string line;

    // Loop through VCF records
    while(getline(in_vcf, line)) {
        // exclude the VCF header
        if (line.find_first_of('#') == std::string::npos || (line.find_first_of('#') != std::string::npos && line.find_first_of('#') != 0)) {
            std::vector<std::string> tmparr = split(line, '\t');
            std::string variant_signature = fmt::format("{0}:{1}:{2}:{3}", tmparr[0], tmparr[1], tmparr[3], tmparr[4]); // chrom:pos:ref:alt
            std::vector<std::string> info_fields = split(tmparr[7], ';');
            std::string hiaf = ".";
            std::string gene = ".";
            std::string hicov = ".";
            std::string cov = ".";
            std::string vaf = ".";
            std::string hgvs_c = ".";
            std::string hgvs_p = ".";
            std::string tx = ".";


            for (auto &field : info_fields) {
                if (field.find("HIAF=") != std::string::npos) {
                    hiaf = std::to_string(int(std::stof(split(field, '=')[1]) * 100));
                }
                if (field.find("GENE=") != std::string::npos) {
                    gene = split(field, '=')[1];
                }
                if (field.find("HICOV=") != std::string::npos) {
                    hicov = split(field, '=')[1];
                }
                if (field.find("DP=") != std::string::npos) {
                    cov = split(field, '=')[1];
                }
                if (field.rfind("AF=", 0) == 0) {
                    vaf = std::to_string(int(std::stof(split(field, '=')[1]) * 100));
                }
                if (field.find("HGVS_C=") != std::string::npos) {
                    std::string hgvs_c_tx = split(field, '=')[1];
                    if (hgvs_c_tx.find(":") != std::string::npos) {
                        hgvs_c = split(hgvs_c_tx, ':')[1];
                    } else {
                        hgvs_c = hgvs_c_tx;
                    }
                }
                if (field.find("HGVS_P=") != std::string::npos) {
                    std::string hgvs_p_tx = split(field, '=')[1];
                    if (hgvs_p_tx.find(":") != std::string::npos) {
                        hgvs_p = split(hgvs_p_tx, ':')[1];
                    } else {
                    hgvs_p = hgvs_p_tx;
                    }
                }
                if (field.find("TRANSCRIPT=") != std::string::npos) {
                    tx = split(field, '=')[1];
                }
            }
            std::string var_info = fmt::format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}", vaf, cov, hiaf, hicov, gene, hgvs_c, hgvs_p, tx); 
            variant_signatures.insert({variant_signature, var_info});
        }
    }
    in_vcf.close();
    return variant_signatures;
}

int main(int argc, char **argv) {
    // Entry point for the app
    // Accepts 2 CLI arguments: Benchmark VCF file and Query VCF file
    // ./varbench <benchmark.vcf> <query.vcf>
    if (argc < 4) {
        fmt::print("Missing arguments.\n");
        fmt::print("CLI command: ./varbench <benchmark.vcf> <query.vcf> <comparison_result.txt>\n");
        exit(1);
    }
    std::string bench_vcf_file = argv[1];
    std::string query_vcf_file = argv[2];
    std::string result_file = argv[3];

    fmt::print("Loading benchmark VCF file...\n");
    std::unordered_map<std::string, std::string> bench_variants = load_variant_signatures(bench_vcf_file);
    fmt::print("Loading query VCF file...\n");
    std::unordered_map<std::string, std::string> query_variants = load_variant_signatures(query_vcf_file);

    // buffer for holding variant categories
    std::set<std::string> tp_variants;
    std::set<std::string> fp_variants;
    std::set<std::string> fn_variants;
    
    // identify true positive and false positive variants
    fmt::print("Identifying true and false positive variants...\n");
    for (auto &variant : query_variants) {
        if (bench_variants.count(variant.first) > 0) {
            // true positive variant
            std::string expected_vaf = split(bench_variants[variant.first], '\t')[0];
            std::string expected_hiaf = split(bench_variants[variant.first], '\t')[0];

            tp_variants.insert(fmt::format("{0}\t{1}\t{2}", variant.first, bench_variants[variant.first], variant.second));
        } else {
            // false positive variant
            fp_variants.insert(fmt::format("{0}\t{1}\t{2}", variant.first, ".\t.\t.\t.\t.\t.\t.\t.", variant.second));
        }
    }
    // calculate number of tp and fp variants
    int tp = tp_variants.size();
    int fp = fp_variants.size();

    // identify false negative variants
    fmt::print("Identifying false negative variants...\n");
    for (auto &variant : bench_variants) {
        if (query_variants.count(variant.first) == 0) {
            // false negative variant
            fn_variants.insert(fmt::format("{0}\t{1}\t{2}", variant.first, variant.second, ".\t.\t.\t.\t.\t.\t.\t."));
        }
    }
    // calculate fn variants
    int fn = fn_variants.size();

    // statistical variables
    fmt::print("Computing statistics...\n");
    int total_variants = tp + fn;
    float precision = (float)tp / ((float)tp + (float)fp);
    float recall = (float)tp / ((float)tp + (float)fn);
    float f1_score = (2 * precision * recall) / (precision + recall);

    // write output to file
    fmt::print("Generating results...\n");
    // std::string outfile = "varbench_result.txt";

    std::ofstream outf (result_file.c_str(), std::ios::out | std::ios::trunc);
    outf << "# VCF comparision results\n";
    outf << fmt::format("# Benchmark VCF: {0}\n", bench_vcf_file);
    outf << fmt::format("# Query VCF: {0}\n", query_vcf_file);
    outf << "\n";
    outf << "Confusion matrix values\n";
    outf << fmt::format("Total variants: {0}\n", total_variants);
    outf << fmt::format("True positives: {} ({:.{}f}%)\n", tp, ((float)tp/(float)total_variants) * 100, 1);
    outf << fmt::format("False positives: {} ({:.{}f}%)\n", fp, ((float)fp/(float)total_variants) * 100, 1);
    outf << fmt::format("False negatives: {} ({:.{}f}%)\n", fn, ((float)fn/(float)total_variants) * 100, 1);
    outf << "\n";
    outf << fmt::format("Precision: {:.{}f}\n", precision, 2);
    outf << fmt::format("Recall: {:.{}f}\n", recall, 2);
    outf << fmt::format("F1 score: {:.{}f}\n", f1_score, 2);
    outf << "\n";
    outf << "-----------\n";
    outf << "\n";
    outf << "False negative variants\n";
    if (fn > 0) {
        outf << "variant\te_vaf\te_cov\te_hiaf\te_hicov\te_gene\te_hgvs_c\te_hgvs_p\te_transcript\to_vaf\to_cov\to_hiaf\to_hicov\to_gene\to_hgvs_c\to_hgvs_p\to_transcript\n";
        std::string fn_var_str;
        outf << join(fn_variants, "\n", fn_var_str) << "\n";    
    } else {
        outf << "none" << "\n";
    }
    outf << "\n";
    outf << "True positive variants\n";
    if (tp > 0) {
        outf << "variant\te_vaf\te_cov\te_hiaf\te_hicov\te_gene\te_hgvs_c\te_hgvs_p\te_transcript\to_vaf\to_cov\to_hiaf\to_hicov\to_gene\to_hgvs_c\to_hgvs_p\to_transcript\n";
        std::string tp_var_str; 
        outf << join(tp_variants, "\n", tp_var_str) << "\n";
    } else {
        outf << "none" << "\n";
    }
    outf << "\n";
    outf << "False positive variants\n";
    if (fp > 0) {
        outf << "variant\te_vaf\te_cov\te_hiaf\te_hicov\te_gene\te_hgvs_c\te_hgvs_p\te_transcript\to_vaf\to_cov\to_hiaf\to_hicov\to_gene\to_hgvs_c\to_hgvs_p\to_transcript\n";
        std::string fp_var_str; 
        outf << join(fp_variants, "\n", fp_var_str) << "\n";
    } else {
        outf << "none" << "\n";
    }   
    return 0;
}