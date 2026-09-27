#include "core/CityGrid.h"
#include <cstring>
#include <cstdio>
#include <iostream>


// Constructor — start with an empty list

CityGrid::CityGrid() : head(nullptr), count(0), nextId(0) {
}


// Destructor — traverse the list and free every dynamically allocated node

CityGrid::~CityGrid() {
    CityNode* current = head;
    while (current != nullptr) {
        CityNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    head = nullptr;
}


// Private helper — linear traversal to find a node by ID

CityNode* CityGrid::findNodeById(int id) const {
    CityNode* current = head;
    while (current != nullptr) {
        if (current->id == id)
            return current;
        current = current->next;
    }
    return nullptr;
}


// INSERT — allocate a new node and attach it at the end of the list

bool CityGrid::insertNode(const char* name, float x, float y, NodeType type) {
    CityNode* newNode = new CityNode;
    newNode->id = nextId++;
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0';
    newNode->x    = x;
    newNode->y    = y;
    newNode->type = type;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;                     // list was empty
    } else {
        CityNode* current = head;
        while (current->next != nullptr)
            current = current->next;
        current->next = newNode;            // attach at the tail
    }

    count++;
    std::cout << "[CityGrid] Inserted: ID=" << newNode->id
              << " Name=" << newNode->name << "\n";
    return true;
}


// DELETE — unlink the node from the list and free its memory

bool CityGrid::deleteNode(int id) {
    CityNode* current = head;
    CityNode* prev = nullptr;

    while (current != nullptr && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        std::cout << "[CityGrid] ERROR: Node ID=" << id << " not found.\n";
        return false;
    }

    if (prev == nullptr) {
        head = current->next;      // deleting the head node
    } else {
        prev->next = current->next;
    }

    std::cout << "[CityGrid] Deleted: ID=" << id
              << " Name=" << current->name << "\n";
    delete current;
    count--;
    return true;
}


// UPDATE — change coordinates of an existing node

bool CityGrid::updateNode(int id, float newX, float newY) {
    CityNode* node = findNodeById(id);
    if (node == nullptr) {
        std::cout << "[CityGrid] ERROR: Node ID=" << id << " not found.\n";
        return false;
    }
    node->x = newX;
    node->y = newY;
    std::cout << "[CityGrid] Updated: ID=" << id
              << " New coords=(" << newX << ", " << newY << ")\n";
    return true;
}


// SEARCH — linear search by name, returns ID

int CityGrid::searchByName(const char* name) const {
    CityNode* current = head;
    while (current != nullptr) {
        if (strncmp(current->name, name, MAX_NAME_LEN) == 0) {
            std::cout << "[CityGrid] Found: ID=" << current->id
                      << " at (" << current->x << ", "
                      << current->y << ")\n";
            return current->id;
        }
        current = current->next;
    }
    std::cout << "[CityGrid] Not found: " << name << "\n";
    return -1;
}


// DISPLAY — print a single node
void CityGrid::displayNode(int id) const {
    CityNode* node = findNodeById(id);
    if (node == nullptr) {
        std::cout << "[CityGrid] Node ID=" << id << " not found.\n";
        return;
    }
    const char* types[] = {"WAREHOUSE","CUSTOMER","CHARGING","JUNCTION"};
    printf("  [%02d] %-20s  (%.1f, %.1f)  Type: %s\n",
           node->id, node->name, node->x, node->y, types[node->type]);
}


// DISPLAY ALL — traverse the list and print every node
void CityGrid::displayAll() const {
    std::cout << "\n════════════════════════════════════════\n";
    std::cout << "  AeroRoute City Map  (" << count << " nodes)\n";
    std::cout << "════════════════════════════════════════\n";
    const char* types[] = {"WAREHOUSE","CUSTOMER","CHARGING","JUNCTION"};
    CityNode* current = head;
    while (current != nullptr) {
        printf("  [%02d] %-20s  (%.1f, %.1f)  Type: %s\n",
               current->id,
               current->name,
               current->x,
               current->y,
               types[current->type]);
        current = current->next;
    }
    std::cout << "════════════════════════════════════════\n\n";
}


// GET NODE — const pointer for read-only access

const CityNode* CityGrid::getNode(int id) const {
    return findNodeById(id);
}


// LOAD SAMPLE CITY — pre-populate for demo

void CityGrid::loadSampleCity() {
    insertNode("Warehouse_Central",  0.0f,  0.0f, WAREHOUSE);
    insertNode("Customer_A",         5.0f,  3.0f, CUSTOMER);
    insertNode("Customer_B",        -3.0f,  4.0f, CUSTOMER);
    insertNode("Customer_C",         7.0f, -2.0f, CUSTOMER);
    insertNode("Charging_North",     2.0f,  5.0f, CHARGING);
    insertNode("Junction_1",         3.0f,  2.0f, JUNCTION);
    insertNode("Junction_2",        -1.0f,  3.0f, JUNCTION);
    insertNode("Junction_3",         4.0f, -1.0f, JUNCTION);
    std::cout << "[CityGrid] Sample city loaded.\n";
}
