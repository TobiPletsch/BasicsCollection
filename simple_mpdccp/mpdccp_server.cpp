#include <iostream>
#include <cstdlib>

void executeCommand(const std::string& command) {
    if(system(command.c_str()) != 0) {
        std::cerr << "Error executing command: " << command << std::endl;
    } else {
        std::cout << "Command executed successfully: " << command << std::endl;
    }
}

void createVirtualInterface(const std::string& interfaceName) {
    if (system(("ip link show " + interfaceName + " > /dev/null 2>&1").c_str()) == 0) {
        std::cout << "Virtual interface " << interfaceName << " already exists" << std::endl;
        return;
    }

    std::string command = "sudo ip link add " + interfaceName + " type dummy";
    executeCommand(command);
}

void setIPAddress(const std::string& interfaceName, const std::string& ipAddress) {
    executeCommand("sudo ip addr flush dev " + interfaceName);
    executeCommand("sudo ip addr add " + ipAddress + " dev " + interfaceName);
}

// 
void configureRouting(const std::string& interfaceName, const std::string& ipAddress, const std::string& gateway, const std::string& table, const std::string& fwmark) {
    executeCommand("sudo ip rule add from all fwmark " + fwmark + " table " + table);
    executeCommand("sudo iptables -A OUTPUT -s " + ipAddress + " -t mangle -j MARK --set-mark " + fwmark);
    executeCommand("sudo ip route add " + ipAddress.substr(0, ipAddress.find_last_of('.')) + ".0/24 dev " + interfaceName + " scope link table " + table);
    executeCommand("sudo ip route add default via " + gateway + " dev " + interfaceName + " table " + table);
}

void setInterfaceState(const std::string& interfaceName, const std::string& state) {
    std::string command = "sudo ip link set " + interfaceName + " " + state;
    executeCommand(command);
}

void enableMPDCCP(const std::string &interfaceName) {
    std::string command = "sudo bash -c 'printf \"0x%x\\n\" $((($(cat \"/sys/class/net/" + interfaceName + "/flags\"))|0x200000))' | sudo tee /sys/class/net/" + interfaceName + "/flags";
    executeCommand(command);
}


int main() {
    createVirtualInterface("ens192");
    setIPAddress("ens192", "192.168.102.10/24");
    setInterfaceState("ens192", "up");
    enableMPDCCP("ens192");

    executeCommand("sudo ip route add 192.168.100.0/24 via 192.168.102.11 dev ens192");
    executeCommand("sudo ip route add 192.168.101.0/24 via 192.168.102.11 dev ens192");
    // startIperfServer(); or other server functionality

    return 0;
}

