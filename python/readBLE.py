# bluetooth low energy scan
from bluetooth.ble import DiscoveryService
# simple inquiry example
import bluetooth

service = DiscoveryService()
devices = service.discover(2)

nearby_devices = bluetooth.discover_devices(lookup_names=True)
print("Found {} devices.".format(len(nearby_devices)))

for addr, name in nearby_devices:
    print("  {} - {}".format(addr, name))

for address, name in devices.items():
    print("name: {}, address: {}".format(name, address))