#ifndef RANKTREE_H
#define RANKTREE_H
#include <cstdlib>
#include <iostream>

    template<typename T>
    class RankTree {
    private:
        template<typename S>
        class Node {
        private:
            S data; //owner
            Node<S> *father;
            Node<S> *right;
            Node<S> *left;
            int height;
            int size;

            Node<S>() : father(NULL), right(NULL), left(NULL), height(0), size(1) {}
            explicit Node<S>(const S& data) : data(data), father(NULL), right(NULL), left(NULL),
                                              height(0), size(1) {}
            //const S& getData() const{ return data;}
            static void swapNodeNextInOrder(Node<S>* first, Node<S>* second) {
                Node<S> *tmp = first;
                Node<S> *tmp_father = first->father;
                Node<S> *tmp_first_right_son = first->right;
                Node<S> *tmp_first_left_son = first->left;
                Node<S> *tmp_second_father = second->father;
                Node<S> *tmp_second_right_son = second->right;
                int first_height = first->height;
                int second_height = second->height;
                int first_size = first->size;
                int second_size = second->size;

                first = second;
                first->height = first_height;
                first->size = first_size - 1;
                first->right = tmp_first_right_son;
                tmp_first_right_son->father = first;
                first->left = tmp_first_left_son;
                tmp_first_left_son->father = first;
                first->father = tmp_father;
                if (tmp_father != NULL && *tmp_father > *first) {
                    tmp_father->left = first;
                } else if (tmp_father != NULL && *tmp_father < *first)
                    tmp_father->right = first;

                second = tmp;
                second->height = second_height;
                second->size = second_size;
                second->right = tmp_second_right_son;
                second->left = NULL;
                if (tmp_second_right_son != NULL)
                    tmp_second_right_son->father = second;
                second->father = tmp_second_father;
                tmp_second_father->left = second;
            }

            static Node<T>* neighborSwapNodeNextInOrder(Node<S>* first, Node<S>* second)
            {
                Node<S>* tmp = first;
                Node<S>* tmp_father = first->father;
                Node<S>* tmp_first_left_son = first->left;
                int first_size = first->size;

                first = second;
                first->left = tmp_first_left_son;
                tmp_first_left_son->father = first;
                first->father = tmp_father;
                first->size = first_size - 1;
                if (tmp_father != NULL && *(tmp_father) > *first) {
                    tmp_father->left = first;
                }
                else if (tmp_father != NULL && *(tmp_father) < *first)
                    tmp_father->right = first;
                tmp->right = NULL;
                tmp->left = NULL;
                tmp_father = NULL;
                return tmp;
            }
        public:
            Node<S>(const Node<S> &other) = delete;

            virtual ~Node<S>(){}

            Node<S> &operator=(const Node<S> &other) = delete;

            bool operator<(const Node<S> &other)
            {
                return data < other.data;
            }
            bool operator>(const Node<S> &other)
            {
                return data > other.data;
            }
            bool operator==(const Node<S> &other)
            {
                if(other == NULL){
                    return this == NULL;
                }
                return data == other.data;
            }

            Node<T>* getMin(Node<T>* node)
            {
                if (node == NULL)
                    return NULL;
                Node<T> *min = node;
                while (min->left != NULL)
                    min = min->left;
                return min;
            }

            Node<T>* removeNode(const T& received_data, Node<T>** root) {
                Node<T>* to_return = NULL;
                if (received_data < data && left != NULL)
                    to_return = left->removeNode(received_data,root);
                else if (received_data > data && right != NULL)
                    to_return = right->removeNode(received_data,root);
                else {
                    Node<T> *to_replace = NULL;
                    if (left == NULL && right == NULL) { //has no sons
                        if (father != NULL && *father > *this)
                            father->left = NULL;
                        else if (father != NULL && !(*father > *this))
                            father->right = NULL;
                        to_return = this;
                        if (to_return == *root)
                            *root = NULL;
                    }
                    else if (left == NULL || right == NULL) { //has one son
                        if (left == NULL) {
                            if(father != NULL){
                                if (father->data > data)
                                    father->left = right;
                                else
                                    father->right = right;
                            }
                            right->father = father;
                            if (this == *root )
                                *root = right;
                        } else {
                            if(father != NULL) {
                                if (father->data > data)
                                    father->left = left;
                                else
                                    father->right = left;
                            }
                            left->father = father;
                            if (this == *root)
                                *root = left;
                        }
                        to_return = this;
                    } else { //has two sons
                        to_replace = right->getMin(right);
                        if (this == *root)
                            *root = to_replace;
                        if (to_replace == this->right) {
                            to_return = neighborSwapNodeNextInOrder(this, to_replace);
                        }
                        else {
                            if (!(to_replace == this))
                                swapNodeNextInOrder(this, to_replace);
                            to_return = to_replace->right->removeNode(data, root);
                        }
                    }
                }
                if (*root) {
                    (*root)->height = 1 + std::max(RankTree<T>::height((*root)->right), RankTree<T>::height((*root)->left));
                    (*root)->doBalance(root);
                }
                else {
                    doBalance(root);
                }

                height = 1 + std::max(RankTree<T>::height(right), RankTree<T>::height(left));
                size = RankTree<T>::size(right) + RankTree<T>::size(left) + 1;
                return to_return;
            }

            void ll_roll(Node<T>** root)
            {
                Node<T> *temp = left;
                left = temp->right;
                if (temp->right != NULL)
                    temp->right->father = this;
                temp->right = this;
                temp->father = father;
                if (father != NULL) {
                    if (father->data < temp->data)
                        father->right = temp;
                    else {
                        father->left = temp;
                    }
                }
                father = temp;
                height = 1+ std::max(RankTree<T>::height(right), RankTree<T>::height(left));
                size = RankTree<T>::size(right) + RankTree<T>::size(left) + 1;
                temp->height = 1+ std::max(RankTree<T>::height(temp->right), RankTree<T>::height(temp->left));
                temp->size = RankTree<T>::size(temp->right) + RankTree<T>::size(temp->left) + 1;
                if (this == *root){
                    *root = temp;
                }
            }

            void rr_roll(Node<T>** root) {
                Node<T> *temp = right;
                right = temp->left;
                if (temp->left != NULL)
                    temp->left->father = this;
                temp->left = this;
                temp->father = father;
                if (father != NULL) {
                    if (father->data < temp->data)
                        father->right = temp;
                    else {
                        father->left = temp;
                    }
                }
                father = temp;
                height = 1+ std::max(RankTree<T>::height(right), RankTree<T>::height(left));
                size = RankTree<T>::size(right) + RankTree<T>::size(left) + 1;
                temp->height = 1+ std::max(RankTree<T>::height(temp->right), RankTree<T>::height(temp->left));
                temp->size = RankTree<T>::size(temp->right) + RankTree<T>::size(temp->left) + 1;
                if (this == *root){
                    *root = temp;
                }
            }

            void lr_roll(Node<T>** root) {
                Node<T> *temp = left;
                temp->rr_roll(root);
                ll_roll(root);
            }

            void rl_roll(Node<T>** root) {
                Node<T> *temp = right;
                temp->ll_roll(root);
                rr_roll(root);
            }

            void doBalance(Node<T> **root) {
                int dif = difference();
                if (dif == 2) {
                    if (left->difference() >= 0) {
                        ll_roll(root);
                    } else {
                        lr_roll(root);
                    }
                }
                if (dif == -2) {
                    if (right->difference() <= 0) {
                        rr_roll(root);
                    } else {
                        rl_roll(root);
                    }
                }
            }

            int difference() {
                if (!left && !right)
                    return 0;
                else if (!left)
                    return -1 - right->height;
                else if (!right)
                    return 1+ left->height;
                return left->height - right->height;
            }

            friend class RankTree;
        };


        Node<T> *root;

        void insertNode(const T& data, Node<T>* input_root, Node<T>* intended_father, Node<T>** real_root);
        Node<T> *findNode(const T& data, Node<T> *node);
        static int height(const Node<T>* node);
        static int size(const Node<T>* node);
        Node<T> *getMax(Node<T> *node);
        Node<T> *selectNode(Node<T>* node, int index);
        Node<T>* findCloserFromTopNode(const T& data, Node<T>* node, Node<T>* last_bigger);
        Node<T>* findCloserFromButtomNode(const T& data, Node<T>* node, Node<T>* last_smaller);
    public:
        RankTree() : root(NULL) {}
        RankTree(const RankTree &other);
        ~RankTree();
        RankTree &operator=(const RankTree &other) = delete;
        void insert(const T& data);
        void remove(const T& data);
        T* getMaxData();
        T* find(const T& data);
        T* select(int index);
        void postOrderClear(Node<T> *node);
        Node<T>* getRoot() const;
        void createEmptyTree(int *final_tree_size, int h, int lower_full_tree_size);
        void recursiveCreateEmptyTree(int* size, int h, int* dif, Node<T>* node);
        void postOrderUpdateHeight(Node<T>* node);
        void recursivePutValuesInOrder(T* array, Node<T>* node, int arr_len, int* index);
        void insertToArrayInOrder(T* array, int n, int* index,Node<T>* node);
        void insertToArrayInReverseOrder(T* array, int n, int *index, Node<T>* node);
        Node<T>* findCloserFromTop(const T& data);
        Node<T>* findCloserFromButtom(const T& data);
        int getNumOfNodesInRange(const T& lower, const T& higher);
        Node<T> *findHighestInBoundsNode(const T& lower,const T& higher, Node<T> *current_node);
        void setRootNull();

        class Exception : public std::exception {};
        class DoesNotExist : public Exception {};
        class AlreadyExists : public Exception {};
    };

    template<class T>
    RankTree<T>::~RankTree() {
        postOrderClear(root);
        root = NULL;
    }

    template<class T>
    typename RankTree<T>:: template Node<T>* RankTree<T>::getRoot() const
    {
        return root;
    }

    template<class T>
    void RankTree<T>::insert(const T& data)
    {
        if (findNode(data, root) != NULL)
            throw AlreadyExists();
        if (root == NULL) {
            root = new Node<T>(data);
            return;
        }
        if ((root->left == NULL) && data < root->data) {
            root->left = new Node<T>(data);
            root->left->father = root;
            root->height = 1;
            root->size += 1;
        }
        else if ((root->right == NULL) && data > root->data) {
            root->right = new Node<T>(data);
            root->right->father = root;
            root->height = 1;
            root->size += 1;
        }
        else
            insertNode(data, root, NULL, &root);
    }

    template<class T>
    void RankTree<T>::insertNode(const T& data, Node<T>* input_root, Node<T>* intended_father, Node<T>** real_root
    )
    {
        if (input_root == NULL) {
            Node<T> *node = new Node<T>(data);
            if (intended_father->data > data) {
                intended_father->left = node;
            }
            else {
                intended_father->right = node;
            }
            node->father = intended_father;
            return;
        }
        if (data < input_root->data) {
            insertNode(data, input_root->left, input_root, real_root);
            input_root->height = (input_root->right == NULL) ? 1 + height(input_root->left) :
                                 1 + std::max(height(input_root->left), height(input_root->right));
        } else {
            insertNode(data, input_root->right, input_root, real_root);
            input_root->height = (input_root->left == NULL) ? 1 + height(input_root->right) :
                                 1 + std::max(height(input_root->left), height(input_root->right));
        }
        input_root->size = 1+ (size(input_root->left) + size(input_root->right));
        input_root->doBalance(real_root);
    }

    template<class T>
    void RankTree<T>::remove(const T& data)
    {
        Node<T>* found = findNode(data, root);
        if (found == NULL) {
            throw DoesNotExist();
        }
        else{
            Node<T>* to_remove = root->removeNode(data, &root);
            delete to_remove;
        }
    }
    template<class T>
    T* RankTree<T>::select(int index)
    {
        index++;
        return &(selectNode(root,index)->data);
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::selectNode(Node<T>* node, int index)
    {
        if(size(node->left)== index-1){
            return node;
        }
        else if(size(node->left) > index-1)
            return selectNode(node->left, index);
        else //size(node->left) < index
            return selectNode(node->right, index - size(node->left)-1);
    }
//    template<class T>
//    typename RankTree<T>::template Node<T> *RankTree<T>::getMin(Node<T> *node)
//    {
//        if (node == NULL)
//            return NULL;
//        Node<T> *min = node;
//        while (min->left != NULL)
//            min = min->left;
//        return min;
//    }

    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findHighestInBoundsNode(const T& lower,const T& higher, Node<T> *current_node)
    {
        if (current_node!= NULL && (current_node->data < lower || current_node->data > higher)){
            if (current_node->data < lower)
                return findHighestInBoundsNode(lower,higher,current_node->right);
            else
                return findHighestInBoundsNode(lower,higher,current_node->left);
        }
        return current_node;
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findCloserFromTop(const T& data)
    {
        return findCloserFromTopNode(data,root, NULL);
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findCloserFromButtom(const T& data)
    {
        return findCloserFromButtomNode(data,root, NULL);
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findCloserFromTopNode(const T& data, Node<T>* node, Node<T>* last_bigger)
    {
        if (node == NULL)
            return last_bigger;
        if (data < node->data)
            return findCloserFromTopNode(data, node->left, node);
        else if(data > node->data)
            return findCloserFromTopNode(data, node->right, last_bigger);
        else
            return node;
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findCloserFromButtomNode(const T& data, Node<T>* node, Node<T>* last_smaller)
    {
        if (node == NULL)
            return last_smaller;
        if (data < node->data)
            return findCloserFromButtomNode(data, node->left, last_smaller);
        else if(data > node->data)
            return findCloserFromButtomNode(data, node->right, node);
        else
            return node;
    }
    template<class T>
    int RankTree<T>::getNumOfNodesInRange(const T& lower, const T& higher)
    {
        Node<T>* common_father = findHighestInBoundsNode(lower, higher, root);
        if(common_father == NULL){
            return 0;
        }
        Node<T>* left_edge = findCloserFromTop(lower);
        Node<T>* current_left = left_edge;
        Node<T>* right_edge = findCloserFromButtom(higher);
        Node<T>* current_right = right_edge;
        int count = 0;
        while(current_left != common_father)
        {
            if(!(current_left->data < left_edge->data))
            {
                if(current_left->right != NULL){
                    count += current_left->right->size;
                }
                count++;
            }
            current_left = current_left->father;
        }
        while(current_right != common_father)
        {
            if(!(current_right->data > right_edge->data))
            {
                if(current_right->left != NULL){
                    count += current_right->left->size;
                }
                count++;
            }
            current_right = current_right->father;
        }
        return count + 1;
    }

    template<class T>
    T* RankTree<T>::getMaxData()
    {
        Node<T>* get_max = getMax(root);
        if(root == NULL){
            return NULL;
        }
        return &(get_max->data);
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::getMax(Node<T> *node)
    {
        if (node == NULL)
            return NULL;
        Node<T> *max = node;
        while (max->right != NULL)
            max = max->right;
        return max;
    }
    template<class T>
    T* RankTree<T>::find(const T& data)
    {
        Node<T>* found = findNode(data, root);
        if (found == NULL){
            throw DoesNotExist();
        }
        else
            return &(found->data);
    }
    template<class T>
    typename RankTree<T>::template Node<T> *RankTree<T>::findNode(const T& data, Node<T> *node) {
        if (node == NULL)
            return NULL;
        if(data == node->data)
            return node;
        if (data < node->data)
            return findNode(data, node->left);
        else /*(data > node->data)*/
            return findNode(data, node->right);
    }

    template<class T>
    int RankTree<T>::height(const Node<T> *node){
        if (node == NULL)
            return -1;
        return node->height;
    }
    template<class T>
    int RankTree<T>::size(const Node<T>* node)
    {
        if(node == NULL)
            return 0;
        return node->size;
    }

    template<class T>
    void RankTree<T>::setRootNull()
    {
        root = NULL;
    }

    template<class T>
    void RankTree<T>::postOrderClear(Node<T> *node)
    {
        if (node == NULL) {
            return;
        }
        postOrderClear(node->left);
        postOrderClear(node->right);
        node->right = NULL;
        node->left = NULL;
        node->father = NULL;
        delete node;
    }

    template<class T>
    void RankTree<T>::createEmptyTree(int *final_tree_size, int h, int lower_full_tree_size)
    {
        //check difference between full tree and our future tree
        int dif = *final_tree_size - lower_full_tree_size;

        //clean original tree
        postOrderClear(root);

        //create new root for the tree
        Node<T>* node = new Node<T>;
        root = node;
        root->height = h;
        (*final_tree_size)--;
        //fill the tree with empty nodes recursively - the tree won't be a full tree but a quite-full tree
        recursiveCreateEmptyTree(final_tree_size,h-1, &dif, root);
        postOrderUpdateHeight(root);
    }


    template<class T>
    void RankTree<T>::postOrderUpdateHeight(Node<T>* node)
    {
        if (node == NULL) {
            return;
        }
        postOrderUpdateHeight(node->left);
        postOrderUpdateHeight(node->right);
        node->height = 1+ std::max(RankTree<T>::height(node->right), RankTree<T>::height(node->left));
        node->size = 1+ RankTree<T>::size(node->right) + RankTree<T>::size(node->left);
    }

    template<class T>
    void RankTree<T>::recursiveCreateEmptyTree(int* size, int h, int* dif, Node<T>* node) {
        if (*size < 0 || h <= -1)
            return;
        if (h == 0) {
            if ((*dif) > 0)
                (*dif)--;
            else
                return;
        }
        node->left = new Node<T>;
        (*size)--;
        node->left->father = node;
        recursiveCreateEmptyTree(size, h - 1, dif, node->left);
        if (h == 0) {
            if ((*dif) > 0)
                (*dif)--;
            else
                return;
        }
        if (*size > 0){
            node->right = new Node<T>;
            //node->height = h; // todo:: return later
            (*size)--;
            node->right->father = node;
            recursiveCreateEmptyTree(size, h - 1, dif, node->right);
        }
    }

    template<class T>
    void RankTree<T>::recursivePutValuesInOrder(T* array, Node<T>* node, int arr_len, int* index)
    {
        if (node == NULL || *index >= arr_len)
            return;
        recursivePutValuesInOrder(array, node->left, arr_len, index);
        node->data = array[*index];
        (*index)++;
        recursivePutValuesInOrder(array, node->right, arr_len, index);
    }
    template<class T>
    void RankTree<T>::insertToArrayInOrder(T* array, int n, int* index, Node<T>* node)
    {
        if (node == NULL || *index >= n) {
            return;
        }
        insertToArrayInOrder(array, n, index,node->left);
        if (*index >= n)
            return;
        array[*index] = node->data;
        (*index)++;
        insertToArrayInOrder(array, n, index, node->right);
    }
    template<class T>
    void RankTree<T>::insertToArrayInReverseOrder(T* array, int n, int *index, Node<T>* node)
    {
        if (node == NULL || *index >= n) {
            return;
        }
        insertToArrayInReverseOrder(array, n, index,node->right);
        if (*index >= n)
            return;
        array[*index] = node->data;
        (*index)++;
        insertToArrayInReverseOrder(array, n, index, node->left);
    }

#endif