#include "node.h"

uint64_t Node::lastId_;
std::unordered_map<std::string, uint64_t> Node::nameToId_;
std::unordered_map<uint64_t, std::weak_ptr<Node>> Node::idToNode_;