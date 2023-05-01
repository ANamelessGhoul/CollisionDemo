#include "quadTree.h"
#include "quadTreeWorld.h"

void Quadtree::query(Rectangle range, std::vector<size_t>& results) {
    queryRecursive(root, range, results);
}


void Quadtree::queryRecursive(Node* node, Rectangle range, std::vector<size_t>& results) {
    if (!CheckCollisionRecs(node->bounds, range)) {
        // Node is entirely outside of the range, so skip it
        return;
    }

    for (const auto& entry: node->points) {
        if (CheckCollisionPointRec(entry, range)) {
            // results[resultsCount++] = entry.first;
            results.emplace_back(entry);
        }
    }

    if (node->nw != nullptr) {
        queryRecursive(node->nw, range, results, resultsCount);
        queryRecursive(node->ne, range, results, resultsCount);
        queryRecursive(node->sw, range, results, resultsCount);
        queryRecursive(node->se, range, results, resultsCount);
    }
}



// Friend function definition
bool Contains(const Rectangle& rect, const Vector2& point) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width
        && point.y >= rect.y && point.y <= rect.y + rect.height);
}

// Modified insert method that uses the Contains method
bool Quadtree::insert(size_t index, Vector2 point) {
    if (!Contains(root->bounds, point)) {
        // Point is outside of the root node's bounds, so don't insert it
        return false;
    }
    Node* node = root;
    while (node->nw != nullptr) {
        // Point is inside an internal node, so descend the tree
        int xMid = (node->bounds.x + node->bounds.width) / 2.0;
        int yMid = (node->bounds.y + node->bounds.height) / 2.0;
        if (point.x <= xMid && point.y <= yMid) {
            node = node->nw;
        } else if (point.x > xMid && point.y <= yMid) {
            node = node->ne;
        } else if (point.x <= xMid && point.y > yMid) {
            node = node->sw;
        } else {
            node = node->se;
        }
    }
    node->points.insert({index, point});
    if (node->points.size() > static_cast<size_t>(capacity)) {
        subdivide(node);
    }
    return true;
}
void Quadtree::clear() {
    // Delete all nodes in the tree
    deleteSubtree(root);

    // Create a new root node with the original bounds
    Node* newRoot = new Node{root->bounds, std::unordered_map<size_t, Vector2>(), nullptr, nullptr, nullptr, nullptr};
    delete root;
    root = newRoot;
}

Quadtree::Quadtree(Rectangle bounds, World* newWorld) : world(newWorld) {
    root = new Node{bounds, nullptr, nullptr, nullptr, nullptr};
    this->capacity = capacity;
}


Quadtree::~Quadtree() {
    deleteSubtree(root);
}

void Quadtree::deleteSubtree(Node* node) {
    if (node->nw != nullptr) {
        deleteSubtree(node->nw);
        deleteSubtree(node->ne);
        deleteSubtree(node->sw);
        deleteSubtree(node->se);
    }
    delete node;
}

void Quadtree::subdivide(Node* node) {
    node->deleteChildren();

    float subWidth = node->bounds.width / 2.0f;
    float subHeight = node->bounds.height / 2.0f;
    float x = node->bounds.x;
    float y = node->bounds.y;

    // Create four child nodes with the appropriate bounds
    Rectangle nwBounds = {x, y, subWidth, subHeight};
    node->nw = new Node{nwBounds, std::unordered_map<size_t, Vector2>(), nullptr, nullptr, nullptr, nullptr};

    Rectangle neBounds = {x + subWidth, y, subWidth, subHeight};
    node->ne = new Node{neBounds, std::unordered_map<size_t, Vector2>(), nullptr, nullptr, nullptr, nullptr};

    Rectangle swBounds = {x, y + subHeight, subWidth, subHeight};
    node->sw = new Node{swBounds, std::unordered_map<size_t, Vector2>(), nullptr, nullptr, nullptr, nullptr};

    Rectangle seBounds = {x + subWidth, y + subHeight, subWidth, subHeight};
    node->se = new Node{seBounds, std::unordered_map<size_t, Vector2>(), nullptr, nullptr, nullptr, nullptr};
}



bool Quadtree::remove(size_t index, Vector2 point) {
    return removeRecursive(root, index, point);
}
bool Quadtree::removeRecursive(Node* node, size_t index, Vector2 point) {
    if (!Contains(node->bounds, point)) {
        return false;
    }

    auto it = node->points.find(index);
    if (it != node->points.end()) {
        node->points.erase(it);
        return true;
    }

    if (node->nw == nullptr) {
        return false;
    }

    if (removeRecursive(node->nw, index, point)) return true;
    if (removeRecursive(node->ne, index, point)) return true;
    if (removeRecursive(node->sw, index, point)) return true;
    if (removeRecursive(node->se, index, point)) return true;

    return false;
}