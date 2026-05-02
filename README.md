# BGP Simulator — CSE3150 Course Project

A C++17 BGP (Border Gateway Protocol) simulator that builds an AS graph from CAIDA topology data, propagates announcements using standard BGP policies, and outputs a CSV of AS paths per prefix.

---

## Building

```bash
make          # builds bgp_sim and run_tests
make clean    # removes binaries
```

Requires g++ with C++17 support.

---

## Running

```bash
./bgp_sim <relationships_file> <announcements.csv> <rov_asns.csv> <output.csv>
```

**Arguments:**
- `relationships_file` — CAIDA serial-2 format. Lines: `asn1|asn2|type` where `-1` = provider→customer, `0` = peer-peer. Comment lines start with `#`.
- `announcements.csv` — CSV with header `asn,prefix,rov_invalid`. The `rov_invalid` field is `1`/`true` or `0`/`false`.
- `rov_asns.csv` — One ASN per line. These ASes will deploy ROV filtering.
- `output.csv` — Output file. Written with columns `asn,prefix,as_path`.

**Exit codes:**
- `0` — Success
- `1` — Cycle detected in provider/customer relationships
- `2` — File I/O error

---

## Running Tests

```bash
./run_tests
```

---

## Design Decisions

### File Structure

| File | Purpose |
|---|---|
| `announcement.h` | `Announcement` struct with conflict-resolution logic |
| `policy.h` | `BGP` base policy class; `ROV` subclass |
| `as_node.h` | `AS` class: holds neighbors, policy, send/process methods |
| `as_graph.h/.cpp` | Graph construction, cycle detection, rank building, propagation, CSV output |
| `main.cpp` | CLI entry point, file loading |
| `tests.cpp` | Unit/system tests |

### BGP Policy OOP Design

Each `AS` holds a `std::unique_ptr<BGP>`. `ROV` inherits from `BGP` and overrides `shouldAccept()` to drop `rov_invalid` announcements. This makes it easy to add new policies later without changing the propagation logic.

The `AS` class is declared a `friend` of `BGP` so it can directly access `local_rib` and `recv_queue` for the prepend-on-store step, avoiding the need for a more complex callback interface.

### Propagation (Up → Across → Down)

**Phase 1 — Up** (rank 0 → max rank):
For each rank level, ASes process their received queue (prepending their own ASN to each announcement's path), then send their RIB to providers. This naturally propagates from leaves to the top of the provider hierarchy.

**Phase 2 — Across** (peers, one hop only):
All ASes send to their peers first. Then all ASes process. The two-pass approach is critical: if we interleaved send and process, announcements could propagate multiple peer hops, creating routing valleys.

**Phase 3 — Down** (max rank → rank 0):
The reverse of Phase 1. Top-level ASes send to customers, each rank level processes and stores, then sends further down.

### Propagation Ranks (DAG Flattening)

Uses BFS from leaf nodes (no customers = rank 0). Each provider's rank = max(customer ranks) + 1. This assigns ranks in one pass and handles diamonds in the DAG correctly.

### Conflict Resolution

When two announcements compete for the same prefix, priority is:
1. Better relationship: `ORIGIN > CUSTOMER > PEER > PROVIDER`
2. Shorter AS path length
3. Lower next-hop ASN (tiebreak)

Implemented in `Announcement::betterThan()`.

### Cycle Detection

After loading the relationships file, DFS is run following provider→customer edges. If a cycle is found, the program prints an error and exits with code `1`. Peer cycles are not checked (they are expected and valid).

### AS Path Building

Each AS prepends its own ASN to the incoming announcement's `as_path` when storing it (not when sending). The origin AS's own ASN is included at seeding time. So for a chain `AS3 → AS2 → AS1`, the path stored at AS1 is `[1, 2, 3]`.

---

## Example

Given a small graph:
```
AS1 is provider of AS2
AS2 is provider of AS3
```

With `announcements.csv`:
```
asn,prefix,rov_invalid
3,1.2.0.0/16,0
```

Output `ribs.csv`:
```
asn,prefix,as_path
3,1.2.0.0/16,3
2,1.2.0.0/16,2 3
1,1.2.0.0/16,1 2 3
```
