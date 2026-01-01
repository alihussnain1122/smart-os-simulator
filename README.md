# OS Simulator in C

A comprehensive command-line-based simulator for various operating system concepts, implemented in C. This project provides a hands-on approach to understanding core OS functionalities.

## 🚀 Features

This simulator# OS Simulator in C

A comprehensive command-line-based simulator for various operating system concepts, implemented in C. This project provides a hands-on approach to understanding core OS functionalities.

## 🚀 Features

This simulator covers a wide range of OS topics, including:

*   **CPU Scheduling Algorithms**
    *   First-Come, First-Served (FCFS)
    *   Shortest Job First (SJF)
    *   Round Robin
*   **Memory Management Techniques**
    *   First-Fit
    *   Best-Fit
    *   Worst-Fit
*   **Paging and Page Replacement Algorithms**
    *   First-In, First-Out (FIFO)
    *   Least Recently Used (LRU)
*   **Deadlock Avoidance**
    *   Banker's Algorithm
*   **Inter-Process Communication (IPC)**
    *   Shared Memory
*   **Core Concepts**
    *   Process Management
    *   Multithreading

## 📂 Project Structure

The project is organized into modules, with each directory focusing on a specific OS concept:

```
.
├── cpu/              # CPU scheduling algorithms
├── deadlock/         # Deadlock avoidance algorithms
├── ipc/              # Inter-process communication
├── memory/           # Memory management algorithms
├── paging/           # Paging and page replacement
├── process/          # Process creation and management
├── src/              # Main application logic and reporting
├── threads/          # Thread management
└── makefile          # Build script
```

## 🛠️ How to Compile and Run

This project uses a  for easy compilation.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/alihussnain1122/smart-os-simulator.git
    cd os_simulator
    ```

2.  **Compile the project:**
    Open your terminal and run the `make` command. This will compile the source files and create an executable.
    ```bash
    make
    ```

3.  **Run the simulator:**
    After successful compilation, an executable file (e.g., `os_simulator`) will be created. Run it from the terminal:
    ```bash
    ./os_simulator
    ```
    *(Note: The executable name may vary based on the  configuration.)*

## 💡 Usage

Once running, the simulator will present a menu-driven interface allowing you to:
*   Run each algorithm individually to observe its behavior.
*   Execute an integrated simulation that combines multiple OS concepts.
*   View performance reports and metrics for each simulation.

## 🤝 Contributing

Contributions are welcome! If you have suggestions for improvements or want to add more features, please feel free to:
1.  Fork the repository.
2.  Create a new feature branch (`git checkout -b feature/YourFeature`).
3.  Commit your changes (`git commit -m 'Add some feature'`).
4.  Push to the branch (`git push origin feature/YourFeature`).
5.  Open a Pull Request.

