#pragma once
#include <string>
#include <vector>
#include <cstdint>

enum class Relationship {
    ORIGIN,
    CUSTOMER,
    PEER,
    PROVIDER
};

struct Announcement {
    std::string prefix;
    std::vector<uint32_t> as_path;
    uint32_t next_hop_asn;
    Relationship recv_relationship;
    bool rov_invalid;

    Announcement() : next_hop_asn(0), recv_relationship(Relationship::ORIGIN), rov_invalid(false) {}

    Announcement(const std::string& pfx, uint32_t origin_asn, bool invalid = false)
        : prefix(pfx), next_hop_asn(origin_asn),
          recv_relationship(Relationship::ORIGIN), rov_invalid(invalid) {
        as_path.push_back(origin_asn);
    }

    static int relPriority(Relationship r) {
        switch (r) {
            case Relationship::ORIGIN:   return 0;
            case Relationship::CUSTOMER: return 1;
            case Relationship::PEER:     return 2;
            case Relationship::PROVIDER: return 3;
        }
        return 99;
    }

    bool betterThan(const Announcement& other) const {
        int myPri = relPriority(recv_relationship);
        int otPri = relPriority(other.recv_relationship);
        if (myPri != otPri) return myPri < otPri;
        if (as_path.size() != other.as_path.size())
            return as_path.size() < other.as_path.size();
        return next_hop_asn < other.next_hop_asn;
    }

    std::string asPathStr() const {
        std::string s = "(";
        for (size_t i = 0; i < as_path.size(); ++i) {
            if (i) s += ", ";
            s += std::to_string(as_path[i]);
        }
        if (as_path.size() == 1) s += ",";
        s += ")";
        return s;
    }
};
