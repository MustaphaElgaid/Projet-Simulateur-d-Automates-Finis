#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include <utility>  
#include <functional> 
#include <stdexcept>

#include <string>
#include <vector>
#include <type_traits>


namespace graph{ 
template <typename N, typename ND, typename Hash, typename Equal>
struct NodeData {
    ND m_data;
    std::unordered_set<N, Hash, Equal> m_succs;
    std::unordered_set<N, Hash, Equal> m_preds;

    NodeData() : m_data() {}
    NodeData(const ND & mdata) : m_data(mdata), m_succs(), m_preds() {}

    void add_succ(const N& succ) {
        m_succs.insert(succ);
    }

    void add_pred(const N& pred) {
        m_preds.insert(pred);
    }
};

template <typename N, typename Hash = std::hash<N>>
struct pair_hash
{
    std::size_t operator()(const std::pair<N, N>& v) const{
        std::size_t v1 = Hash()(v.first);
        std::size_t v2 = Hash()(v.second);
    return v2 + 0x9e3779b9 + (v1<<6) + (v1>>2);
    }
};

template <typename N, typename Equal = std::equal_to<N>>
struct pair_eq
{
    bool operator()(const std::pair<N, N>& lhs, const std::pair<N, N>& rhs) const {
        return Equal()(lhs.first, rhs.first) and Equal()(lhs.second, rhs.second);
    }
};

struct GetSuccs {
    template <typename N, typename ND, typename Hash, typename Equal>
    static auto& get(const NodeData<N, ND, Hash, Equal>& nodeData) {
        return nodeData.m_succs;
    }
};

struct GetPreds {
    template <typename N, typename ND, typename Hash, typename Equal>
    static auto& get(const NodeData<N, ND, Hash, Equal>& nodeData) {
        return nodeData.m_preds;
    }
};


template <typename G, typename GetSet>
struct struct_traversal {
    G const& g;
    typename G::Neff node;

    struct_traversal(G const& graph, typename G::Neff n) : g(graph), node(n) {}

    auto begin() -> decltype(GetSet::get(g.m_nodes.at(node)).begin()) {
        if (!g.has_node(node)) {
            throw std::runtime_error("Node n'est pas trouve");
        }
        return GetSet::get(g.m_nodes.at(node)).begin();
    }

    auto end() -> decltype(GetSet::get(g.m_nodes.at(node)).end()) {
        if (!g.has_node(node)) {
            throw std::runtime_error("Node n'est pas trouve");
        }
        return GetSet::get(g.m_nodes.at(node)).end();
    }

    auto begin() const -> decltype(GetSet::get(g.m_nodes.at(node)).cbegin()) {
        if (!g.has_node(node)) {
            throw std::runtime_error("Node n'est pas trouve");
        }
        return GetSet::get(g.m_nodes.at(node)).cbegin();
    }

    auto end() const -> decltype(GetSet::get(g.m_nodes.at(node)).cend()) {
        if (!g.has_node(node)) {
            throw std::runtime_error("Node n'est pas trouve");
        }
        return GetSet::get(g.m_nodes.at(node)).cend();
    }
};



//  using struct_traversal pour struct_next et struct_pred
template <typename G>
using struct_next = struct_traversal<G, GetSuccs>;

template <typename G>
using struct_pred = struct_traversal<G, GetPreds>;


template <typename T>
constexpr bool is_type_large = sizeof(T) > 4;



template <typename N, typename ND, typename ED, typename Hash= std::hash<N>, typename Equal= std::equal_to<N>>
struct Graph
{
    using Neff = typename std::conditional<is_type_large<N>, N const &, N const>::type;
    using NDeff = typename std::conditional<is_type_large<ND>, ND const &, ND const>::type;
    using EDeff = typename std::conditional<is_type_large<ED>, ED const &, ED const>::type;
    using Eeff = typename std::conditional<is_type_large<std::pair<N, N>>, std::pair<N, N> const &, std::pair<N, N> const>::type;
    using gset_t = std::unordered_set<N, Hash, Equal>;
    using node_t = N;
    using map_nodes_t  = std::unordered_map<node_t, NodeData<N, ND, Hash, Equal>, Hash, Equal>;
    using map_edges_t = std::unordered_map<std::pair<node_t, node_t>, ED, pair_hash<node_t, Hash>, pair_eq<node_t, Equal>>;
    map_nodes_t m_nodes;
    map_edges_t m_edges;
    
    

    Graph() = default;


    bool has_node(Neff node) const {
        return m_nodes.find(node) != m_nodes.end();
    }

    void add_node(const N n, const ND d){
        NodeData<N, ND, Hash, Equal> node_dt(d);
        m_nodes.emplace(n, node_dt);
    }

    void add_edge(N pred, N succ, ED data) {

        std::pair<N, N> edge(pred, succ);

        m_edges.emplace(edge, data);

        auto resultPred = m_nodes.emplace(pred, NodeData<N, ND, Hash, Equal>(ND{}));
            resultPred.first->second.add_succ(succ);

        auto resultSucc = m_nodes.emplace(succ, NodeData<N, ND, Hash, Equal>(ND{}));
            resultSucc.first->second.add_pred(pred);
        
    }


    ND& operator[](Neff node) {
        auto it = m_nodes.find(node);
        if (it != m_nodes.end()) {
            return it->second.m_data;
        }
        //  gérer l'erreur
        throw std::runtime_error("Node n'est pas trouve");
    }

    ED& operator[](Eeff edge) {
        auto it = m_edges.find(edge);
        if (it != m_edges.end()) {
            return it->second;
        }
        throw std::runtime_error("Edge n'est pas trouve");
    }

    NDeff operator[](Neff node) const {
        auto it = m_nodes.find(node);
        if (it != m_nodes.end()) {
            return it->second.m_data;
        }
        throw std::runtime_error("Node n'est pas trouve");
    }

    EDeff operator[](Eeff edge) const {
        auto it = m_edges.find(edge);
        if (it != m_edges.end()) {
            return it->second;
        }
        throw std::runtime_error("Edge n'est pas trouve");
    }



    struct_next<Graph> next(Neff node) {
    return struct_next<Graph>(*this, node);
    }

    const struct_next<Graph> next(Neff node) const {
        return struct_next<Graph>(*this, node);
    }

    struct_pred<Graph> pred(Neff node) {
        return struct_pred<Graph>(*this, node);
    }

    const struct_pred<Graph> pred(Neff node) const {
        return struct_pred<Graph>(*this, node);
    }


};




template <typename N, typename Hash, typename Equal>
struct NodeData<N, void, Hash, Equal> {
    std::unordered_set<N, Hash, Equal> m_succs;
    std::unordered_set<N, Hash, Equal> m_preds;
    NodeData() : m_succs(), m_preds() {}

    void add_succ(const N& succ) {
        m_succs.insert(succ);
    }

    void add_pred(const N& pred) {
        m_preds.insert(pred);
    }
};



template <typename N, typename ED, typename Hash, typename Equal>
struct Graph<N, void, ED, Hash, Equal> {
    using Neff = typename std::conditional<is_type_large<N>, N const &, N const>::type;
    using EDeff = typename std::conditional<is_type_large<ED>, ED const &, ED const>::type;
    using Eeff = typename std::conditional<is_type_large<std::pair<N, N>>, std::pair<N, N> const &, std::pair<N, N> const>::type;
    using gset_t = std::unordered_set<N, Hash, Equal>;
    using node_t = N;
    using map_nodes_t  = std::unordered_map<node_t, NodeData<N, void, Hash, Equal>, Hash, Equal>;
    using map_edges_t = std::unordered_map<std::pair<node_t, node_t>, ED, pair_hash<node_t, Hash>, pair_eq<node_t, Equal>>;

    map_nodes_t m_nodes;
    map_edges_t m_edges;

    Graph() = default;

    void add_node(N n) {
        m_nodes.emplace(n, NodeData<N, void, Hash, Equal>());
    }
/* 
    void add_edge(N pred, N succ, ED data) {
        std::pair<N, N> edge(pred, succ);
        m_edges.emplace(edge, data);
        m_nodes[pred].add_succ(succ);
        m_nodes[succ].add_pred(pred);
    }

 */  
    void add_edge(N pred, N succ, ED data) {

        std::pair<N, N> edge(pred, succ);

        m_edges.emplace(edge, data);

        auto resultPred = m_nodes.emplace(pred, NodeData<N, void, Hash, Equal>());
            resultPred.first->second.add_succ(succ);

        auto resultSucc = m_nodes.emplace(succ, NodeData<N, void, Hash, Equal>());
            resultSucc.first->second.add_pred(pred);
        
    }

    ED& operator[](Eeff edge) {
        auto it = m_edges.find(edge);
        if (it != m_edges.end()) {
            return it->second;
        }
        throw std::runtime_error("Edge n'est pas trouve");
    }

    EDeff operator[](Eeff edge) const {
        auto it = m_edges.find(edge);
        if (it != m_edges.end()) {
            return it->second;
        }
        throw std::runtime_error("Edge n'est pas trouve");
    }
};







template <typename N, typename ND, typename Hash, typename Equal>
struct Graph<N, ND, void, Hash, Equal> {
    using Neff = typename std::conditional<is_type_large<N>, N const &, N const>::type;
    using NDeff = typename std::conditional<is_type_large<ND>, ND const &, ND const>::type;
    using gset_t = std::unordered_set<N, Hash, Equal>;
    using node_t = N;
    using map_nodes_t = std::unordered_map<node_t, NodeData<N, ND, Hash, Equal>, Hash, Equal>;
    using map_edges_t = std::unordered_set<std::pair<node_t, node_t>,  pair_hash<node_t, Hash>, pair_eq<node_t, Equal>>;
    map_edges_t m_edges;
    map_nodes_t m_nodes;

    Graph() = default;

    void add_node(N n, ND d) {
        m_nodes.emplace(n, NodeData<N, ND, Hash, Equal>(d));
    }

    bool has_node(Neff node) const {
        return m_nodes.find(node) != m_nodes.end();
    }

    void add_edge(N pred, N succ) {
        m_edges.emplace(std::make_pair(pred, succ));
    
        auto resultPred = m_nodes.emplace(pred, NodeData<N, ND, Hash, Equal>(ND{}));
        resultPred.first->second.add_succ(succ);

        auto resultSucc = m_nodes.emplace(succ, NodeData<N, ND, Hash, Equal>(ND{}));
        resultSucc.first->second.add_pred(pred);
    }

    ND& operator[](Neff node) {
        auto it = m_nodes.find(node);
        if (it != m_nodes.end()) {
            return it->second.m_data;
        }
        throw std::runtime_error("Node not found");
    }

    NDeff operator[](Neff node) const {
        auto it = m_nodes.find(node);
        if (it != m_nodes.end()) {
            return it->second.m_data;
        }
        throw std::runtime_error("Node not found");
    }

     struct_next<Graph> next(Neff node) {
        return struct_next<Graph>(*this, node);
    }

    const struct_next<Graph> next(Neff node) const {
        return struct_next<Graph>(*this, node);
    }

    struct_pred<Graph> pred(Neff node) {
        return struct_pred<Graph>(*this, node);
    }

    const struct_pred<Graph> pred(Neff node) const {
        return struct_pred<Graph>(*this, node);
    }
};

} 
#endif


//----------------------------------------------------