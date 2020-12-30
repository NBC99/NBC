# The NBC Ledger

The NBC Ledger is a decentralized cryptographic ledger powered by a network of peer-to-peer servers. The NBC Ledger uses a novel Byzantine Fault Tolerant consensus algorithm to settle and record transactions in a secure distributed database without a central operator.

## NBC
NBC is a public, counterparty-less asset native to the NBC Ledger, and is designed to bridge the many different currencies in use worldwide. NBC is traded on the open-market and is available for anyone to access. The NBC Ledger was created in 2012 with a finite supply of 100 billion units of NBC. Its creators gifted 80 billion NBC to a company, now called [NBC], to develop the NBC Ledger and its ecosystem.  NBC uses NBC the help build the Internet of Value, ushering in a world in which money moves as fast and efficiently as information does today.

## NBC
The server software that powers the NBC Ledger is called `NBC` and is available in this repository under the permissive [ISC open-source license](LICENSE). The `NBC` server is written primarily in C++ and runs on a variety of platforms.

### Build from Source

* [Linux](Builds/linux/README.md)
* [Mac](Builds/macos/README.md)
* [Windows](Builds/VisualStudio2017/README.md)

## Key Features of the NBC Ledger

- **[Censorship-Resistant Transaction Processing][]:** No single party decides which transactions succeed or fail, and no one can "roll back" a transaction after it completes. As long as those who choose to participate in the network keep it healthy, they can settle transactions in seconds.
- **[Fast, Efficient Consensus Algorithm][]:** The NBC Ledger's consensus algorithm settles transactions in 4 to 5 seconds, processing at a throughput of up to 1500 transactions per second. These properties put NBC at least an order of magnitude ahead of other top digital assets.
- **[Finite NBC Supply][]:** When the NBC Ledger began, 100 billion NBC were created, and no more NBC will ever be created. (Each NBC is subdivisible down to 6 decimal places, for a grand total of 100 quintillion _drops_ of NBC.) The available supply of NBC decreases slowly over time as small amounts are destroyed to pay transaction costs.
- **[Responsible Software Governance][]:** A team of full-time, world-class developers at NBC maintain and continually improve the NBC Ledger's underlying software with contributions from the open-source community. NBC acts as a steward for the technology and an advocate for its interests, and builds constructive relationships with governments and financial institutions worldwide.
- **[Secure, Adaptable Cryptography][]:** The NBC Ledger relies on industry standard digital signature systems like ECDSA (the same scheme used by Bitcoin) but also supports modern, efficient algorithms like Ed25519. The extensible nature of the NBC Ledger's software makes it possible to add and disable algorithms as the state of the art in cryptography advances.
- **[Modern Features for Smart Contracts][]:** Features like Escrow, Checks, and Payment Channels support cutting-edge financial applications including the [Interledger Protocol](https://interledger.org/). This toolbox of advanced features comes with safety features like a process for amending the network and separate checks against invariant constraints.
- **[On-Ledger Decentralized Exchange][]:** In addition to all the features that make NBC useful on its own, the NBC Ledger also has a fully-functional accounting system for tracking and trading obligations denominated in any way users want, and an exchange built into the protocol. The NBC Ledger can settle long, cross-currency payment paths and exchanges of multiple currencies in atomic transactions, bridging gaps of trust with NBC.

### Repository Contents

| Folder     | Contents                                         |
|:-----------|:-------------------------------------------------|
| `./bin`    | Scripts and data files for NBC integrators.   |
| `./Builds` | Platform-specific guides for building `NBC`. |
| `./docs`   | Source documentation files and doxygen config.   |
| `./cfg`    | Example configuration files.                     |
| `./src`    | Source code.                                     |

Some of the directories under `src` are external repositories included using
git-subtree. See those directories' README files for more details.
