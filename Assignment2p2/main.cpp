#include <iostream>
#include <vector>
using namespace std;
#include <set>
#include <utility>
#include <algorithm>
#include <queue>
#include <math.h>
class Interval
{
public:
    int start;
    int end;

    Interval(int start = 0, int end = 0)
    {
        this->start = start;
        this->end = end;
    }
};

int minPark(vector<Interval> lInterval)
{
    //todo
    int size = lInterval.size();
    int *arr = new int[size];
    int *dep = new int[size];
    int idx = 0;
    for (auto it = lInterval.begin(); it != lInterval.end(); it++)
    {
        arr[idx] = (*it).start;
        dep[idx] = (*it).end;
        idx++;
        if (idx >= size)
            break;
    }
    sort(arr, arr + size);
    sort(dep, dep + size);

    int plat_needed = 0, result = 0;
    int i = 0, j = 0;

    while (i < size && j < size)
    {
        if (arr[i] < dep[j])
        {
            plat_needed++;
            i++;
        }
        else if (arr[i] >= dep[j])
        {
            plat_needed--;
            j++;
        }
        if (plat_needed > result)
            result = plat_needed;
    }
    delete[] arr;
    delete[] dep;
    return result;
}
//
//2
//

class BusParking
{
public:
    class Interval
    {
    public:
        int start;
        int end;

        Interval(int start = 0, int end = 0)
        {
            this->start = start;
            this->end = end;
        }
    };
    class Node
    {
    public:
        Interval *interval;
        int digit;
        int quantityOfIntersect;
        Node *left, *right;
        friend class ITree;
        Node(Interval *i)
        {
            this->interval = i;
            this->left = NULL;
            this->right = NULL;
            digit = 0;
            quantityOfIntersect = 0;
        }
        Node(int i)
        {
            this->interval = NULL;
            left = NULL;
            right = NULL;
            digit = i;
            quantityOfIntersect = 1;
        }
        ~Node(){
            this->interval = NULL;
            left = NULL;
            right = NULL;
            digit = 0;
            quantityOfIntersect = 0;
        }
    };
    class ITree
    {

    public:
        Node *root;
        Node *root2;
        ITree() : root(NULL), root2(NULL) {}
        ~ITree(){this->clear(root); this->clear(root2);}
        Node *minNode(Node *node)
        {
            while (node->left != NULL)
            {
                node = node->left;
            }
            return node;
        }
        int findMax(Node *root2)
        {
            if (root2 == NULL)
            {
                return 0;
            }
            int res = root2->quantityOfIntersect;
            int lmax = findMax(root2->left);
            int rmax = findMax(root2->right);
            if (lmax > res)
                res = lmax;
            if (rmax > res)
                res = rmax;
            return res;
        }

        bool isExistInterval(Node *root, Interval *interval)
        {
            if (root == NULL)
                return false;
            if (root->interval->start > interval->start)
            {
                return isExistInterval(root->left, interval);
            }
            else if (root->interval->start < interval->start)
            {
                return isExistInterval(root->right, interval);
            }
            else
            {
                if (root->interval->end == interval->end)
                {
                    return true;
                }
                else
                    return isExistInterval(root->right, interval);
            }
        }
        int insert(Interval *interval)
        {
            if (!isExistInterval(root, interval))
            {
                root = insertRec(root, interval);
                insertDigit(root2, interval->start, interval->end);
            }
            return findMax(root2);
        }
        Node *insertRec(Node *root, Interval *interval)
        {
            if (root == NULL)
            {

                return new Node(interval);
            }
            if (interval->start < root->interval->start)
                root->left = insertRec(root->left, interval);
            else
            {
                root->right = insertRec(root->right, interval);
            }
            return root;
        }
        void insertDigit(Node *root2, int s, int t)
        {
            for (int i = s; i < t; i++)
            {
                this->root2 = insertDigitRec(this->root2, i);
            }
        }
        Node *insertDigitRec(Node *root2, int i)
        {
            if (root2 == NULL)
            {
                return new Node(i);
            }
            if (i == root2->digit)
            {
                root2->quantityOfIntersect++;
            }
            if (i < root2->digit)
            {
                root2->left = insertDigitRec(root2->left, i);
            }
            if (i > root2->digit)
            {
                root2->right = insertDigitRec(root2->right, i);
            }
            return root2;
        }
        int remove(Interval *interval)
        {
            //if (root == NULL) { return 0; }
            if (isExistInterval(root, interval))
            {
                root = removeRec(root, interval);
                removeDigit(root2, interval->start, interval->end);
            }
            return findMax(root2);
        }
        Node *removeRec(Node *node, Interval *interval)
        {
            if (interval->start < node->interval->start)
            {
                node->left = removeRec(node->left, interval);
                return node;
            }
            else
            {
                if (interval->start == node->interval->start && interval->end == node->interval->end)
                {
                    if (node->left == NULL && node->right == NULL)
                    {
                        delete node;
                        node = NULL;
                        return node;
                    }
                    if (node->left == NULL)
                    {
                        Node *temp = node->right;
                        delete node;
                        return temp;
                    }
                    else if (node->right == NULL)
                    {
                        Node *temp = node->left;
                        delete node;
                        return temp;
                    }
                    else
                    {
                        Node *succParent = node;

                        Node *succ = node->right;
                        while (succ->left != NULL)
                        {
                            succParent = succ;
                            succ = succ->left;
                        }
                        if (succParent != node)
                            succParent->left = succ->right;
                        else
                            succParent->right = succ->right;

                        node->interval = succ->interval;

                        delete succ;
                        return node;
                    }
                }
                else
                {
                    node->right = removeRec(node->right, interval);
                    return node;
                }
            }
        }

        void removeDigit(Node *root2, int s, int t)
        {
            for (int i = s; i < t; i++)
            {
                this->root2 = removeDigitRec(this->root2, i);
            }
        }
        Node *removeDigitRec(Node *root2, int i)
        {
            if (root2 == NULL)
            {
                return new Node(i);
            }
            if (i == root2->digit)
            {
                root2->quantityOfIntersect--;
            }
            if (i < root2->digit)
            {
                root2->left = removeDigitRec(root2->left, i);
            }
            if (i > root2->digit)
            {
                root2->right = removeDigitRec(root2->right, i);
            }
            return root2;
        }

        void clear(Node *node)
        {
            if (node == NULL)
                return;

            clear(node->left);
            clear(node->right);

            delete node->interval;
            delete node;

            return;
        }
    };

    //
public:
    ITree *tree;
    int min;

public:
    BusParking() : tree(NULL), min(0) {}
    ~BusParking() { this->clear(); min = 0;}

public:
    void add(int s, int t)
    {
        if (tree == NULL)
            tree = new ITree();
        Interval *newInterval = new Interval(s, t);
        int minCurrent = tree->insert(newInterval);
        if (minCurrent > min)
            min = minCurrent;
        return;
    }

    void remove(int s, int t)
    {
        if (tree == NULL)
            return;
        Interval *newInterval = new Interval(s, t);
        min = tree->remove(newInterval);
        return;
    }

    int minPark()
    {
        return min;
    }

    void clear()
    {
        if (tree == NULL)
            return;
        tree->clear(tree->root);
        return;
    }
};
#include <ctime>

int main()
{
    srand(time(NULL));
    BusParking *b = new BusParking;
    vector<Interval> intervals;

    for (int i = 0; i < 1000; i++)
    {
        string st;
        //cin >> st;
        int seed = rand() % 10;
        if (seed < 7)
        {
            int s, t;
            //cin >> s >> t;
            s = rand() % 10;
            t = s + rand() % 10 + 1;
            cout << "b->add(" << s << "," << t << ");" << endl;

            b->add(s, t);
            for (auto i = intervals.begin(); i != intervals.end(); i++)
            {
                if ((*i).start == s && (*i).end == t)
                {
                    intervals.erase(i);
                }
            }
            intervals.push_back(Interval(s, t));
        }
        else
        {
            if (intervals.size() == 0)
                continue;
            int i = rand() % intervals.size();
            cout << "b->remove(" << intervals[i].start << "," << intervals[i].end << ");" << endl;
            b->remove(intervals[i].start, intervals[i].end);
            intervals.erase(intervals.begin() + i, intervals.begin() + i + 1);
        }
        cout << "//" << b->minPark() << " ";
        cout << minPark(intervals) << endl;
        //b.print();
        if (b->minPark() != minPark(intervals))
            break;
    }
    delete b;

    return 0;
}
