# semi-automatic-manufacturing-cell
Design and integration of a semi‑automatic manufacturing cell combining CNC machining, PLC programming, HMI design, Cognex vision, UR cobot, IoT monitoring (ESP32 + Node‑RED), and Siemens Plant Simulation digital twin. Developed at ITESM Querétaro with real‑time data exchange.

## Table of Contents
1. Introduction
2. Manufacturing Process (CIMA)
3. Automation & Integration
4. IoT Monitoring
5. Digital Twin Simulation
6. Access Control
7. Team
---

### 1. Introduction
This project addresses the challenge of modern manufacturing by combining machining, automation, and digital technologies. The goal was to design a semi‑automatic production line capable of handling parts, monitoring data in real time, and demonstrating interoperability between industrial systems.

### 2. Manufacturing Process (CIMA)
The team designed and machined custom V‑blocks using CAD models and CNC milling. A structured process sheet defined the machining steps, tools, and parameters, ensuring precision and consistency in the production of components.

### 3. Automation & Integration
A Siemens PLC controlled the manufacturing cell, supported by an HMI for operator interaction. The Cognex vision system identified parts, while a UR collaborative robot executed pick‑and‑place routines. Together, these elements automated handling and processing tasks within the cell.

### 4. IoT Monitoring
An ESP32 microcontroller with current sensors collected energy consumption data and transmitted it via MQTT. Node‑RED provided a dashboard for real‑time visualization of system states, production metrics, and alerts, enabling accessible monitoring of the cell’s performance.

### 5. Digital Twin Simulation
Siemens Plant Simulation was used to create a digital twin of the production line. This model represented the flow of parts, resource allocation, and logistics, offering insights into throughput and efficiency while supporting decision‑making for process optimization.

### 6. Access Control
To ensure secure operation, the system integrated RFID authentication and a Face ID module developed in Python with OpenCV. These features allowed only authorized users to access and control the manufacturing cell, combining cybersecurity with industrial automation.

### 7. Team 
Developed by **Team MARO** at ITESM Campus Querétaro: - Jair André Ibarra Campa - Ana Laura Rosas Balderas - Jorge Julián Razura Piña - José Manuel Morales Murueta
