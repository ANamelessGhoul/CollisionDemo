#include "quadTree.h"
#include "quadTreeWorld.h"
#include <iostream>

QuadTree::QuadTree(QuadTreeWorld* newWorld, const Rectangle& bounds)
{
    world = newWorld;
    root = new Node;
    root->bbox = {{bounds.x + bounds.width / 2, bounds.y + bounds.height / 2}, bounds.width / 2, bounds.height / 2};
    root->level = 0;
}

void QuadTree::splitNode(Node* node) {
    Vector2 center = node->bbox.center;
    float halfWidth = node->bbox.halfWidth / 2.0f;
    float halfHeight = node->bbox.halfHeight / 2.0f;

    // Create four child nodes
    for (int i = 0; i < 4; i++) {
        node->children[i] = new Node();
        node->children[i]->level = node->level + 1;
        node->children[i]->parent = node; // Set the parent pointer for each child node
    }

    node->children[0]->bbox = BoundingBoxCustom{ Vector2{ center.x - halfWidth, center.y - halfHeight }, halfWidth, halfHeight }; // NW
    node->children[1]->bbox = BoundingBoxCustom{ Vector2{ center.x + halfWidth, center.y - halfHeight }, halfWidth, halfHeight }; // NE
    node->children[2]->bbox = BoundingBoxCustom{ Vector2{ center.x - halfWidth, center.y + halfHeight }, halfWidth, halfHeight }; // SW
    node->children[3]->bbox = BoundingBoxCustom{ Vector2{ center.x + halfWidth, center.y + halfHeight }, halfWidth, halfHeight }; // SE

    // Move points to child nodes
    for (auto& index : node->indices) {
        if (index.first >= world->PointsSize()) {
            std::cerr << "Error: Invalid index in positions vector: " << index.first << std::endl;
            continue;
        }

        // Use the root node's bbox to determine the child index, as all child nodes' bboxes are now initialized
        int childIndex = getIndex(root->bbox, world->GetPosition(index.first));
        if (childIndex != -1) {
            node->children[childIndex]->indices[index.first] = true;
        }
    }

    // Clear the current node's point indices
    node->indices.clear();
}




void QuadTree::remove(size_t index, const Vector2& point) {
    // Find the leaf node that contains the point
    Node* leafNode = root;
    while (!leafNode->indices.empty() || leafNode->children[0] != nullptr) {
        int childIndex = getIndex(leafNode->bbox, point);
        if (leafNode->children[childIndex] == nullptr) {
            break;
        }
        leafNode = leafNode->children[childIndex];
    }

    // Remove the index from the leaf node
    auto it = leafNode->indices.find(index);
    if (it != leafNode->indices.end()) {
        leafNode->indices.erase(it);

        // Check if the node needs to be merged
        while (leafNode->indices.empty() && leafNode->level > 0) {
            Node* parentNode = leafNode->parent;
            for (size_t i = 1; i < 4; i++) {
                if (parentNode->children[i] != nullptr && Vector2Equals(parentNode->children[i]->bbox.center, leafNode->bbox.center)) {
                    deleteNode(leafNode);
                    parentNode->children[i] = nullptr;
                    break;
                }
            }
            leafNode = parentNode;
        }
    }
}




int QuadTree::getIndex(const BoundingBoxCustom& bbox, const Vector2& point) {

    std::cout << "Point: (" << point.x << ", " << point.y << ")" << std::endl;
    std::cout << "BBox Center: (" << bbox.center.x << ", " << bbox.center.y << ")" << std::endl;

    bool topHalf = point.y < bbox.center.y;
    bool leftHalf = point.x < bbox.center.x;

    if (leftHalf && topHalf) // NW
        return 0;
    else if (!leftHalf && topHalf) // NE
        return 1;
    else if (leftHalf && !topHalf) // SW
        return 2;
    else if (!leftHalf && !topHalf) // SE
        return 3;

    return -1;  // point does not fit in any quadrant
}

void QuadTree::insert(size_t index, const Vector2& point) {
    // Insert the point into the root node
    int childIndex = getIndex(root->bbox, point);
    if (childIndex != -1) {
        insertIntoChild(root, childIndex, index);
    }
}


void QuadTree::insertIntoChild(Node* node, int childIndex, size_t index) {
    // If the node is a leaf, add the index to its indices unordered_map
    if (!node->children[0]) {
        node->indices[index] = true;

        // Split the node if it exceeds the maximum number of objects and the maximum level
        if (node->indices.size() > MAX_OBJECTS && node->level < MAX_LEVELS) {
            splitNode(node);
        }
    }
    // Otherwise, insert the index into the appropriate child node
    else {
        int newChildIndex = getIndex(node->children[childIndex]->bbox, world->GetPosition(index));
        if (newChildIndex != -1) {
            insertIntoChild(node->children[childIndex], newChildIndex, index);
        }
    }
}


QuadTree::~QuadTree() {
    // Delete all nodes starting from the root
    deleteNode(root);
}

void QuadTree::deleteNode(Node* node) {
    // Recursively delete all children nodes
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != nullptr) {
            deleteNode(node->children[i]);
        }
    }

    // Delete this node
    delete node;
}

int QuadTree::QueryNearest(const Vector2& position) {
    int nearestIndex = -1;
    float nearestDistance = FLT_MAX;
    QueryNearestRecursive(root, position, nearestIndex, nearestDistance);
    return nearestIndex;
}

bool BoundingBoxCircleOverlap(const BoundingBoxCustom& bbox, const Vector2& position, float radius) {
    Vector2 closestPoint = Vector2{ Clamp(position.x, bbox.center.x - bbox.halfWidth, bbox.center.x + bbox.halfWidth),
                                    Clamp(position.y, bbox.center.y - bbox.halfHeight, bbox.center.y + bbox.halfHeight) };
    float distance = Vector2Distance(closestPoint, position);
    return distance < radius;
}

void QuadTree::QueryNearestRecursive(Node* node, const Vector2& position, int& nearestIndex, float& nearestDistance) {
    if (!node) {
        return;
    }

    // Check if this node is closer than the current nearest point
    for (const auto& index : node->indices) {
        float distance = Vector2Distance(position, world->GetPosition(index.first));
        if (distance < nearestDistance) {
            nearestIndex = index.first;
            nearestDistance = distance;
        }
    }

    // Check if any child nodes intersect with the search radius
    for (const auto& child : node->children) {
        if (child && BoundingBoxCircleOverlap(child->bbox, position, nearestDistance)) {
            float distanceToChild = BoundingBoxCustomDistance(position, child->bbox);
            if (distanceToChild < nearestDistance) {
                QueryNearestRecursive(child, position, nearestIndex, nearestDistance);
            }
        }
    }
}

float QuadTree::BoundingBoxCustomDistance(const Vector2& position, const BoundingBoxCustom& bbox) {
    float dx = std::max(bbox.center.x - position.x - bbox.halfWidth, 0.0f);
    float dy = std::max(bbox.center.y - position.y - bbox.halfHeight, 0.0f);
    return sqrt(dx * dx + dy * dy);
}



void QuadTree::search(const Rectangle& bounds, std::vector<size_t>& results) {
    if (!root) {
        return;
    }

    BoundingBoxCustom searchBbox = {{bounds.x + bounds.width / 2, bounds.y + bounds.height / 2}, bounds.width / 2, bounds.height / 2};

    std::vector<Node*> nodesToCheck;
    nodesToCheck.push_back(root);

    while (!nodesToCheck.empty()) {
        Node* currentNode = nodesToCheck.back();
        nodesToCheck.pop_back();

        if (currentNode->bbox.halfWidth + currentNode->bbox.center.x >= searchBbox.center.x - searchBbox.halfWidth &&
            currentNode->bbox.center.x - currentNode->bbox.halfWidth <= searchBbox.center.x + searchBbox.halfWidth &&
            currentNode->bbox.halfHeight + currentNode->bbox.center.y >= searchBbox.center.y - searchBbox.halfHeight &&
            currentNode->bbox.center.y - currentNode->bbox.halfHeight <= searchBbox.center.y + searchBbox.halfHeight) {

            // Node intersects the search bounding box, add its points to the results
            for (const auto& index : currentNode->indices) {
                results.push_back(index.first);
            }

            // If it's a leaf node, no need to check children
            if (currentNode->children[0] != nullptr) {
                // Check the children
                for (const auto& child : currentNode->children) {
                    nodesToCheck.push_back(child);
                }
            }
        }
    }
}

