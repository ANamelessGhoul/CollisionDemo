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
    std::vector<int> indices;
    BoundingBoxCustom bbox;
    std::array<Node*, 4> children;
    unsigned int level;
};

class QuadTree {
private:
    const unsigned int MAX_OBJECTS = 10;
    const unsigned int MAX_LEVELS = 5;
    Node* root;
    std::vector<Vector2> positions;

    void splitNode(Node* node);
    int getIndex(const BoundingBoxCustom& bbox, const Vector2& point);
    void insertIntoChild(Node* node, int childIndex, int index);
    void deleteNode(Node* node);
    int QueryNearest(const Vector2 &position);
    void QueryNearestRecursive(Node *node, const Vector2 &position, int &nearestIndex, float &nearestDistance);
    float BoundingBoxCustomDistance(const Vector2 &position, const BoundingBoxCustom &bbox);

public:
    QuadTree(const BoundingBoxCustom& bbox);
    ~QuadTree();
    void insert(int index, const Vector2& point);
    void search(const BoundingBoxCustom& searchBbox, std::vector<int>& results);
};
