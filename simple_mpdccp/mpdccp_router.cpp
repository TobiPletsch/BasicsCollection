#include <cstdlib>
#include <iostream>

void executeCommand(const std::string &command) {
    if (system(command.c_str()) != 0) {
        std::cerr << "Error executing command: " << command << std::endl;
    } else {
        std::cout << "### " << command << " ### executed successfully " << std::endl;
    }
}

bool checkInterfaceExists(const std::string &interfaceName) {
    std::string command = "ip link show " + interfaceName + " > /dev/null 2>&1";
    return system(command.c_str()) == 0;
}

void createVirtualInterface(const std::string &interfaceName) {
    if (checkInterfaceExists(interfaceName)) {
        return;
    }
    std::string command = "sudo ip link add " + interfaceName + " type dummy";
    executeCommand(command);
}

void activateInterface(const std::string& interfaceName) {
    std::string command = "sudo ip link set " + interfaceName + " up";
    executeCommand(command);
}

void setIPAddress(const std::string &interfaceName, const std::string &ipAddress) {
    executeCommand("sudo ip addr flush dev " + interfaceName);
    executeCommand("sudo ip addr add " + ipAddress + " dev " + interfaceName);
}

void setGateway(const std::string &gatewayIp) {
    std::string command = "sudo ip route add default via " + gatewayIp;
    executeCommand(command);
}

void setRoute(const std::string& destination, const std::string& gateway, const std::string& interfaceName) {
    std::string command = "sudo ip route add " + destination + " via " + gateway + " dev " + interfaceName;
    executeCommand(command);
}

void configureRouting(const std::string& interfaceName, const std::string& ipAddress, const std::string& gateway, const std::string& table, const std::string& fwmark) {
    // executeCommand("sudo ip rule add from all fwmark " + fwmark + " table " + table);
    // executeCommand("sudo iptables -A OUTPUT -s " + ipAddress + " -t mangle -j MARK --set-mark " + fwmark);
    // executeCommand("sudo ip route add " + ipAddress.substr(0, ipAddress.find_last_of('.')) + ".0/24 dev " + interfaceName + " scope link table " + table);
    // executeCommand("sudo ip route add default via " + gateway + " dev " + interfaceName + " table " + table);

    executeCommand("sudo iptables -t nat -A POSTROUTING -o ens256 -j MASQUERADE");
    executeCommand("sudo iptables -A FORWARD -i ens192 -o ens256 -j ACCEPT");
    executeCommand("sudo iptables -A FORWARD -i ens224 -o ens256 -j ACCEPT");
}

void setInterfaceState(const std::string& interfaceName, const std::string& state) {
    std::string command = "sudo ip link set " + interfaceName + " " + state;
    executeCommand(command);
}

void enableMPDCCP(const std::string &interfaceName) {
    std::string command = "sudo bash -c 'printf \"0x%x\\n\" $((($(cat \"/sys/class/net/" + interfaceName + "/flags\"))|0x200000))' | sudo tee /sys/class/net/" + interfaceName + "/flags";
    executeCommand(command);
}

void startIperfServer() {
    std::string command = "iperf3 -s -i 1 -4";
    executeCommand(command);
}

void startIperfClientMPDCCP(const std::string &serverIp) {
    std::string command = "iperf3 -c " + serverIp + " -i 1 -4 --dccp --multipath -t 10";
    executeCommand(command);
}

void startIperfClientDCCP(const std::string &serverIp) {
    std::string command = "iperf3 -c " + serverIp + " -i 1 -4 --dccp -t 10";
    executeCommand(command);
}

void stopIperfClient() {
    std::string command = "pkill -f 'iperf3 -c'";
    executeCommand(command);
}

int main() {
    createVirtualInterface("ens192");
    setIPAddress("ens192", "192.168.100.11/24");
    setInterfaceState("ens192", "up");

    createVirtualInterface("ens224");
    setIPAddress("ens224", "192.168.101.11/24");
    setInterfaceState("ens224", "up");

    createVirtualInterface("ens256");
    setIPAddress("ens256", "192.168.102.11/24");
    setInterfaceState("ens256", "up");

    // Enable IP forwarding
    executeCommand("sudo sysctl -w net.ipv4.ip_forward=1");
    executeCommand("sudo iptables -t nat -A POSTROUTING -o ens256 -j MASQUERADE");

    // executeCommand("sudo iptables -A FORWARD -i ens192 -o ens256 -j ACCEPT");
    // executeCommand("sudo iptables -A FORWARD -i ens224 -o ens256 -j ACCEPT");
    // executeCommand("sudo iptables -A FORWARD -m mark --mark 0x100 -j ACCEPT");
    // executeCommand("sudo iptables -A FORWARD -m mark --mark 0x101 -j ACCEPT");

    //merge above rules
    // executeCommand("sudo iptables -A FORWARD -i ens192 -o ens256 -m mark --mark 0x100 -j ACCEPT");
    // executeCommand("sudo iptables -A FORWARD -i ens224 -o ens256 -m mark --mark 0x101 -j ACCEPT");


    return 0;
}

