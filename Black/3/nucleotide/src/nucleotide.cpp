#include "test_runner.h"

#include <cstddef>

struct Nucleotide {
    char Symbol;
    size_t Position;
    int ChromosomeNum;
    int GeneNum;
    bool IsMarked;
    char ServiceInfo;
};

struct CompactNucleotide {
    uint32_t Position;
    uint16_t GeneNum : 15;
    uint16_t IsMarked : 1;
    uint8_t Symbol : 2;
    uint8_t ChromosomeNum : 6;
    char ServiceInfo;
};

bool operator ==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.Symbol == rhs.Symbol)
        && (lhs.Position == rhs.Position)
        && (lhs.ChromosomeNum == rhs.ChromosomeNum)
        && (lhs.GeneNum == rhs.GeneNum)
        && (lhs.IsMarked == rhs.IsMarked)
        && (lhs.ServiceInfo == rhs.ServiceInfo);
}

static const char nucleotides[] = {'A', 'C', 'T', 'G'};

uint8_t Index(char c) {
    switch (c) {
    case 'A': return 0;
    case 'C': return 1;
    case 'T': return 2;
    case 'G': return 3;
    }
    return uint8_t(-1);
}

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide res;
    res.Symbol = Index(n.Symbol);
    res.Position = n.Position;
    res.ChromosomeNum = n.ChromosomeNum;
    res.GeneNum = n.GeneNum;
    res.IsMarked = n.IsMarked;
    res.ServiceInfo = n.ServiceInfo;
    return res;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide res;
    res.Symbol = nucleotides[cn.Symbol];
    res.Position = cn.Position;
    res.ChromosomeNum = cn.ChromosomeNum;
    res.GeneNum = cn.GeneNum;
    res.IsMarked = cn.IsMarked;
    res.ServiceInfo = cn.ServiceInfo;
    return res;
}

void TestSize() {
    ASSERT(sizeof(CompactNucleotide) <= 8);
}

void TestCompressDecompress() {
    Nucleotide source;

    source.Symbol = 'T';
    source.Position = 1'000'000'000;
    source.ChromosomeNum = 48;
    source.GeneNum = 1'000;
    source.IsMarked = true;
    source.ServiceInfo = 'T';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    ASSERT(source == decompressedSource);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSize);
    RUN_TEST(tr, TestCompressDecompress);
    return 0;
}
