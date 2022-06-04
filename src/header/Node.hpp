#ifndef NODE_HPP
#define NODE_HPP

class Node {

    public:

    Node(Node* no, Node* ne, Node* so, Node*se);
    ~Node();
    bool operator==(Node* b);
    bool operator!=(Node* b);

    private:

    static int nodeCount; 
    int id;
    int m_depth;

    Node* m_no;
    Node* m_ne;
    Node* m_so;
    Node* m_se;

};

#endif