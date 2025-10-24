/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "PortDriven.hpp"
#include "../ConfigureGuard.hpp"

using namespace iodrivers_base;
using namespace iodrivers_base::test;
using namespace std;

static const int DUMMY_BUFFER_SIZE = 4;

namespace
{
    class DummyDriver : public iodrivers_base::Driver {
    public:
        DummyDriver()
            : iodrivers_base::Driver(4) {}

        int extractPacket(boost::uint8_t const* buffer, size_t size) const
        {
            if (size >= 4) {
                return 4;
            }
            return 0;
        }
    };
}

PortDriven::PortDriven(std::string const& name)
    : PortDrivenBase(name)
{
}

PortDriven::~PortDriven()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See PortDriven.hpp for more detailed
// documentation about them.

bool PortDriven::configureHook()
{
    unique_ptr<Driver> driver(new DummyDriver());
    // Un-configure the device driver if the configure fails.
    // You MUST call guard.commit() once the driver is fully
    // functional (usually before the configureHook's "return true;"
    iodrivers_base::ConfigureGuard guard(this);
    if (!_io_port.get().empty())
        driver->openURI(_io_port.get());
    setDriver(driver.get());

    // This is MANDATORY and MUST be called after the setDriver but before you do
    // anything with the driver
    if (!PortDrivenBase::configureHook())
        return false;

    // If some device configuration was needed, it must be done after the
    // setDriver and call to configureHook on TaskBase (i.e., here)

    mDriver = move(driver);
    guard.commit();
    return true;

}
bool PortDriven::startHook()
{
    if (! PortDrivenBase::startHook())
        return false;
    return true;
}
void PortDriven::updateHook()
{
    PortDrivenBase::updateHook();
}
void PortDriven::processIO()
{
    uint8_t buffer[DUMMY_BUFFER_SIZE];
    int packet_size = mDriver->readPacket(buffer, DUMMY_BUFFER_SIZE);

    RawPacket packet;
    packet.time = base::Time::now();
    packet.data = std::vector<uint8_t>(buffer, buffer + packet_size);
    _rx.write(packet);
}

void PortDriven::errorHook()
{
    PortDrivenBase::errorHook();
}
void PortDriven::stopHook()
{
    PortDrivenBase::stopHook();
}
void PortDriven::cleanupHook()
{
    PortDrivenBase::cleanupHook();
}
