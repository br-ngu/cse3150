// Simple test suite - compile with: g++ -std=c++17 tests.cpp as_graph.cpp -o tests
#include "as_graph.h"
#include <cassert>
#include <iostream>
#include <fstream>

int pass_count = 0, fail_count = 0;

#define CHECK(cond, msg) \
    if (cond) { std::cout << "[PASS] " << msg << "\n"; pass_count++; } \
    else { std::cout << "[FAIL] " << msg << "\n"; fail_count++; }

// Helper: write a mini CAIDA-format relationships file
void writeRelFile(const std::string& path, const std::string& content) {
    std::ofstream f(path);
    f << content;
}

// -------------------------------------------------------
// Test 1: Basic provider->customer relationship
// -------------------------------------------------------
void test_basic_graph() {
    writeRelFile("/tmp/test1.txt", "1|2|-1\n"); // AS1 provider, AS2 customer
    ASGraph g;
    g.loadFromFile("/tmp/test1.txt");
    CHECK(g.nodes.size() == 2, "Basic graph has 2 nodes");
    AS* as1 = g.getAS(1);
    AS* as2 = g.getAS(2);
    CHECK(as1 != nullptr, "AS1 exists");
    CHECK(as2 != nullptr, "AS2 exists");
    CHECK(as1->customers.size() == 1 && as1->customers[0]->asn == 2, "AS1 has AS2 as customer");
    CHECK(as2->providers.size() == 1 && as2->providers[0]->asn == 1, "AS2 has AS1 as provider");
}

// -------------------------------------------------------
// Test 2: Peer relationship
// -------------------------------------------------------
void test_peer_relationship() {
    writeRelFile("/tmp/test2.txt", "10|20|0\n"); // peer
    ASGraph g;
    g.loadFromFile("/tmp/test2.txt");
    AS* a10 = g.getAS(10);
    AS* a20 = g.getAS(20);
    CHECK(a10->peers.size() == 1, "AS10 has 1 peer");
    CHECK(a20->peers.size() == 1, "AS20 has 1 peer");
    CHECK(a10->peers[0]->asn == 20, "AS10 peers with AS20");
}

// -------------------------------------------------------
// Test 3: Propagation ranks
// -------------------------------------------------------
void test_ranks() {
    // AS1->AS2->AS3 (provider chain)
    writeRelFile("/tmp/test3.txt", "1|2|-1\n2|3|-1\n");
    ASGraph g;
    g.loadFromFile("/tmp/test3.txt");
    g.buildRanks();
    // AS3 has no customers -> rank 0
    // AS2 has AS3 as customer -> rank 1
    // AS1 has AS2 as customer -> rank 2
    CHECK(g.getAS(3)->propagation_rank == 0, "AS3 rank 0 (leaf)");
    CHECK(g.getAS(2)->propagation_rank == 1, "AS2 rank 1");
    CHECK(g.getAS(1)->propagation_rank == 2, "AS1 rank 2 (top)");
}

// -------------------------------------------------------
// Test 4: Simple propagation - announcement goes up and down
// -------------------------------------------------------
void test_simple_propagation() {
    // AS1 provider of AS2, AS2 provider of AS3
    // Seed announcement at AS2, should reach AS1 and AS3
    writeRelFile("/tmp/test4.txt", "1|2|-1\n2|3|-1\n");
    ASGraph g;
    g.loadFromFile("/tmp/test4.txt");
    g.buildRanks();

    Announcement ann("1.2.0.0/16", 2, false);
    g.getAS(2)->seedAnnouncement(ann);
    g.propagate();

    auto* rib1 = &g.getAS(1)->policy->getLocalRib();
    auto* rib2 = &g.getAS(2)->policy->getLocalRib();
    auto* rib3 = &g.getAS(3)->policy->getLocalRib();

    CHECK(rib2->count("1.2.0.0/16"), "AS2 has prefix in RIB");
    CHECK(rib1->count("1.2.0.0/16"), "AS1 received prefix from customer AS2");
    CHECK(rib3->count("1.2.0.0/16"), "AS3 received prefix from provider AS2");
}

// -------------------------------------------------------
// Test 5: Conflict resolution - customer beats provider
// -------------------------------------------------------
void test_conflict_resolution() {
    // AS4 has customers AS2 and AS3, both announce same prefix
    // AS4 picks best
    // Topology: AS1 -> AS4 -> AS2, AS3
    //                  AS4 -> AS3
    // Actually simpler: AS4 receives from customer AS2 and customer AS3
    // Both same prefix; AS2 has shorter path -> AS4 picks AS2
    writeRelFile("/tmp/test5.txt", "4|2|-1\n4|3|-1\n");
    ASGraph g;
    g.loadFromFile("/tmp/test5.txt");
    g.buildRanks();

    // AS2 announces with path length 1, AS3 announces with path length 1
    // They'll tie on path length, tiebreak by lower next_hop_asn = AS2
    Announcement a2("5.0.0.0/8", 2, false);
    Announcement a3("5.0.0.0/8", 3, false);
    g.getAS(2)->seedAnnouncement(a2);
    g.getAS(3)->seedAnnouncement(a3);
    g.propagate();

    auto& rib4 = g.getAS(4)->policy->getLocalRib();
    CHECK(rib4.count("5.0.0.0/8"), "AS4 has prefix");
    if (rib4.count("5.0.0.0/8")) {
        // Tiebreak: lowest next_hop_asn = 2
        CHECK(rib4.at("5.0.0.0/8").next_hop_asn == 2, "AS4 chose AS2 (lower ASN tiebreak)");
    }
}

// -------------------------------------------------------
// Test 6: ROV drops invalid announcements
// -------------------------------------------------------
void test_rov() {
    writeRelFile("/tmp/test6.txt", "1|2|-1\n");
    std::unordered_set<uint32_t> rov = {1}; // AS1 deploys ROV
    ASGraph g;
    g.loadFromFile("/tmp/test6.txt", rov);
    g.buildRanks();

    // AS2 announces invalid prefix
    Announcement ann("9.9.9.0/24", 2, true); // rov_invalid = true
    g.getAS(2)->seedAnnouncement(ann);
    g.propagate();

    auto& rib1 = g.getAS(1)->policy->getLocalRib();
    CHECK(!rib1.count("9.9.9.0/24"), "ROV AS1 dropped invalid announcement");
}

// -------------------------------------------------------
// Test 7: Peer propagation (one hop only)
// -------------------------------------------------------
void test_peer_propagation() {
    // AS1 and AS2 are peers. AS2 and AS3 are peers.
    // AS1 announces prefix. Should reach AS2 (peer), but NOT AS3 (AS2's other peer)
    writeRelFile("/tmp/test7.txt", "1|2|0\n2|3|0\n");
    ASGraph g;
    g.loadFromFile("/tmp/test7.txt");
    g.buildRanks();

    Announcement ann("7.7.0.0/16", 1, false);
    g.getAS(1)->seedAnnouncement(ann);
    g.propagate();

    auto& rib2 = g.getAS(2)->policy->getLocalRib();
    auto& rib3 = g.getAS(3)->policy->getLocalRib();
    CHECK(rib2.count("7.7.0.0/16"), "AS2 (peer of AS1) received prefix");
    CHECK(!rib3.count("7.7.0.0/16"), "AS3 did NOT receive prefix (no multi-hop peer)");
}

// -------------------------------------------------------
// Test 8: AS path is built correctly
// -------------------------------------------------------
void test_as_path() {
    // AS1 provider of AS2, AS2 provider of AS3
    // AS3 originates prefix. Path at AS1 should be [1, 2, 3]
    writeRelFile("/tmp/test8.txt", "1|2|-1\n2|3|-1\n");
    ASGraph g;
    g.loadFromFile("/tmp/test8.txt");
    g.buildRanks();

    Announcement ann("3.3.0.0/16", 3, false);
    g.getAS(3)->seedAnnouncement(ann);
    g.propagate();

    auto& rib1 = g.getAS(1)->policy->getLocalRib();
    CHECK(rib1.count("3.3.0.0/16"), "AS1 has prefix");
    if (rib1.count("3.3.0.0/16")) {
        auto& path = rib1.at("3.3.0.0/16").as_path;
        // path should be [1, 2, 3] (each AS prepends itself)
        bool correct = path.size() == 3 && path[0] == 1 && path[1] == 2 && path[2] == 3;
        CHECK(correct, "AS path at AS1 is [1, 2, 3]");
    }
}

int main() {
    std::cout << "=== BGP Simulator Tests ===\n\n";
    test_basic_graph();
    test_peer_relationship();
    test_ranks();
    test_simple_propagation();
    test_conflict_resolution();
    test_rov();
    test_peer_propagation();
    test_as_path();

    std::cout << "\n=== Results: " << pass_count << " passed, " << fail_count << " failed ===\n";
    return fail_count > 0 ? 1 : 0;
}
