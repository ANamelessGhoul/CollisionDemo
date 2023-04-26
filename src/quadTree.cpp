#include "quadTree.h"

void QuadTree::splitNode(Node* node) {
    Vector2 center = node->bbox.center;
    float halfWidth = node->bbox.halfWidth / 2.0f;
    float halfHeight = node->bbox.halfHeight / 2.0f;

    // Create four child nodes
    node->children[0] = new Node();
    node->children[0]->bbox = BoundingBoxCustom{ Vector2{ center.x - halfWidth, center.y - halfHeight }, halfWidth, halfHeight };
    node->children[0]->level = node->level + 1;

    node->children[1] = new Node();
    node->children[1]->bbox = BoundingBoxCustom{ Vector2{ center.x + halfWidth, center.y - halfHeight }, halfWidth, halfHeight };
    node->children[1]->level = node->level + 1;

    node->children[2] = new Node();
    node->children[2]->bbox = BoundingBoxCustom{ Vector2{ center.x - halfWidth, center.y + halfHeight }, halfWidth, halfHeight };
    node->children[2]->level = node->level + 1;

    node->children[3] = new Node();
    node->children[3]->bbox = BoundingBoxCustom{ Vector2{ center.x + halfWidth, center.y + halfHeight }, halfWidth, halfHeight };
    node->children[3]->level = node->level + 1;

    // Move points to child nodes
    for (size_t i = 0; i < node->indices.size(); i++) {
        int childIndex = getIndex(node->children[0]->bbox, positions[node->indices[i]]);
        if (childIndex != -1) {
            insertIntoChild(node, childIndex, node->indices[i]);
        }
    }

    // Clear the current node's point indices
    node->indices.clear();
}


int QuadTree::getIndex(const BoundingBoxCustom& bbox, const Vector2& point) {
    bool topHalf = point.y < bbox.center.y;
    bool leftHalf = point.x < bbox.center.x;

    if (leftHalf && topHalf)
        return 2;
    else if (!leftHalf && topHalf)
        return 3;
    else if (leftHalf && !topHalf)
        return 0;
    else if (!leftHalf && !topHalf)
        return 1;

    return -1;  // point does not fit in any quadrant
}

void QuadTree::insert(int index, const Vector2& point) {
    // Insert the point into the root node
    insertIntoChild(root, getIndex(root->bbox, point), index);
}

void QuadTree::insertIntoChild(Node* node, int childIndex, int index) {
    // If the node is a leaf, add the index to its indices vector
    if (!node->children[0]) {
        node->indices.push_back(index);

        // Split the node if it exceeds the maximum number of objects and the maximum level
        if (node->indices.size() > MAX_OBJECTS && node->level < MAX_LEVELS) {
            splitNode(node);
        }
    }
    // Otherwise, insert the index into the appropriate child node
    else {
        insertIntoChild(node->children[childIndex], getIndex(node->children[childIndex]->bbox, positions[index]), index);
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

void QuadTree::QueryNearestRecursive(Node* node, const Vector2& position, int& nearestIndex, float& nearestDistance) {
    if (!node) {
        return;
    }

    // Check if this node is closer than the current nearest point
    for (const auto& index : node->indices) {
        float distance = Vector2Distance(position, positions[index]);
        if (distance < nearestDistance) {
            nearestIndex = index;
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

bool BoundingBoxCircleOverlap(const BoundingBoxCustom& bbox, const Vector2& position, float radius) {
    Vector2 closestPoint = Vector2{ Clamp(position.x, bbox.center.x - bbox.halfWidth, bbox.center.x + bbox.halfWidth),
                                    Clamp(position.y, bbox.center.y - bbox.halfHeight, bbox.center.y + bbox.halfHeight) };
    float distance = Vector2Distance(closestPoint, position);
    return distance < radius;
}

void QuadTree::search(const BoundingBoxCustom& searchBbox, std::vector<int>& results) {
    if (!root) {
        return;
    }

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
                Vector2& point = positions[index];
                if (point.x >= searchBbox.center.x - searchBbox.halfWidth &&
                    point.x <= searchBbox.center.x + searchBbox.halfWidth &&
                    point.y >= searchBbox.center.y - searchBbox.halfHeight &&
                    point.y <= searchBbox.center.y + searchBbox.halfHeight) {

                    results.push_back(index);
                }
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

