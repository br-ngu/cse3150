#include "as_graph.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cstdlib>

void ASGraph::loadFromFile(const std::string& filename,
                           const std::unordered_set<uint32_t>& rov_asns) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: Cannot open AS relationships file: " << filename << "\n";
        std::exit(2);
    }

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        // Format: asn1|asn2|relationship[|source]
        std::istringstream ss(line);
        std::string tok;
        std::vector<std::string> parts;
        while (std::getline(ss, tok, '|'))
            parts.push_back(tok);

        if (parts.size() < 3) continue;
        uint32_t asn1 = (uint32_t)std::stoul(parts[0]);
        uint32_t asn2 = (uint32_t)std::stoul(parts[1]);
        int rel = std::stoi(parts[2]);

        AS* a1 = getOrCreate(asn1, rov_asns);
        AS* a2 = getOrCreate(asn2, rov_asns);

        if (rel == -1) {
            // asn1 is provider, asn2 is customer
            a1->customers.push_back(a2);
            a2->providers.push_back(a1);
        } else if (rel == 0) {
            // peer to peer
            a1->peers.push_back(a2);
            a2->peers.push_back(a1);
        }
    }

    // Check for provider/customer cycles
    if (hasCycle()) {
        std::cerr << "ERROR: Cycle detected in provider/customer relationships.\n";
        std::exit(1);
    }
}

bool ASGraph::hasCycle() const {
    std::unordered_set<uint32_t> visited;
    std::unordered_set<uint32_t> in_stack;
    for (auto& [asn, node] : nodes) {
        if (!visited.count(asn)) {
            if (dfsCheckCycle(asn, visited, in_stack))
                return true;
        }
    }
    return false;
}

bool ASGraph::dfsCheckCycle(uint32_t asn,
                             std::unordered_set<uint32_t>& visited,
                             std::unordered_set<uint32_t>& in_stack) const {
    visited.insert(asn);
    in_stack.insert(asn);

    auto it = nodes.find(asn);
    if (it == nodes.end()) return false;

    // Follow provider->customer edges (asn's customers)
    for (AS* c : it->second->customers) {
        if (!visited.count(c->asn)) {
            if (dfsCheckCycle(c->asn, visited, in_stack))
                return true;
        } else if (in_stack.count(c->asn)) {
            return true;
        }
    }

    in_stack.erase(asn);
    return false;
}

void ASGraph::buildRanks() {
    // BFS from ASes with no customers (leaves = rank 0)
    // Then assign rank = max(provider ranks) + 1

    std::unordered_map<uint32_t, int> rank_map;
    std::queue<uint32_t> q;

    // Start with nodes that have no customers
    for (auto& [asn, node] : nodes) {
        if (node->customers.empty()) {
            rank_map[asn] = 0;
            q.push(asn);
        }
    }

    while (!q.empty()) {
        uint32_t cur = q.front(); q.pop();
        int cur_rank = rank_map[cur];
        AS* cur_node = nodes[cur].get();

        for (AS* provider : cur_node->providers) {
            int new_rank = cur_rank + 1;
            auto it = rank_map.find(provider->asn);
            if (it == rank_map.end() || it->second < new_rank) {
                rank_map[provider->asn] = new_rank;
                q.push(provider->asn);
            }
        }
    }

    // Any node not reached gets rank 0
    for (auto& [asn, node] : nodes) {
        if (!rank_map.count(asn))
            rank_map[asn] = 0;
    }

    // Find max rank
    int max_rank = 0;
    for (auto& [asn, r] : rank_map)
        max_rank = std::max(max_rank, r);

    // Build ranks vector
    ranks.clear();
    ranks.resize(max_rank + 1);
    for (auto& [asn, node] : nodes) {
        int r = rank_map[asn];
        node->propagation_rank = r;
        ranks[r].push_back(node.get());
    }
}

void ASGraph::propagate() {
    int max_rank = (int)ranks.size() - 1;

    // === PHASE 1: Send UP (rank 0 -> max_rank) ===
    // rank 0 sends to providers, then rank 1 processes and sends up, etc.
    for (int r = 0; r <= max_rank; ++r) {
        // Process received queue at this rank (rank 0 has nothing to process initially)
        if (r > 0) {
            for (AS* node : ranks[r])
                node->processAndStore();
        }
        // Send to providers (if not at top)
        if (r < max_rank) {
            for (AS* node : ranks[r])
                node->sendToProviders();
        }
    }

    // === PHASE 2: Send ACROSS (peers, one hop only) ===
    // All ASes send to peers first, then all process
    for (auto& [asn, node] : nodes)
        node->sendToPeers();
    for (auto& [asn, node] : nodes)
        node->processAndStore();

    // === PHASE 3: Send DOWN (max_rank -> rank 0) ===
    for (int r = max_rank; r >= 0; --r) {
        // Send to customers
        if (r > 0) {
            for (AS* node : ranks[r])
                node->sendToCustomers();
        }
        // Process at rank below
        if (r > 0) {
            for (AS* node : ranks[r - 1])
                node->processAndStore();
        }
    }
}

void ASGraph::writeCSV(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: Cannot open output file: " << filename << "\n";
        std::exit(2);
    }
    f << "asn,prefix,as_path\n";
    for (auto& [asn, node] : nodes) {
        for (auto& [prefix, ann] : node->policy->getLocalRib()) {
            f << asn << "," << prefix << ",\"" << ann.asPathStr() << "\"\n";
        }
    }
}
