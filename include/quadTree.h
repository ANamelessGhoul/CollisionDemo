#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "raylib.h"
#include "raymath.h"
#include <cfloat>
#include <unordered_map>
#include <vector>
#include <array>
#include "world.h"


struct Node {
    Rectangle bounds;
    Node* nw;
    Node* ne;
    Node* sw;
    Node* se;
    std::vector<size_t> points;

    void deleteChildren() {
        if (nw != nullptr) {
            nw->deleteChildren();
            delete nw;
            nw = nullptr;
        }
        if (ne != nullptr) {
            ne->deleteChildren();
            delete ne;
            ne = nullptr;
        }
        if (sw != nullptr) {
            sw->deleteChildren();
            delete sw;
            sw = nullptr;
        }
        if (se != nullptr) {
            se->deleteChildren();
            delete se;
            se = nullptr;
        }
    }
};

class Quadtree {
public:
    Quadtree(Rectangle bounds, int capacity, World* newWorld);
    ~Quadtree();
    bool insert(size_t index, Vector2 point);
    void clear();
    void query(Rectangle range, std::vector<size_t>& results);
    bool remove(size_t index, Vector2 point);
private:
    Node* root;
    int capacity;

    World* world;

    void subdivide(Node* node);
    void queryRecursive(Node* node, Rectangle range, std::vector<size_t>& results);
    void deleteSubtree(Node* node);
    bool removeRecursive(Node* node, size_t index, Vector2 point);
};


#endif