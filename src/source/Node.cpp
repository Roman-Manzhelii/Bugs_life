#include "../header/Node.hpp"
using namespace std;

Node::Node(Node* no, Node* ne, Node* so, Node* se) : m_no(no), m_ne(ne), m_so(so), m_se(se) {
    nodeCount = id;
    id++;
}

Node::~Node() {
    
}

bool Node::operator==(Node* b) {
    return m_depth > 0 ? (m_ne->id == b->m_ne->id 
         && m_no->id == b->m_no->id 
         && m_se->id == b->m_se->id 
         && m_so->id == b->m_so->id)
         : false;
}

bool Node::operator!=(Node* b) {
    return !operator==(b);
}