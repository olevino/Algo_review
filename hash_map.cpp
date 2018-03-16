
#include<exception>
#include<functional>
#include<vector>
#include<utility>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
    struct Node {
        std::pair<const KeyType, ValueType> item;
        Node * pref, * next;
        Node() {
            pref = next = nullptr;
        }
        Node(std::pair<const KeyType, ValueType> new_item) : item(new_item) {
            pref = nullptr;
            next = nullptr;
        }
    };
    Node * start, * finish;
    std::vector<std::pair<Node*, Node*>> table;
    Hash hasher;
    size_t length, count;
public:
    HashMap() : hasher(std::hash<KeyType>()) {
        HashMap new_hash(hasher);
        *this = new_hash;
    }
    HashMap(Hash new_hasher) : hasher(new_hasher) {
        finish = start = new Node();
        table.resize(16, {nullptr, nullptr});
        length = 16;
        count = 0;
    }
    void clear() {
        HashMap new_hash_map(hasher);
        *this = new_hash_map;
    }
    void update() {
        if (count * 2 > length) {
            std::vector<std::pair<Node*, Node*>> new_table(length * 2, {nullptr, nullptr});
            Node * new_start, * new_end;
            new_start = new_end = new Node();
            Node * cur = start;
            cur = cur->next;
            while (cur != nullptr) {
                Node * tmp = cur->next;
                size_t hash_value = hasher(cur->item.first) % (2 * length);
                if (new_table[hash_value].first == nullptr) {
                    cur->pref = new_end;
                    new_end->next = cur;
                    new_end = cur;
                    new_end->next = nullptr;
                    new_table[hash_value] = {new_end, new_end};
                } else {
                    if (new_table[hash_value].second -> next != nullptr) {
                        new_table[hash_value].second->next->pref = cur;
                        cur->next = new_table[hash_value].second->next;
                    } else {
                        cur->next = nullptr;
                        new_end = cur;
                    }
                    new_table[hash_value].second->next = cur;
                    cur->pref = new_table[hash_value].second;
                    new_table[hash_value].second = cur;
                }
                cur = tmp;
            }
            table.resize(0);
            table = std::move(new_table);
            length *= 2;
            delete start;
            start = new_start;
            finish = new_end;
        }
    }
    void insert(std::pair<const KeyType, ValueType> item) {
        size_t hash_value = hasher(item.first) % length;
        if (table[hash_value].first == nullptr) {
            Node * cur = new Node(item);
            finish->next = cur;
            cur->pref = finish;
            finish = cur;
            table[hash_value] = {finish, finish};
            count++;
            update();
        } else {
            int flag = 0;
            Node * tmp = table[hash_value].first;
            for(;;) {
                if (tmp->item.first == item.first) {
                    flag = 1;
                    break;
                }
                if(tmp == table[hash_value].second)
                    break;
                tmp = tmp->next;
            }
            if (flag == 0) {
                Node * cur = new Node(item);
                if(table[hash_value].second == finish) {
                    finish = cur;
                } else {
                    table[hash_value].second->next->pref = cur;
                    cur->next = table[hash_value].second->next;
                }
                table[hash_value].second->next = cur;
                cur->pref = table[hash_value].second;
                table[hash_value].second = cur;
                count++;
                update();
            }
        }
    }
    template<typename Iter>
    HashMap(Iter in, Iter out, Hash new_hasher) {
        HashMap new_hash_map(new_hasher);
        while (in != out) {
            new_hash_map.insert(*in);
            in++;
        }
        *this = new_hash_map;
    }
    template<typename Iter>
    HashMap(Iter in, Iter out) : hasher(std::hash<KeyType>()) {
        HashMap new_hash(in, out, hasher);
        *this = new_hash;
    }
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> list, Hash new_hasher) {
        HashMap new_hash_map(new_hasher);
        for (auto it = list.begin(); it != list.end(); it++) {
            new_hash_map.insert(*it);
        }
        *this = new_hash_map;
    }
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> list) : hasher(Hash()) {
        HashMap new_hasher(list, hasher);
        *this = new_hasher;
    }
    size_t size() const {
        return count;
    }
    bool empty() const {
        return (count == 0);
    }
    Hash hash_function() const {
        return hasher;
    }
    void erase(const KeyType key) {
        size_t hash_value = hasher(key) % length;
        if(table[hash_value].first == nullptr)
            return;
        Node * cur = table[hash_value].first;
        for(;;) {
            if (cur->item.first == key) {
                if (cur->next == nullptr) {
                    cur->pref->next = nullptr;
                    finish = cur->pref;
                    table[hash_value].second = finish;
                    if (table[hash_value].first == cur)
                        table[hash_value] = {nullptr, nullptr};
                    delete cur;
                } else {
                    cur->pref->next = cur->next;
                    cur->next->pref = cur->pref;
                    if (cur == table[hash_value].second && cur == table[hash_value].first)
                        table[hash_value] = {nullptr, nullptr};
                    else
                    if(cur == table[hash_value].second) {
                        table[hash_value].second = cur->pref;
                    }
                    if(cur == table[hash_value].first) {
                        table[hash_value].first = cur->next;
                    }
                    delete cur;
                }
                count--;
                break;
            }
            if (cur == table[hash_value].second)
                break;
            cur = cur->next;
        }
    }
    struct iterator {
        Node * cur;
        iterator() {
            cur = nullptr;
        }
        iterator(Node * tmp) {
            cur = tmp;
        }
        iterator operator ++(int) {
            auto tmp = cur;
            cur = cur->next;
            return iterator(tmp);
        }
        iterator& operator ++() {
            cur = cur->next;
            return *this;
        }
        std::pair<const KeyType, ValueType>& operator *() {
            return cur->item;
        }
        std::pair<const KeyType, ValueType>* operator ->() {
            return &cur->item;
        }
        bool operator == (iterator second) {
            return cur == second.cur;
        }
        bool operator != (iterator second) {
            return !(*this == second);
        }
    };
    struct const_iterator {
        const Node * cur;
        const_iterator() {
            cur = nullptr;
        }
        const_iterator(const Node * tmp) {
            cur = tmp;
        }
        const_iterator operator ++(int) {
            const Node * tmp = cur;
            cur = cur->next;
            const_iterator ans(tmp);
            return ans;
        }
        const_iterator& operator ++() {
            cur = cur->next;
            return *this;
        }
        const std::pair<const KeyType, ValueType>& operator *() const {
            return cur->item;
        }
        const std::pair<const KeyType, ValueType> * operator -> () const {
            return &cur->item;
        }
        bool operator == (const_iterator second) {
            return cur == second.cur;
        }
        bool operator != (const_iterator second) {
            return !(*this == second);
        }
    };
    iterator begin() {
        iterator it(start);
        it++;
        return it;
    }
    const_iterator begin() const {
        const_iterator it(start);
        it++;
        return it;
    }
    iterator end() {
        return iterator();
    }
    const_iterator end() const {
        return const_iterator();
    }
    iterator find(KeyType key) {
        size_t hash_value = hasher(key) % length;
        if (table[hash_value].first == nullptr)
            return iterator();
        Node * cur = table[hash_value].first;
        while (cur != table[hash_value].second->next) {
            if (cur->item.first == key) {
                return iterator(cur);
            }
            cur = cur->next;
        }
        return iterator();
    }
    const_iterator find(KeyType key) const {
        size_t hash_value = hasher(key) % length;
        if (table[hash_value].first == nullptr)
            return const_iterator();
        Node * cur = table[hash_value].first;
        while (cur != table[hash_value].second->next) {
            if (cur->item.first == key) {
                return const_iterator(cur);
            }
            cur = cur->next;
        }
        return const_iterator();
    }
    ValueType& operator [](KeyType key){
        insert(std::make_pair(key, ValueType()));
        size_t hash_value = hasher(key) % length;
        Node * cur = table[hash_value].first;
        while (cur->item.first != key)
            cur = cur->next;
        return cur->item.second;
    }
    ValueType const & at(KeyType key) const {
        size_t hash_value = hasher(key) % length;
        if(table[hash_value].first == nullptr)
            throw std::out_of_range("Sorry");
        Node * cur = table[hash_value].first;
        while (cur != table[hash_value].second->next) {
            if (cur->item.first == key)
                return cur->item.second;
            cur = cur->next;
        }
        throw std::out_of_range("Sorry");
    }
    HashMap(const HashMap& second) {
        if (&*this == &second) {
            return;
        }
        if (table.size() != 0) {
            Node *cur = start;
            while (cur != nullptr) {
                Node *tmp = cur->next;
                delete cur;
                cur = tmp;
            }
        }
        table.resize(0);
        table.resize(second.table.size(), {nullptr, nullptr});
        hasher = second.hash_function();
        start = new Node();
        finish = start;
        for (int i = 0; i < table.size(); i++) {
            Node * cur = second.table[i].first;
            if (cur != nullptr) {
                while (cur != second.table[i].second->next) {
                    Node *tmp = new Node(cur->item);
                    finish->next = tmp;
                    tmp->pref = finish;
                    finish = tmp;
                    if (table[i].first == nullptr)
                        table[i].first = finish;
                    table[i].second = finish;
                    cur = cur->next;
                }
            }
        }
        length = table.size();
        count = second.count;
    }
    HashMap& operator =(HashMap& second) {
        if (&*this == &second)
            return *this;
        if(table.size() != 0) {
            Node *cur = start;
            while (cur != nullptr) {
                Node *tmp = cur->next;
                delete cur;
                cur = tmp;
            }
        }
        table.resize(0);
        table.resize(second.table.size(), {nullptr, nullptr});
        hasher = second.hash_function();
        start = new Node();
        finish = start;
        for (size_t i = 0; i < table.size(); i++) {
            Node * cur = second.table[i].first;
            if(cur != nullptr) {
                while (cur != second.table[i].second->next) {
                    Node *tmp = new Node(cur->item);
                    finish->next = tmp;
                    tmp->pref = finish;
                    finish = tmp;
                    if(table[i].first == nullptr)
                        table[i].first = finish;
                    table[i].second = finish;
                    cur = cur->next;
                }
            }
        }
        length = table.size();
        count = second.count;
        return *this;
    }
    ~HashMap() {
        Node * cur = start;
        while (cur != nullptr) {
            Node * tmp = cur->next;
            delete cur;
            cur = tmp;
        }
    }
};
