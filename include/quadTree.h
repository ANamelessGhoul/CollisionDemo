#include "raylib.h"
#include "raymath.h"
#include <cfloat>
#include <vector>
#include <array>

struct BoundingBoxCustom {
    Vector2 center;
    float halfWidth;
    float halfHeight;
};

struct Node {
    BoundingBoxCustom bbox;
    std::vector<size_t> indices;
    Node* parent = nullptr;
    Node* children[4];  // Initialize the children pointers to nullptr
    int level;

    // Add a constructor for the Node struct
    Node() {
        for (int i = 0; i < 4; ++i) {
            children[i] = nullptr;
        }
    }
};


class QuadTree {
private:
    const unsigned int MAX_OBJECTS = 10;
    const unsigned int MAX_LEVELS = 5;
    Node* root;
    std::vector<Vector2> positions;

    void splitNode(Node* node);
    int getIndex(const BoundingBoxCustom &bbox, const Vector2 &point);
    void insertIntoChild(Node* node, int childIndex, size_t index);
    void deleteNode(Node* node);
    int QueryNearest(const Vector2 &position);
    void QueryNearestRecursive(Node *node, const Vector2 &position, int &nearestIndex, float &nearestDistance);
    float BoundingBoxCustomDistance(const Vector2 &position, const BoundingBoxCustom &bbox);

public:
    QuadTree(const Rectangle& bbox);
    ~QuadTree();
    void remove(size_t index, const Vector2 &point);
    void insert(size_t index, const Vector2& point);
    void search(const Rectangle& searchBbox, std::vector<size_t>& results);
};
