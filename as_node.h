#pragma once
#include "policy.h"
#include <vector>
#include <memory>
#include <cstdint>

class AS {
public:
    uint32_t asn;
    int propagation_rank = -1;

    std::vector<AS*> providers;
    std::vector<AS*> customers;
    std::vector<AS*> peers;

    std::unique_ptr<BGP> policy;

    explicit AS(uint32_t asn_, bool use_rov = false) : asn(asn_) {
        if (use_rov)
            policy = std::make_unique<ROV>();
        else
            policy = std::make_unique<BGP>();
    }

    // Send all local RIB announcements to providers (they see us as a customer)
    void sendToProviders() {
        for (auto& [prefix, ann] : policy->local_rib) {
            Announcement out = ann;
            out.next_hop_asn = asn;
            out.recv_relationship = Relationship::CUSTOMER;
            for (AS* p : providers)
                p->policy->receive(out);
        }
    }

    // Send all local RIB announcements to customers (they see us as a provider)
    void sendToCustomers() {
        for (auto& [prefix, ann] : policy->local_rib) {
            Announcement out = ann;
            out.next_hop_asn = asn;
            out.recv_relationship = Relationship::PROVIDER;
            for (AS* c : customers)
                c->policy->receive(out);
        }
    }

    // Send all local RIB announcements to peers
    void sendToPeers() {
        for (auto& [prefix, ann] : policy->local_rib) {
            Announcement out = ann;
            out.next_hop_asn = asn;
            out.recv_relationship = Relationship::PEER;
            for (AS* peer : peers)
                peer->policy->receive(out);
        }
    }

    // Process received queue: prepend our ASN to each incoming announcement, then store best
    void processAndStore() {
        for (auto& [prefix, anns] : policy->recv_queue) {
            for (auto& ann : anns) {
                if (!policy->shouldAccept(ann)) continue;
                Announcement stored = ann;
                stored.as_path.insert(stored.as_path.begin(), asn);
                auto it = policy->local_rib.find(prefix);
                if (it == policy->local_rib.end() || stored.betterThan(it->second)) {
                    policy->local_rib[prefix] = stored;
                }
            }
        }
        policy->recv_queue.clear();
    }

    // Seed an announcement as origin (goes straight into local RIB)
    void seedAnnouncement(const Announcement& ann) {
        policy->seed(ann);
    }
};
