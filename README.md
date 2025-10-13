
# ⚽ World Cup Statistics Manager

A C++ project that implements a World Cup management system, designed for efficient tracking of teams, players, and matches using custom data structures.
It combines AVL trees, Rank Trees, and a Union-Find Hash system to handle complex operations while maintaining optimal runtime and memory performance.

🔹 Key Concepts: AVL Trees, Rank Trees, Union-Find, Hash Tables, Algorithm Design, Data Structures

🔹 Features:

Manage all teams and players in dynamic data structures

Insert and remove teams or players efficiently (O(log n) operations)

Simulate matches and update points, cards, and stats automatically

Merge teams with full player integration while preserving hierarchy

Calculate player stats like games played and partial “spirit”

Rank teams by ability and query by index efficiently

Full memory management and complexity control

🧩 Core Components:

Avl<Team> — stores all teams sorted by ID

RankTree<Team> — maintains teams ordered by ability for quick ranking

UnionFindHash — manages all players using hashed disjoint sets with path compression

Each Team tracks its players, points, ability sum, matches, and team spirit permutation

Each Player stores ID, ability, cards, games played, role (goalkeeper), and partial spirit

# 🛠️ Tech Stack

Language: C++

Core Concepts: AVL Trees | Union-Find | Hashing | Algorithms | Object-Oriented Design | Memory Management
