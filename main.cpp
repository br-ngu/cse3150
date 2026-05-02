#include "as_graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>

// Load ROV ASNs from a file (one ASN per line)
std::unordered_set<uint32_t> loadROVAsns(const std::string& filename) {
    std::unordered_set<uint32_t> asns;
    std::ifstream f(filename);
    if (!f.is_open()) return asns; // ROV file optional
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        try { asns.insert((uint32_t)std::stoul(line)); }
        catch (...) {}
    }
    return asns;
}

// Load announcements from CSV: asn,prefix,rov_invalid
void loadAnnouncements(const std::string& filename, ASGraph& graph) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: Cannot open announcements file: " << filename << "\n";
        std::exit(2);
    }
    std::string line;
    bool header = true;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        if (header) { header = false; continue; } // skip header row
        std::istringstream ss(line);
        std::string tok;
        std::vector<std::string> parts;
        while (std::getline(ss, tok, ','))
            parts.push_back(tok);
        if (parts.size() < 3) continue;

        // Strip carriage returns (Windows line endings)
        for (auto& p : parts) if (!p.empty() && p.back() == '\r') p.pop_back();

        uint32_t asn = (uint32_t)std::stoul(parts[0]);
        std::string prefix = parts[1];
        bool rov_invalid = (parts[2] == "1" || parts[2] == "true" || parts[2] == "True");

        AS* node = graph.getAS(asn);
        if (!node) {
            std::cerr << "WARNING: ASN " << asn << " in announcements not found in graph, skipping.\n";
            continue;
        }
        Announcement ann(prefix, asn, rov_invalid);
        node->seedAnnouncement(ann);
    }
}

void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog
              << " <relationships_file> <announcements_csv> <rov_asns_file> <output_csv>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printUsage(argv[0]);
        return 1;
    }

    std::string rel_file   = argv[1];
    std::string ann_file   = argv[2];
    std::string rov_file   = argv[3];
    std::string out_file   = argv[4];

    std::cerr << "Loading ROV ASNs...\n";
    auto rov_asns = loadROVAsns(rov_file);
    std::cerr << "  " << rov_asns.size() << " ROV ASes loaded.\n";

    std::cerr << "Building AS graph...\n";
    ASGraph graph;
    graph.loadFromFile(rel_file, rov_asns);
    std::cerr << "  " << graph.nodes.size() << " ASes loaded.\n";

    std::cerr << "Building propagation ranks...\n";
    graph.buildRanks();
    std::cerr << "  " << graph.ranks.size() << " ranks.\n";

    std::cerr << "Loading announcements...\n";
    loadAnnouncements(ann_file, graph);

    std::cerr << "Propagating...\n";
    graph.propagate();

    std::cerr << "Writing output to " << out_file << "...\n";
    graph.writeCSV(out_file);

    std::cerr << "Done.\n";
    return 0;
}
