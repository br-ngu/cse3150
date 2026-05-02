#pragma once
#include "as_node.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

class ASGraph {
public:
    // asn -> AS node
    std::unordered_map<uint32_t, std::unique_ptr<AS>> nodes;
    // propagation_rank -> list of AS pointers at that rank
    std::vector<std::vector<AS*>> ranks;

    // Load CAIDA serial-2 format relationships file
    // Format lines (non-comment): asn1|asn2|type  where type -1=p2c, 0=p2p
    void loadFromFile(const std::string& filename, const std::unordered_set<uint32_t>& rov_asns = {});

    // Build propagation ranks (flatten the DAG)
    void buildRanks();

    // Propagate all announcements (up, across, down)
    void propagate();

    // Write output CSV
    void writeCSV(const std::string& filename) const;

    AS* getAS(uint32_t asn) {
        auto it = nodes.find(asn);
        return it != nodes.end() ? it->second.get() : nullptr;
    }

private:
    // Detect provider->customer cycles using DFS
    // Returns true if cycle found
    bool hasCycle() const;
    bool dfsCheckCycle(uint32_t asn,
                       std::unordered_set<uint32_t>& visited,
                       std::unordered_set<uint32_t>& in_stack) const;

    AS* getOrCreate(uint32_t asn, const std::unordered_set<uint32_t>& rov_asns) {
        auto it = nodes.find(asn);
        if (it != nodes.end()) return it->second.get();
        bool rov = rov_asns.count(asn) > 0;
        auto node = std::make_unique<AS>(asn, rov);
        AS* ptr = node.get();
        nodes[asn] = std::move(node);
        return ptr;
    }
};
